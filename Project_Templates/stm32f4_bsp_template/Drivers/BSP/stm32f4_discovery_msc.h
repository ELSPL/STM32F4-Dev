/**
  ******************************************************************************
  * @file    stm32f4_discovery_msc.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-June-2015
  * @brief   This file contains definitions and support for Host Mass Storage
  *          (MSC) drivers
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_MSC_H
#define __STM32F4_DISCOVERY_MSC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_MSC
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_MSC_Exported_Handle
 * @{
 */

typedef enum {
 APPLICATION_IDLE = 0,
 APPLICATION_START,
 APPLICATION_READY,
 APPLICATION_DISCONNECT,
}ApplicationTypeDef;

extern USBH_HandleTypeDef hUsbHostMSC;
extern ApplicationTypeDef Appli_state;

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_MSC_Exported_Constants
  * @{
  */


/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_MSC_Exported_Functions
  * @{
  */

/* MSC Initialization function */
void BSP_MSCHost_Init (void);
void BSP_USB_HOST_Process(void);

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

#endif /* __STM32F4_DISCOVERY_MSC_H */

/****************************************END OF FILE******************************************/
