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
#ifdef USE_STM32F4_HOST_MSC

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

/*Application Variables*/
/* You can change the Wave file name as you need, but do not exceed 11 characters */
#define WAVE_NAME "0:audio_sample.wav"
#define REC_WAVE_NAME "0:rec.wav"

/* State Machine for the USBH_USR_ApplicationState */
#define USBH_USR_FS_INIT    ((uint8_t)0x00)
#define USBH_USR_AUDIO      ((uint8_t)0x01)

/* Defines for the Audio used commands */
#define CMD_PLAY           ((uint32_t)0x00)
#define CMD_RECORD         ((uint32_t)0x01)
#define CMD_STOP           ((uint32_t)0x02)

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */

#define REPEAT_ON        ((uint32_t)0x00) /* Replay Status in ON */
#define REPEAT_OFF       ((uint32_t)0x01) /* Replay Status in OFF */

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
void BSP_HostMSC_Init (void);
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

#endif /* USE_STM32F4_HOST_MSC */

#endif /* __STM32F4_DISCOVERY_MSC_H */

/****************************************END OF FILE******************************************/
