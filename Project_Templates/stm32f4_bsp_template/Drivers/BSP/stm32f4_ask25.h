/**
  ******************************************************************************
  * @file    stm32f4_ask25.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains definitions for STM32F4-ASK25 Kit's Leds and
  *          push-button hardware resources.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_ASK25_H
#define __STM32F4_ASK25_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_ask25_lcd.h"
#include "stm32f4_ask25_at24c16.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25
  * @{
  */

/** @addtogroup STM32F4_ASK25_LOW_LEVEL
  * @{
  */

/** @defgroup STM32F4_ASK25_LOW_LEVEL_Exported_Types
  * @{
  */

typedef enum
{
  ALED1 = 0x01,
  ALED2 = 0x02,
  ALED3 = 0x04,
  ALED4 = 0x08,
  ALED5 = 0x10,
  ALED6 = 0x20,
  ALED7 = 0x40,
  ALED8 = 0x80,
  ALEDALL= 0xFF
} ALED_TypeDef;

typedef enum
{
  ABUTTON_KEY1 = 0x10,
  ABUTTON_KEY2 = 0x20,
  ABUTTON_KEY3 = 0x40,
  ABUTTON_KEY4 = 0x80,
  ABUTTON_ALL= 0xFF
} AButton_TypeDef;

typedef enum
{
  ABUTTON_MODE_GPIO = 0,
  ABUTTON_MODE_EXTI = 1
} AButtonMode_TypeDef;
/**
  * @}
  */

/** @defgroup STM32F4_ASK25_LOW_LEVEL_Exported_Constants
  * @{
  */

/**
* @brief  Define for STM32F4_ASK25 board
*/
#if !defined (USE_STM32F4_ASK25)
 #define USE_STM32F4_ASK25
#endif


/** @addtogroup STM32F4_ASK25_LOW_LEVEL_BUTTON
  * @{
  */
#define USE_BUTTON_INT  DISABLE

/**
  * @}
  */

/** @addtogroup STM32F4_ASK25_LOW_LEVEL_RELAY
  * @{
  */
#define RELAY_PORT  GPIOB
#define RELAY_PIN   GPIO_PIN_0

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_LOW_LEVEL_Exported_Functions
  * @{
  */
/* LED Functions */
void ASK25_LED_Init(ALED_TypeDef LED);
void ASK25_LED_On(ALED_TypeDef LED);
void ASK25_LED_Off(ALED_TypeDef LED);
void ASK25_LED_Toggle(ALED_TypeDef LED);

/* Push Button Functions */
void ASK25_PB_Init(AButton_TypeDef Button, AButtonMode_TypeDef Mode);
uint32_t ASK25_PB_GetState(AButton_TypeDef Button);

/* Relay Functions */
void ASK25_Relay_Init(void);

/* DC Motor Functions */
void ASK25_DCMotor_Init(void);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_ASK25_H */

/****************************************END OF FILE******************************************/
