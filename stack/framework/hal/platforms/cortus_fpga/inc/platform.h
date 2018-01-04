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

#ifndef PLATFORM_CORTUS_FPGA
    #error Mismatch between the configured platform and the actual platform. Expected PLATFORM_CORTUS_FPGA to be defined
#endif

//#define HW_USE_HFXO
//#define HW_USE_LFXO

#include "cortus_mcu.h"
#include "machine/gpio.h"

/********************
 * LED DEFINITIONS *
 *******************/

#define HW_NUM_LEDS 8


//INT_HANDLER

/********************
 *  USB SUPPORT      *
 ********************/

//#define USB_DEVICE

/********************
 * UART DEFINITIONS *
 *******************/

// console configuration
#define CONSOLE_UART        0
#define CONSOLE_LOCATION    1
#define CONSOLE_BAUDRATE    115200

/*************************
 * DEBUG PIN DEFINITIONS *
 ************************/

//#define DEBUG_PIN_NUM 4
//#define DEBUG0	D4
//#define DEBUG1	D5
//#define DEBUG2	D6
//#define DEBUG3	D7

/**************************
 * USERBUTTON DEFINITIONS *
 *************************/

#define NUM_USERBUTTONS 	2
#define BUTTON0				PIN(gpioPortA, 4)
#define BUTTON1				PIN(gpioPortA, 5)

// CC1101 PIN definitions
#ifdef USE_CC1101
#define CC1101_SPI_USART    1  // not used
#define CC1101_SPI_BAUDRATE 8  // divider
#define CC1101_SPI_PIN_CS   PIN(gpioPortA, 2)
#define CC1101_GDO0_PIN     PIN(gpioPortA, 0)
#define CC1101_GDO2_PIN     PIN(gpioPortA, 1)
#endif

#ifdef USE_SX127X
#define SX127x_SPI_INDEX    1
#define SX127x_SPI_PIN_CS   PIN(gpioPortA, 2)
#define SX127x_SPI_BAUDRATE 8 //10000000
#define SX127x_DIO0_PIN     PIN(gpioPortA, 0)
#define SX127x_DIO1_PIN     PIN(gpioPortA, 1)
#endif

#endif
