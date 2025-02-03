#ifndef BSP_H_
#define BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"
#include "stm32u5xx_hal.h"

void SystemClock_Config(void);
void SystemPower_Config(void);
void MX_GPIO_Init(void);
void MX_GPDMA1_Init(void);
void MX_ICACHE_Init(void);
void MX_TIM2_Init(void);
void MX_TIM17_Init(void);
// void MX_USART1_UART_Init(void);
void MX_USB_OTG_HS_PCD_Init(void);

bool bspInit(void);

void Error_Handler(void);

void delay(uint32_t time_ms);
uint32_t millis(void);

void logPrintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_UTIL_H_ */