/*
 * def.h
 *
 *  Created on: Oct 30, 2023
 *      Author: alren
 */

#ifndef INCLUDE_DEF_H_
#define INCLUDE_DEF_H_

#include <stdbool.h>
#include <stdint.h>

#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "util.h"

#include "bsp.h"

#define _DEF_FIRMWATRE_VERSION    "V231121R1"
#define _DEF_BOARD_NAME           "STM32U5A5JTXQ"

#define TEMP_AVG_SLOPE 0.0043
#define TEMP_V25       1.43
#define TEMP_VSENSE    3.3/4096


#define _USE_VIA_SHOW   //show via data

#define _USE_HW_MICROS
//#define _USE_WSDG_
#define _USE_DBG_

#define _USE_HW_LED
#define      HW_LED_MAX_CH          3

#define _USE_HW_UART
#define      HW_UART_MAX_CH         2
#define      HW_UART_CH_SWD         _DEF_UART1
#define      HW_UART_CH_USB         _DEF_UART2
#define      HW_UART_CH_CLI         HW_UART_CH_SWD

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       2

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    32
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_CLI_GUI
#define      HW_CLI_GUI_WIDTH       80
#define      HW_CLI_GUI_HEIGHT      24

#define _USE_HW_LOG
#define      HW_LOG_CH              HW_UART_CH_SWD
#define      HW_LOG_BOOT_BUF_MAX    2048
#define      HW_LOG_LIST_BUF_MAX    4096

#define _USE_HW_USB
#define _USE_HW_CDC
#define      HW_USB_LOG             1
#define      HW_USB_CMP             1
#define      HW_USB_CDC             1
#define      HW_USB_MSC             0
#define      HW_USB_HID             1

#define _USE_HW_KEYSCAN
#define      HW_KEYSCAN_MAX_CH      HW_BUTTON_MAX_CH
#define      HW_KEYSCAN_PRESS_MAX   6

//-- CLI
//
#define _USE_CLI_HW_BUTTON          1
#define _USE_CLI_HW_KEYSCAN         1

#define _DEF_CH1              0
#define _DEF_CH2              1
#define _DEF_CH3              2
#define _DEF_CH4              3

#define _DEF_LED1             0
#define _DEF_LED2             1
#define _DEF_LED3             2
#define _DEF_LED4             3

#define _DEF_UART1            0
#define _DEF_UART2            1
#define _DEF_UART3            2
#define _DEF_UART4            3

#define _DEF_BUTTON1          0
#define _DEF_BUTTON2          1
#define _DEF_BUTTON3          2
#define _DEF_BUTTON4          3

#define _DEF_SPI1             0
#define _DEF_SPI2             1
#define _DEF_SPI3             2
#define _DEF_SPI4             3

#define _DEF_I2C1             0
#define _DEF_I2C2             1
#define _DEF_I2C3             2
#define _DEF_I2C4             3

#define _DEF_I2S1             0
#define _DEF_I2S2             1
#define _DEF_I2S3             2
#define _DEF_I2S4             3

#define _DEF_PWM1             0
#define _DEF_PWM2             1
#define _DEF_PWM3             2
#define _DEF_PWM4             3

#define _DEF_LOW              0
#define _DEF_HIGH             1

#define _DEF_INPUT            (1<<0)
#define _DEF_OUTPUT           (1<<1)
#define _DEF_PULLUP           (1<<2)
#define _DEF_PULLDOWN         (1<<3)
#define _DEF_INPUT_PULLUP     (_DEF_INPUT  | _DEF_PULLUP)
#define _DEF_INPUT_PULLDOWN   (_DEF_INPUT  | _DEF_PULLDOWN)
#define _DEF_OUTPUT_PULLUP    (_DEF_OUTPUT | _DEF_PULLUP)
#define _DEF_OUTPUT_PULLDOWN  (_DEF_OUTPUT | _DEF_PULLDOWN)

#define _DEF_CAN1             0
#define _DEF_CAN2             1
#define _DEF_CAN3             2
#define _DEF_CAN4             3

#define _DEF_CAN_100K         0
#define _DEF_CAN_125K         1
#define _DEF_CAN_250K         2
#define _DEF_CAN_500K         3
#define _DEF_CAN_1000K        4

#define _DEF_CAN_STD          0
#define _DEF_CAN_EXT          1

#define _DEF_DXL1             0
#define _DEF_DXL2             1
#define _DEF_DXL3             2
#define _DEF_DXL4             3

#define constrain(amt,low,high) ((int)(amt)<(low)?(low):((amt)>(high)?(high):(amt)))

//#define _USE_HW_EEPROM
#define      HW_EEPROM_MAX_SIZE     (1*1024)

//#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1
#define      HW_I2C_CH_EEPROM       _DEF_I2C1



#define VBUS_SENSE_Pin GPIO_PIN_2
#define VBUS_SENSE_GPIO_Port GPIOC
#define UCPD_FLT_Pin GPIO_PIN_14
#define UCPD_FLT_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_2
#define LED_RED_GPIO_Port GPIOG
#define LED_GREEN_Pin GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOC
#define T_VCP_TX_Pin GPIO_PIN_9
#define T_VCP_TX_GPIO_Port GPIOA
#define T_VCP_RX_Pin GPIO_PIN_10
#define T_VCP_RX_GPIO_Port GPIOA
#define UCPD_DBn_Pin GPIO_PIN_5
#define UCPD_DBn_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_7
#define LED_BLUE_GPIO_Port GPIOB

#endif /* INCLUDE_DEF_H_ */
