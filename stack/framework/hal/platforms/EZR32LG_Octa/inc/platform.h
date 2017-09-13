/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2015 University of Antwerp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include "platform_defs.h"
#include "mcu.h"

#ifndef PLATFORM_EZR32LG_OCTA
    #error Mismatch between the configured platform and the actual platform. Expected PLATFORM_EZR32LG_USB01 to be defined
#endif


#include <ezr32lg_chip.h>

#define HW_USE_HFXO
#define HW_USE_LFXO

/********************
 * LED DEFINITIONS *
 *******************/

#define HW_NUM_LEDS 3
#define LED0	PIN(1, 4)
#define	LED1	PIN(1, 5)
#define LED2	PIN(1, 6)
#define LED_GREEN 2
#define LED_ORANGE 1
#define LED_RED 0


/********************
 *  USB SUPPORT      *
 ********************/

#define USB_DEVICE

/********************
 * UART DEFINITIONS *
 *******************/

// console configuration
#define CONSOLE_UART        PLATFORM_EZR32LG_OCTA_CONSOLE_UART
#define CONSOLE_LOCATION    PLATFORM_EZR32LG_OCTA_CONSOLE_LOCATION
#define CONSOLE_BAUDRATE    PLATFORM_EZR32LG_OCTA_CONSOLE_BAUDRATE


#define si4455_GDO0_PIN A15
#define si4455_GDO1_PIN E14
#define si4455_SDN_PIN  E8

#ifdef USE_SX127X
  #define SX127x_SPI_INDEX    1
  #define SX127x_SPI_LOCATION 1
  #define SX127x_SPI_PIN_CS   D3
  #define SX127x_SPI_BAUDRATE 10000000
  #define SX127x_DIO0_PIN E3
  #define SX127x_DIO1_PIN E2
  #ifdef PLATFORM_SX127X_USE_DIO3_PIN
    #define SX127x_DIO3_PIN E0
  #endif
  #ifdef PLATFORM_SX127X_USE_RESET_PIN
    #define SX127x_RESET_PIN A13
  #endif
#endif

/*************************
 * DEBUG PIN DEFINITIONS *
 ************************/

#define DEBUG_PIN_NUM 0


/**************************
 * USERBUTTON DEFINITIONS *
 *************************/

#define NUM_USERBUTTONS 	2
#define BUTTON0				PIN(5, 4)
#define BUTTON1				PIN(0, 0)


#endif
