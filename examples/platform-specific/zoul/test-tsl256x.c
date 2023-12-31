/*
 * Copyright (c) 2015, Zolertia - http://www.zolertia.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
/*---------------------------------------------------------------------------*/
/**
 * \addtogroup zoul-examples
 * @{
 *
 * \defgroup zoul-tsl256x-test TSL256X light sensor test (TSL2561/TSL2563)
 *
 * Demonstrates the use of the TSL256X digital ambient light sensor
 * @{
 *
 * \file
 *  Test file for the external TSL256X light sensor
 *
 * \author
 *         Antonio Lignan <alinan@zolertia.com>
 *         Toni Lozano <tlozano@zolertia.com>
 */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include "contiki.h"
#include "dev/i2c.h"
#include "dev/leds.h"
#include "dev/tsl256x.h"
/*---------------------------------------------------------------------------*/
/* Default sensor's integration cycle is 402ms */
#define SENSOR_READ_INTERVAL (CLOCK_SECOND)
/*---------------------------------------------------------------------------*/
PROCESS(remote_tsl256x_process, "TSL256X test process");
AUTOSTART_PROCESSES(&remote_tsl256x_process);
/*---------------------------------------------------------------------------*/
static struct etimer et;
/*---------------------------------------------------------------------------*/
void
light_interrupt_callback(uint8_t value)
{
  printf("* Light sensor interrupt!\n");
  leds_toggle(LEDS_RED);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(remote_tsl256x_process, ev, data)
{
  PROCESS_BEGIN();
  static int light;

  /* Print the sensor used, the default is the TSL2561 (from Grove) */
  if(TSL256X_REF == TSL2561_SENSOR_REF) {
    printf("Light sensor test --> TSL2561\n");
  } else if(TSL256X_REF == TSL2563_SENSOR_REF) {
    printf("Light sensor test --> TSL2563\n");
  } else {
    printf("Unknown light sensor reference, aborting\n");
    PROCESS_EXIT();
  }

  /* Use Contiki's sensor macro to enable the sensor */
  SENSORS_ACTIVATE(tsl256x);

  /* Default integration time is 402ms with 1x gain, use the below call to
   * change the gain and timming, see tsl2563.h for more options
   */
  /* tsl256x.configure(TSL256X_TIMMING_CFG, TSL256X_G16X_402MS); */

  /* Register the interrupt handler */
  TSL256X_REGISTER_INT(light_interrupt_callback);

  /* Enable the interrupt source for values over the threshold.  The sensor
   * compares against the value of CH0, one way to find out the required
   * threshold for a given lux quantity is to enable the DEBUG flag and see
   * the CH0 value for a given measurement.  The other is to reverse the
   * calculations done in the calculate_lux() function.  The below value roughly
   * represents a 2500 lux threshold, same as pointing a flashlight directly
   */
  tsl256x.configure(TSL256X_INT_OVER, 0x15B8);

  /* And periodically poll the sensor */

  while(1) {
    etimer_set(&et, SENSOR_READ_INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    light = tsl256x.value(TSL256X_VAL_READ);
    if(light != TSL256X_ERROR) {
      printf("Light = %u\n", (uint16_t)light);
    } else {
      printf("Error, enable the DEBUG flag in the tsl256x driver for info, ");
      printf("or check if the sensor is properly connected\n");
      PROCESS_EXIT();
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */

