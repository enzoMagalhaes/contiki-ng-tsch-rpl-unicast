/***************************************************************************//**
 * @file
 * @brief PDM microphone driver
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include <math.h>
#include "dmadrv.h"
#include "em_pdm.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "sl_sleeptimer.h"

#include "thunderboard/mic_pdm.h"

static bool dma_complete(unsigned int channel, unsigned int sequence_no, void *user_param);

// Local variables
static int16_t *sample_buffer;                // Pointer to current sample buffer
static uint32_t sample_index;                 // Current sample index of sample buffer
static uint32_t sample_count;                 // Number of samples to collect
static volatile bool reading_samples_to_buffer; // Flag to show whether buffer is currently filling with samples
static bool mic_running;                      // Flag to show if mic is running
static volatile bool mic_ready;               // Flag to show if is ready to read samples
static bool initialized;                      // Flag to show if mic is initialized
static uint8_t num_channels;                  // Number of channels
static uint32_t discard_buffer;               // DMA discard pile

static LDMA_TransferCfg_t dma_transfer_cfg = LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_PDM_RXDATAV);
static unsigned int dma_channel_id;

static LDMA_Descriptor_t dma_descriptor[2] = {
  LDMA_DESCRIPTOR_LINKREL_P2M_WORD(&PDM->RXDATA, &discard_buffer, 2047, 1),
  LDMA_DESCRIPTOR_LINKREL_P2M_WORD(&PDM->RXDATA, &discard_buffer, 2047, -1)
};

static sl_sleeptimer_timer_handle_t mic_wake_up_timer;

/***************************************************************************//**
 * @brief Callback function for sleeptimer
 ******************************************************************************/
void timeout_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;
  mic_ready = true;
}
/***************************************************************************//**
 *    Initializes the microphone
 ******************************************************************************/
sl_status_t sl_mic_init(uint32_t sample_rate, uint8_t n_channels)
{
  sl_status_t status;

  if (n_channels < 1 || n_channels > 2) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  /* Enable clocks */
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_PDM, true);

  CMU_ClockSelectSet(cmuClock_PDMREF, cmuSelect_HFXO);
  CMU_ClockEnable(cmuClock_PDMREF, true);

  /* Setup GPIO pins */
  GPIO_PinModeSet(MIC_PORT_DATA, MIC_PIN_DATA, gpioModeInput, 0);
  GPIO_PinModeSet(MIC_PORT_CLK, MIC_PIN_CLK, gpioModePushPull, 0);

  // Set fast slew rate on PDM mic CLK and DATA pins
  GPIO_SlewrateSet(MIC_PORT_DATA, 0x7, 0X7);

  /* Enable power */
  GPIO_PinModeSet(MIC_PORT_PWR_EN, MIC_PIN_PWR_EN, gpioModePushPull, 1);

  // Wait for microphones to power up
  sl_sleeptimer_delay_millisecond(50);

  /* Configure and enable clock and data routing locations */
#ifdef _SILICON_LABS_32B_SERIES_2
  GPIO->PDMROUTE.ROUTEEN |= GPIO_PDM_ROUTEEN_CLKPEN;
  GPIO->PDMROUTE.DAT0ROUTE |= (MIC_PORT_DATA << _GPIO_PDM_DAT0ROUTE_PORT_SHIFT) | (MIC_PIN_DATA << _GPIO_PDM_DAT0ROUTE_PIN_SHIFT);
  GPIO->PDMROUTE.CLKROUTE |= (MIC_PORT_CLK << _GPIO_PDM_CLKROUTE_PORT_SHIFT) | (MIC_PIN_CLK << _GPIO_PDM_CLKROUTE_PIN_SHIFT);
#else
  PDM->ROUTELOC0 = (PDM->ROUTELOC0 & ~_PDM_ROUTELOC0_DAT0LOC_MASK)
                   | (MIC_PDM_LOC_DATA << _PDM_ROUTELOC0_DAT0LOC_SHIFT);
  PDM->ROUTELOC1 = MIC_PDM_LOC_CLK << _PDM_ROUTELOC1_CLKLOC_SHIFT;
  PDM->ROUTEPEN |= PDM_ROUTEPEN_CLKPEN | PDM_ROUTEPEN_DAT0PEN;
