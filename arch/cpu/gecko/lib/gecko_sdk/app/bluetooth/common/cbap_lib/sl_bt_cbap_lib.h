/***************************************************************************//**
 * @file
 * @brief Certificate Based Authentication and Pairing Library header
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
#ifndef SL_BT_CBAP_LIB_H
#define SL_BT_CBAP_LIB_H

#include <stdbool.h>
#include "sl_status.h"

/**************************************************************************//**
 * Initialize the library.
 * Imports and validates the device with root certificate.
 *
 * @param[in] root_certificate_pem root certificate in PEM format.
 * @param[out] device_certificate_der device certificate in DER format.
 * @param[out] device_certificate_der_len device certificate length.
 *
 * @return SL_STATUS_OK if device certificate is validated, error code otherwise.
 *****************************************************************************/
sl_status_t sl_bt_cbap_lib_init(const char *root_certificate_pem,
                                uint8_t *device_certificate_der,
                                uint32_t *device_certificate_der_len);

/***************************************************************************//**
 * Parse and validate remote certificate and extract remote public key.
 *
 * @param[in] remote_certificate_der Certificate from remote device in DER.
 * @param[in] remote_certificate_der_len Length of the remote certificate.
 *
 * @return SL_STATUS_OK if remote certificate is verified, error code otherwise.
 ******************************************************************************/
sl_status_t sl_bt_cbap_lib_process_remote_cert(uint8_t *remote_certificate_der,
                                               uint32_t remote_certificate_der_len);

/***************************************************************************//**
 * Signs and combines OOB data.
 *
 * @param[in] device_random OOB data generated by the bt stack.
 * @param[in] device_confirm OOB data generated by the bt stack.
 * @param[out] output_data The signed OOB data
 * @param[out] output_len The signed OOB data length
 *
 * @return SL_STATUS_OK if OOB data signed, error code otherwise.
 ******************************************************************************/
sl_status_t sl_bt_cbap_lib_sign_device_oob_data(uint8_t *device_random,
                                                uint8_t *device_confirm,
                                                uint8_t *output_data,
                                                size_t *output_len);

/***************************************************************************//**
 * Verifies the remote device OOB data signature.
 *
 * @param[in] remote_random OOB data from remote device.
 * @param[in] remote_confirm OOB data from remote device.
 * @param[in] remote_oob_signature Remote OOB signature.
 *
 * @return SL_STATUS_OK if OOB data signature is OK, error code otherwise.
 ******************************************************************************/
sl_status_t sl_bt_cbap_lib_verify_remote_oob_data(uint8_t *remote_random,
                                                  uint8_t *remote_confirm,
                                                  uint8_t *remote_oob_signature);

#endif // SL_BT_CBAP_LIB_H
