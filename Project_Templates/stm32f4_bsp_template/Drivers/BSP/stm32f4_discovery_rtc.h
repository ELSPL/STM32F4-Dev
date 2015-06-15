/******************************************************************//**
* @file   stm32f4_discovery_rtc.h
* @brief  This file contains declaration for stm32f4_discovery_rtc.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_RTC_H
#define __STM32F4_DISCOVERY_RTC_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_uart.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_RTC
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_RTC_Public_Types
 * @{
 */

extern RTC_HandleTypeDef hrtc_bsp;
extern uint16_t Year[1];

typedef enum
{
  AlarmA,
  AlarmB,
  BothAlarm
}Alarm_Typedef;


/**
 * @} STM32F4_DISCOVERY_RTC_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_RTC_Public_Functions
 * @{
 */

void BSP_RTC_Init(void);
void BSP_RTC_MspDeInit(RTC_HandleTypeDef* hrtc);
uint8_t BSP_Change_Time (void);
uint8_t BSP_Change_Date (void);
void BSP_RTC_WakeUpTimer_Init(uint16_t WakeUptime);
void BSP_RTC_Alarm_Init(Alarm_Typedef sel_Alarm, uint8_t hour, uint8_t minute, uint8_t second, uint8_t dayselect);
/**
 * @} STM32F4_DISCOVERY_RTC_Public_Functions End
 */





#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_RTC_H_ */

/**
 * @} STM32F4_DISCOVERY_RTC End
 */

/* --------------------------------- End Of File ------------------------------ */