#endif

  // DSR at 32
  uint8_t dsr = 32;

  // Calculate gain based on DSR and filter order
  uint8_t gain = 32 - (uint8_t)(1 + ceil(log10f(pow(dsr, 5)) / log10f(2)));

  // Calculate necessary prescaler based on desired sample rate and DSR
  uint32_t clock_freq = CMU_ClockFreqGet(cmuClock_PDM);
  uint32_t prescaler_val = (clock_freq / (sample_rate * dsr)) - 1;

  if (prescaler_val > 1024) {
    return SL_STATUS_FAIL;
  }

  // Initialize the PDM
  PDM_Init_TypeDef init = PDM_INIT_DEFAULT;
  init.prescaler = prescaler_val;
  init.dsr = dsr;
  init.gain = gain;

  // Don't start PDM filter after initialization
  init.start = false;

#if defined(PDM_CTRL_OUTCLKEN)
  init.outClkEn = false;
#endif

  if (n_channels == 1) {
    // Right-align the 16-bit sample in FIFO
    init.dataFormat = pdmDataFormatRight16;
    init.enableCh0Ch1Stereo = false;
    init.numChannels = pdmNumberOfChannelsOne;
  } else if (n_channels == 2) {
    // Pack two 16-bit samples in one 32-bit FIFO entry
    init.dataFormat = pdmDataFormatDouble16;
    init.enableCh0Ch1Stereo = true;
    init.numChannels = pdmNumberOfChannelsTwo;
  }

  PDM_Init(PDM, &init);

  // Setup DMA
  DMADRV_Init();
  status = DMADRV_AllocateChannel(&dma_channel_id, NULL);

  if ( status != ECODE_EMDRV_DMADRV_OK ) {
    return SL_STATUS_FAIL;
  }

  dma_descriptor[0].xfer.dstInc = ldmaCtrlDstIncNone;
  dma_descriptor[0].xfer.size = ldmaCtrlSizeWord;

  dma_descriptor[1].xfer.dstInc = ldmaCtrlDstIncNone;
  dma_descriptor[1].xfer.size = ldmaCtrlSizeWord;

  if (n_channels == 1) {
    dma_descriptor[0].xfer.size = ldmaCtrlSizeHalf;
    dma_descriptor[1].xfer.size = ldmaCtrlSizeHalf;
  }

  reading_samples_to_buffer = false;
  num_channels = n_channels;
  initialized = true;
  mic_running = false;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Read samples from the microphone into a buffer
 ******************************************************************************/
sl_status_t sl_mic_get_n_samples(void *buffer, uint32_t n_frames)
{
  if (!mic_running) {
    sl_mic_start();
  }

  // Wait until mic is ready
  while (!mic_ready) {
  }

  sample_buffer = (int16_t *)buffer;
  sample_count = n_frames;
  sample_index = 0;
  reading_samples_to_buffer = true;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    De-initialize the microphone
 ******************************************************************************/
sl_status_t sl_mic_deinit(void)
{
  /* Stop sampling */
  DMADRV_StopTransfer(dma_channel_id);

  // DE-initialize the PDM peripheral
  PDM_DeInit(PDM);

  GPIO_PinModeSet(MIC_PORT_CLK, MIC_PIN_CLK, gpioModeDisabled, 0);
  GPIO_PinModeSet(MIC_PORT_DATA, MIC_PIN_DATA, gpioModeDisabled, 0);

  /* Free resources */
  DMADRV_FreeChannel(dma_channel_id);

  initialized = false;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Starts the microphone
 ******************************************************************************/
sl_status_t sl_mic_start(void)
{
  if (!initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  if (mic_running) {
    return SL_STATUS_INVALID_STATE;
  }

  // Clear and start the PDM filter
  PDM_Clear(PDM);
  PDM_FifoFlush(PDM);
  PDM_Start(PDM);

  // Start DMA
  DMADRV_LdmaStartTransfer((int)dma_channel_id,
                           (void*)&dma_transfer_cfg,
                           (void*)&dma_descriptor[0],
                           dma_complete,
                           NULL);

  // Start microphone wake-up timer
  sl_sleeptimer_start_timer_ms(&mic_wake_up_timer, 15, timeout_callback, NULL, 0, 0);

  mic_running = true;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Stops the microphone
 ******************************************************************************/
sl_status_t sl_mic_stop(void)
{
  if (!mic_running) {
    return SL_STATUS_INVALID_STATE;
  }

  DMADRV_StopTransfer(dma_channel_id);

  // Stop the PDM filter
  PDM_Stop(PDM);

  while ((PDM->STATUS & PDM_STATUS_ACT) == 1U) {
    // Wait until PDM is no longer running
  }

  mic_running = false;
  mic_ready = false;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 *    Checks if the sample buffer is ready
 ******************************************************************************/
bool sl_mic_sample_buffer_ready(void)
{
  return (!reading_samples_to_buffer);
}

/***************************************************************************//**
 *    Calculates the dBSPL value for a channel from a sample buffer
 ******************************************************************************/
sl_status_t sl_mic_calculate_sound_level(float *sound_level, const int16_t *buffer, uint32_t n_frames, uint8_t channel)
{
  float sample;
  float power;
  float mean;
  uint32_t i;

  if (channel >= num_channels) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  // Calculate mean
  mean = 0.0f;
  for ( i = channel; i < (n_frames * num_channels); i += num_channels) {
    mean += (float) buffer[i];
  }
  mean = mean / (float) n_frames;

  // Calculate variance
  power = 0;
  for ( i = channel; i < (n_frames * num_channels); i += num_channels) {
    sample = (((float)buffer[i] - mean) / 32767.0f);
    power += sample * sample;
  }
  power = power / (float)n_frames;

  // Convert to dBSPL
  *sound_level = 10.0f * log10f(power) + 120;

  return SL_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *  DMADRV transfer completion callback function.
 *
 * @details
 *  The callback function is called when a transfer is complete.
 *
 * @param[in] channel
 *  The DMA channel number.
 *
 * @param[in] sequenceNo
 *  The number of times the callback was called. Useful on long chains of
 *  linked transfers or on endless ping-pong type transfers.
 *
 * @param[in] userParam
 *  Optional user parameter supplied on DMA invocation.
 *
 * @return
 *   When doing ping-pong transfers, return true to continue or false to
 *   stop transfers.
 ******************************************************************************/
static bool dma_complete(unsigned int channel,
                         unsigned int sequenceNo,
                         void *userParam)
{
  (void)channel;
  (void)userParam;

  LDMA_Descriptor_t *next_desc;

  if (reading_samples_to_buffer) {
    // Set up the next descriptor
    if (sequenceNo & 0x01) {
      next_desc = &dma_descriptor[0];
    } else {
      next_desc = &dma_descriptor[1];
    }

    if (sample_index < sample_count) {
      // There are samples to get, place them in sample buffer
      next_desc->xfer.dstInc = ldmaCtrlDstIncOne;
      next_desc->xfer.dstAddr = (uint32_t)&sample_buffer[sample_index];

      if ((sample_count - sample_index) > 2048) {
        // There are more than 2048 samples to get, set xferCnt to max
        next_desc->xfer.xferCnt = 2047;
        // Increase sample index
        sample_index += 2048;
      } else {
        // Set xferCnt to remaining number of samples
        next_desc->xfer.xferCnt = (sample_count - sample_index) - 1;
        sample_index += (sample_count - sample_index);
      }
    } else {
      next_desc->xfer.dstInc = ldmaCtrlDstIncNone;
      next_desc->xfer.xferCnt = 2047;
      next_desc->xfer.dstAddr = (uint32_t) &discard_buffer;
    }
    if ( (dma_descriptor[0].xfer.dstInc == ldmaCtrlDstIncNone)
         && (dma_descriptor[1].xfer.dstInc == ldmaCtrlDstIncNone) ) {
      // Sample buffer is complete if both descriptors have returned back to default
      // Stop PDM when sample buffer is complete
      reading_samples_to_buffer = false;
      sl_mic_stop();
    }
  }

  return true;
}
