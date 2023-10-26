/***************************************************************************//**
 * @file
 * @brief Silicon Labs PSA Crypto Opaque Driver functions for CRYPTOACC.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef SLI_CRYPTOACC_OPAQUE_FUNCTIONS_H
#define SLI_CRYPTOACC_OPAQUE_FUNCTIONS_H

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

/***************************************************************************//**
 * \addtogroup sl_psa_drivers
 * \{
 ******************************************************************************/

/***************************************************************************//**
 * \addtogroup sl_psa_drivers_cryptoacc CRYPTOACC opaque PSA driver
 * \brief Driver plugin for Silicon Labs CRYPTOACC peripheral adhering to the
 *        PSA opaque accelerator specification.
 * \{
 ******************************************************************************/

#include "em_device.h"

#if defined(CRYPTOACC_PRESENT) && defined(SEPUF_PRESENT)

#include "sli_cryptoacc_opaque_types.h"
// Replace inclusion of crypto_driver_common.h with the new psa driver interface
// header file when it becomes available.
#include "psa/crypto_driver_common.h"

/* NOTE: This header file will be autogenerated by PSA Crypto build system based on
 * the definitions in sli_cryptoacc_opaque_driver.json. However, until such a system is
 * in place, we rely on manually writing the file */

#ifdef __cplusplus
extern "C" {
#endif

psa_status_t sli_cryptoacc_opaque_mac_compute(const psa_key_attributes_t *attributes,
                                              const uint8_t *key_buffer,
                                              size_t key_buffer_size,
                                              psa_algorithm_t alg,
                                              const uint8_t *input,
                                              size_t input_length,
                                              uint8_t *mac,
                                              size_t mac_size,
                                              size_t *mac_length);

psa_status_t sli_cryptoacc_opaque_get_builtin_key(psa_drv_slot_number_t slot_number,
                                                  psa_key_attributes_t *attributes,
                                                  uint8_t *key_buffer,
                                                  size_t key_buffer_size,
                                                  size_t *key_buffer_length);

#ifdef __cplusplus
}
#endif

#endif // CRYPTOACC_PRESENT && SEPUF_PRESENT

/** \} (end addtogroup sl_psa_drivers_cryptoacc) */
/** \} (end addtogroup sl_psa_drivers) */

/// @endcond

#endif // SLI_CRYPTOACC_OPAQUE_FUNCTIONS_H