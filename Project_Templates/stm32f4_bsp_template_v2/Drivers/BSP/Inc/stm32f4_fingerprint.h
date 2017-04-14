/**
  ******************************************************************************
  * @file    stm32f4_fingerprint.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    9-May-2016
  * @brief   Contains all function for fingerprint library on STM32F4
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_FINGERPRINT_H
#define __STM32F4_FINGERPRINT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "stm32f4_global.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_FINGERPRINT
  * @{
  */

/** @defgroup STM32F4_FINGERPRINT_Exported_Functions
  * @{
  */

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_FINGERPRINT_Public_Types
* @{
*/
typedef enum
{
   RESULT_SUCCEEDED=0x01,
   RESULT_FAILED=0x02,
   RESULT_NOT_MASTER_MODE=0x03,
   RESULT_USED_ID=0x04,
   RESULT_INVALID_ID=0x05,
   RESULT_DB_IS_FULL=0x06,
   RESULT_NOT_IN_TIME=0x07,
   RESULT_INVALID_PARAM=0x09,
   RESULT_CANCELED=0x0D,
   RESULT_ANOTHER_FINGER=0x0E,
   RESULT_IDENTIFY_TIMEOUT=0x12,
   RESULT_INVALID_DATASIZE=0x15,
   RESULT_INVALID_DATA=0x16,
   RESULT_EXTRACT_FAIL=0x17
}Result_typedef;

/**
 * @} STM32F4_FINGERPRINT_Public_Types End
 */

void BSP_FP_Cmd(uint8_t *Cmd, uint8_t length);
void BSP_FP_Read(uint8_t *dataBuf ,uint16_t length);
void ResultError(Result_typedef Resul);
uint8_t BSP_FP_Connect(void);
uint8_t BSP_FP_MasterMode(void);
uint8_t BSP_FP_Enroll_step1(uint8_t *id);
uint8_t BSP_FP_Enroll_step2(void);
uint8_t BSP_FP_Verify(uint8_t *id);
uint8_t BSP_FP_Identify(uint8_t *id);
uint8_t BSP_FP_Delete(uint8_t *id);
uint8_t BSP_FP_LeaveMaster_Mode(void);
uint8_t BSP_FP_DeleteAll(void);
uint8_t BSP_FP_List(uint8_t *database, uint8_t *count);

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

#endif /* __STM32F4_FINGERPRINT_H */

 /****************************************END OF FILE******************************************/
