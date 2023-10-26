/***************************************************************************//**
 * @file
 * @brief CMSIS Compatible WGM160 startup file in C.
 *        Should be used with GCC 'GNU Tools ARM Embedded'
 *******************************************************************************
 * # License
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is Third Party Software licensed by Silicon Labs from a third party
 * and is governed by the sections of the MSLA applicable to Third Party
 * Software and the additional terms set forth below.
 *
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include "em_device.h"        /* The correct device header file. */

#ifdef BOOTLOADER_ENABLE
#include "api/btl_interface.h"

#endif

#ifdef SL_APP_PROPERTIES
#include "api/application_properties.h"

#endif

#define TOTAL_INTERRUPTS    (16 + EXT_IRQ_COUNT)

#ifdef BOOTLOADER_ENABLE
extern MainBootloaderTable_t mainStageTable;

extern void SystemInit2(void);

/*----------------------------------------------------------------------------
 * Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void (*VECTOR_TABLE_Type)(void);
#endif

#ifdef SL_APP_PROPERTIES
extern ApplicationProperties_t sl_app_properties;

/*----------------------------------------------------------------------------
 * Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void (*VECTOR_TABLE_Type)(void);
#endif

/*---------------------------------------------------------------------------
 * External References
 *---------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;

extern __NO_RETURN void __PROGRAM_START(void);

#if defined (__START) && defined (__GNUC__)
extern int  __START(void) __attribute__((noreturn));    /* main entry point */
void Zero_Table();
void Copy_Table();
#endif /* __START */

/*---------------------------------------------------------------------------
 * Internal References
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void);
void Default_Handler(void);

#if defined (__GNUC__)
#ifndef __STACK_SIZE
#define __STACK_SIZE    0x00000400
#endif /* __STACK_SIZE */

#ifndef __HEAP_SIZE
#define __HEAP_SIZE    0x00000C00
#endif /* __HEAP_SIZE */
#endif /* __GNUC__ */

/*----------------------------------------------------------------------------
 * Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M Processor Exceptions */
void NMI_Handler(void)               __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)         __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler(void)         __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)        __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler(void)               __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)            __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)           __attribute__ ((weak, alias("Default_Handler")));
#ifndef SL_APP_PROPERTIES
/* Provide a dummy value for the sl_app_properties symbol. */
void sl_app_properties(void);        /* Prototype to please MISRA checkers. */
void sl_app_properties(void)         __attribute__ ((weak, alias("Default_Handler")));
#endif

/* Part Specific Interrupts */

void EMU_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void WDOG0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void LDMA_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void GPIO_EVEN_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void SMU_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void TIMER0_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void USART0_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART0_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void ACMP0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void IDAC0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void GPIO_ODD_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void TIMER2_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void TIMER3_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void USART1_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART1_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART2_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART2_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void UART0_RX_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void UART0_TX_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void UART1_RX_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void UART1_TX_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void LEUART0_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void LEUART1_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void LETIMER0_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void PCNT0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void PCNT1_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void PCNT2_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void RTCC_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void CMU_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void MSC_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void CRYPTO0_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void CRYOTIMER_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void FPUEH_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void USART3_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART3_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART4_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART4_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void WTIMER0_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void WTIMER1_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void WTIMER2_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void WTIMER3_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void I2C2_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void VDAC0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void TIMER4_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void TIMER5_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void TIMER6_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void USART5_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void USART5_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void CSEN_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void LESENSE_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void EBI_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void ACMP2_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void ADC1_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void LCD_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void SDIO_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void ETH_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void CAN0_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void CAN1_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void USB_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void WDOG1_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void LETIMER1_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void TRNG0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void QSPI0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void YKGW_FF_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void YKGW_SMART_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
 * Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#if defined (__ICCARM__)
