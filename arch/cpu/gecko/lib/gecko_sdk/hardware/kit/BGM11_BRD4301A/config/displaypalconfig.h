/***************************************************************************//**
 * @file
 * @brief Configuration file for PAL (Platform Abstraction Layer)
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef __SILICON_LABS_DISPLAY_PAL_CONFIG_H_
#define __SILICON_LABS_DISPLAY_PAL_CONFIG_H_

/*
 * Select which oscillator should source the RTC clock.
 */
#undef  PAL_RTCC_CLOCK_LFXO
#undef  PAL_RTCC_CLOCK_LFRCO
#define  PAL_RTCC_CLOCK_ULFRCO

/*
 * PAL SPI / USART configuration for the SLWSTK6100A_EFR32MG.
 * Select which USART and location is connected to the device via SPI.
 */
#define PAL_SPI_USART_UNIT     (USART1)
#define PAL_SPI_USART_CLOCK    (cmuClock_USART1)
#define PAL_SPI_USART_LOCATION_TX (11)
#define PAL_SPI_USART_LOCATION_SCLK (11)

/*
 * Specify the SPI baud rate:
 */
#define PAL_SPI_BAUDRATE       (1100000)

#endif /* __SILICON_LABS_DISPLAY_PAL_CONFIG_H_ */
