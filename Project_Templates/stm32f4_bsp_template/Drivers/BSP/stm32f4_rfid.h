/**
  ******************************************************************************
  * @file    stm32f4_rfid.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    12-May-2016
  * @brief   Contains all function for RFID library on STM32F4
  ******************************************************************************
  */

#ifndef __STM32F4_RFID_H
#define __STM32F4_RFID_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "stm32f4_global.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_RFID_Public_Types
 * @{
 */

extern uint8_t a[3][10];
extern uint8_t b[3][13];

/**
  * @} STM32F4_RFID_Public_Types End
  */

/** @defgroup STM32F4_RFID_Exported_Functions
  * @{
  */

void BSP_RFID_Read(uint8_t *RxBuf);
void BSP_RFID_Get_Number(uint8_t *RxBuf,uint8_t *RFIDNum);
uint8_t BSP_Match_Card(uint8_t *RFIDNum);

/**
 * @} STM32F4_RFID_Exported_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_RFID_H */

 /****************************************END OF FILE******************************************/