#pragma data_alignment=512
extern const tVectorEntry __VECTOR_TABLE[TOTAL_INTERRUPTS];
const tVectorEntry        __VECTOR_TABLE[TOTAL_INTERRUPTS] __VECTOR_TABLE_ATTRIBUTE = {
#elif defined(__GNUC__)
extern const tVectorEntry __VECTOR_TABLE[TOTAL_INTERRUPTS];
const tVectorEntry __VECTOR_TABLE[TOTAL_INTERRUPTS] __attribute__((aligned(512)))
__VECTOR_TABLE_ATTRIBUTE = {
#else
extern const tVectorEntry __VECTOR_TABLE[TOTAL_INTERRUPTS];
const tVectorEntry __VECTOR_TABLE[TOTAL_INTERRUPTS] __VECTOR_TABLE_ATTRIBUTE = {
#endif
  /* Cortex-M Exception Handlers */
  { .topOfStack = &__INITIAL_SP },            /* Initial Stack Pointer */
  { Reset_Handler             },              /* Reset Handler */
  { NMI_Handler               },              /* NMI Handler */
  { HardFault_Handler         },              /* Hard Fault Handler */
  { MemManage_Handler         },              /* MPU Fault Handler */
  { BusFault_Handler          },              /* Bus Fault Handler */
  { UsageFault_Handler        },              /* Usage Fault Handler */
  { Default_Handler           },              /* Reserved */
  { Default_Handler           },              /* Reserved */
  { Default_Handler           },              /* Reserved */
#ifdef BOOTLOADER_ENABLE
  { (VECTOR_TABLE_Type) & mainStageTable },
#else
  { Default_Handler           },              /* Reserved */
#endif
  { SVC_Handler               },              /* SVCall Handler */
  { DebugMon_Handler          },              /* Debug Monitor Handler */
#ifdef SL_APP_PROPERTIES
  { (VECTOR_TABLE_Type) & sl_app_properties }, /* Application properties*/
#else
  { sl_app_properties },                       /* Application properties*/
#endif
  { PendSV_Handler            },              /* PendSV Handler */
  { SysTick_Handler           },              /* SysTick Handler */

  /* External interrupts */

  { EMU_IRQHandler            },              /* 0 */
  { WDOG0_IRQHandler          },              /* 1 */
  { LDMA_IRQHandler           },              /* 2 */
  { GPIO_EVEN_IRQHandler      },              /* 3 */
  { SMU_IRQHandler            },              /* 4 */
  { TIMER0_IRQHandler         },              /* 5 */
  { USART0_RX_IRQHandler      },              /* 6 */
  { USART0_TX_IRQHandler      },              /* 7 */
  { ACMP0_IRQHandler          },              /* 8 */
  { ADC0_IRQHandler           },              /* 9 */
  { IDAC0_IRQHandler          },              /* 10 */
  { I2C0_IRQHandler           },              /* 11 */
  { I2C1_IRQHandler           },              /* 12 */
  { GPIO_ODD_IRQHandler       },              /* 13 */
  { TIMER1_IRQHandler         },              /* 14 */
  { TIMER2_IRQHandler         },              /* 15 */
  { TIMER3_IRQHandler         },              /* 16 */
  { USART1_RX_IRQHandler      },              /* 17 */
  { USART1_TX_IRQHandler      },              /* 18 */
  { USART2_RX_IRQHandler      },              /* 19 */
  { USART2_TX_IRQHandler      },              /* 20 */
  { UART0_RX_IRQHandler       },              /* 21 */
  { UART0_TX_IRQHandler       },              /* 22 */
  { UART1_RX_IRQHandler       },              /* 23 */
  { UART1_TX_IRQHandler       },              /* 24 */
  { LEUART0_IRQHandler        },              /* 25 */
  { LEUART1_IRQHandler        },              /* 26 */
  { LETIMER0_IRQHandler       },              /* 27 */
  { PCNT0_IRQHandler          },              /* 28 */
  { PCNT1_IRQHandler          },              /* 29 */
  { PCNT2_IRQHandler          },              /* 30 */
  { RTCC_IRQHandler           },              /* 31 */
  { CMU_IRQHandler            },              /* 32 */
  { MSC_IRQHandler            },              /* 33 */
  { CRYPTO0_IRQHandler        },              /* 34 */
  { CRYOTIMER_IRQHandler      },              /* 35 */
  { FPUEH_IRQHandler          },              /* 36 */
  { USART3_RX_IRQHandler      },              /* 37 */
  { USART3_TX_IRQHandler      },              /* 38 */
  { USART4_RX_IRQHandler      },              /* 39 */
  { USART4_TX_IRQHandler      },              /* 40 */
  { WTIMER0_IRQHandler        },              /* 41 */
  { WTIMER1_IRQHandler        },              /* 42 */
  { WTIMER2_IRQHandler        },              /* 43 */
  { WTIMER3_IRQHandler        },              /* 44 */
  { I2C2_IRQHandler           },              /* 45 */
  { VDAC0_IRQHandler          },              /* 46 */
  { TIMER4_IRQHandler         },              /* 47 */
  { TIMER5_IRQHandler         },              /* 48 */
  { TIMER6_IRQHandler         },              /* 49 */
  { USART5_RX_IRQHandler      },              /* 50 */
  { USART5_TX_IRQHandler      },              /* 51 */
  { CSEN_IRQHandler           },              /* 52 */
  { LESENSE_IRQHandler        },              /* 53 */
  { EBI_IRQHandler            },              /* 54 */
  { ACMP2_IRQHandler          },              /* 55 */
  { ADC1_IRQHandler           },              /* 56 */
  { LCD_IRQHandler            },              /* 57 */
  { SDIO_IRQHandler           },              /* 58 */
  { ETH_IRQHandler            },              /* 59 */
  { CAN0_IRQHandler           },              /* 60 */
  { CAN1_IRQHandler           },              /* 61 */
  { USB_IRQHandler            },              /* 62 */
  { RTC_IRQHandler            },              /* 63 */
  { WDOG1_IRQHandler          },              /* 64 */
  { LETIMER1_IRQHandler       },              /* 65 */
  { TRNG0_IRQHandler          },              /* 66 */
  { QSPI0_IRQHandler          },              /* 67 */
  { YKGW_FF_IRQHandler        },              /* 68 */
  { YKGW_SMART_IRQHandler     },              /* 69 */
};

#if defined (__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined (__START) && defined (__GNUC__)
void Copy_Table()
{
  uint32_t        *pSrc, *pDest;
  extern uint32_t __etext;
  extern uint32_t __data_start__;
  extern uint32_t __data_end__;
  pSrc  = &__etext;
  pDest = &__data_start__;

  for (; pDest < &__data_end__; ) {
    *pDest++ = *pSrc++;
  }
}

void Zero_Table()
{
  uint32_t        *pDest;
  extern uint32_t __bss_start__;
  extern uint32_t __bss_end__;
  pDest = &__bss_start__;

  for (; pDest < &__bss_end__; ) {
    *pDest++ = 0UL;
  }
}
#endif /* __START */

/*---------------------------------------------------------------------------
 * Reset Handler called on controller reset
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
#ifndef __NO_SYSTEM_INIT
  SystemInit();                    /* CMSIS System Initialization */
#endif

#ifdef BOOTLOADER_ENABLE
  SystemInit2();
#endif /* BOOTLOADER_ENABLE */

#if defined (__GNUC__) && defined (__START)
  Copy_Table();
  Zero_Table();
  __START();
#else
  __PROGRAM_START();               /* Enter PreMain (C library entry point) */
#endif /* __GNUC__ */
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
 * Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
  while (true) {
  }
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif
