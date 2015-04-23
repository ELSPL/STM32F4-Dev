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

extern uint8_t temp;

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

typedef enum
{
  ASEGMENT1 = 1,
  ASEGMENT2 = 2
} A7Segment_typedef;

typedef enum
{
  StMotorClockwise = 0,
  StMotorAntiClockwise
}StMotorDirection_Typedef;
/******************************************************************************/
/*                         Seven Segment Specification                        */
/******************************************************************************/

#define   a     0x01
#define   b     0x02
#define   c     0x04
#define   d     0x08
#define   e     0x10
#define   f     0x20
#define   g     0x40
#define   dot   0x80
#define   all   0xFF

/******************************************************************************/
/*              Seven Segment Display Character Specifications                */
/******************************************************************************/

#define   ZERO    a+b+c+d+e+f
#define   ONE     b+c
#define   TWO     a+b+d+e+g
#define   THREE   a+b+c+d+g
#define   FOUR    b+c+f+g
#define   FIVE    a+c+d+f+g
#define   SIX     a+c+d+e+f+g
#define   SEVEN   a+b+c
#define   EIGHT   a+b+c+d+e+f+g
#define   NINE    a+b+c+d+f+g

#define   TOTAL_SEGMENTS      2     // Define number of Segments to be connected maximum up to 4
#define   COMMON_ANODE_SEG    ON   // Define type of Segmnent connections
#define   COMMON_CATHODE_SEG  OFF

/******************************************************************************/
/*                           MAT_KB Selection                                 */
/******************************************************************************/
#define   MATKB_2X2     ENABLE
#define   MATKB_3X3     DISABLE
#define   MATKB_4X4     DISABLE
#define   MATKB_5X5     DISABLE
#define   MATKB_6X6     DISABLE
#define   MATKB_7X7     DISABLE
#define   MATKB_8X8     DISABLE

/******************************************************************************/
/*                           MAT_KB Selection                                 */
/******************************************************************************/
#if MATKB_2X2
  #define ROW   2
  #undef ROW3
  #undef ROW4
  #undef ROW5
  #undef ROW6
  #undef ROW7
  #undef ROW8

  #undef COL3
  #undef COL4
  #undef COL5
  #undef COL6
  #undef COL7
  #undef COL8
#endif

/******************************************************************************/
/*                      Stepper Motor Selection                               */
/******************************************************************************/
#define GENERAL_SM_SEL           DISABLE
#define STM_601_SEL              ENABLE

/******************************************************************************/
/*                      Stepper Motor Calibration                             */
/******************************************************************************/

#define   CAL_ANGLE     7.2     // Stepper motor calibration angle

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

/** @addtogroup STM32F4_ASK25_Stepper_Motor
  * @{
  */
#if GENERAL_SM_SEL && STM_601_SEL
  #error Stepper Motor is not properly selected
#endif

#if STM_601_SEL
  #define STM_601
  #define   CAL_ANGLE     18     // Stepper motor calibration angle
#endif

#if GENERAL_SM_SEL
  #define GENERAL_SM
  #define   CAL_ANGLE     72     // Stepper motor calibration angle
#endif
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
void ASK25_DCMotor_Clk(void);
void ASK25_DCMotor_AntiClk(void);

/* 7 Segment function */
void ASK25_7Segment_Init(void);
void ASK25_7Segment_Display_Digit (uint8_t Digit, uint8_t Seg);
void ASK25_7Segment_Display_Data (uint8_t Number);

/* MAT-KEY Functions */
void ASK25_MatKey_Init (void);
uint8_t ASK25_MatKey_Detect_Key(void);

/* Stepper Motor Functions */
void ASK25_SM_Init(void);
void ASK25_SM_Rotate (StMotorDirection_Typedef StMotorDirection, uint8_t Angle, uint8_t Delay);
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
