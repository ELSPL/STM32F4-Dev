/**
  ******************************************************************************
  * @file    stm32f4_gsm.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    18-Feb-2016
  * @brief   Contains all function for GSM library on STM32F4
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_GSM_H
#define __STM32F4_GSM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "stm32f4_global.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_GSM
  * @{
  */

/** @defgroup STM32F4_GSM_Exported_Functions
  * @{
  */

void BSP_GSM_Cmd(uint8_t *Cmd);
void BSP_GSM_Read(uint8_t *dataBuf ,uint8_t length);
char BSP_GSM_ReadADV(void);
void BSP_GSM_ReadADV2(uint8_t *dataBuf ,uint16_t length);
char BSP_GSM_ConnectTest(void);
char BSP_GSM_Call(uint8_t *num);
char BSP_GSM_MissedCall(void);
void BSP_GSM_ScrollPB(uint8_t *dataBuf);
void BSP_GSM_WritePB(uint8_t *location,uint8_t *number,uint8_t *name);
void BSP_GSM_FindPB(uint8_t *name,uint8_t *nameBuf);
char BSP_GSM_TMODE(void);
char BSP_GSM_SendSMS(uint8_t *number,uint8_t *message);
void BSP_GSM_ReadSMS(uint8_t *location,uint8_t *message);


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

#endif /* __STM32F4_GSM_H */

 /****************************************END OF FILE******************************************/
