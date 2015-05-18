/**
  ******************************************************************************
  * @file    stm32f4_discovery_camera.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    16-May-2015
  * @brief   This file contains all the functions prototypes for the
  *          stm32f4_discovery_camera.c firmware driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_CAMERA_H
#define __STM32F4_DISCOVERY_CAMERA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

/* Include Camera component drivers */
#include "../Components/ov9655/ov9655.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_CAMERA
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Exported_Types
  * @{
  */

extern DCMI_HandleTypeDef hdcmi_camera;
extern DMA_HandleTypeDef hdma_dcmi;

typedef enum
{
CAMERA_OK       = 0x00,
CAMERA_ERROR    = 0x01,
CAMERA_TIMEOUT  = 0x02
}Camera_StatusTypeDef;

#define RESOLUTION_R160x120      CAMERA_R160x120  /* QQVGA Resolution */
#define RESOLUTION_R320x240      CAMERA_R320x240  /* QVGA Resolution */

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Exported_Constants
  * @{
  */
#define CAMERA_I2C_ADDRESS 0x60
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Exported_Functions
  * @{
  */
uint8_t BSP_CAMERA_Init(uint32_t Resolution);
void    BSP_DCMI_MspDeInit(DCMI_HandleTypeDef* hdcmi);
void    BSP_CAMERA_ContinuousStart(uint8_t *buff);
void    BSP_CAMERA_SnapshotStart(uint8_t *buff);
void    BSP_CAMERA_Suspend(void);
void    BSP_CAMERA_Resume(void);
uint8_t BSP_CAMERA_Stop(void);
//void    BSP_CAMERA_LineEventCallback(void);
//void    BSP_CAMERA_VsyncEventCallback(void);
//void    BSP_CAMERA_FrameEventCallback(void);
//void    BSP_CAMERA_ErrorCallback(void);

///* Camera features functions prototype */
//void    BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level);
//void    BSP_CAMERA_BlackWhiteConfig(uint32_t Mode);
//void    BSP_CAMERA_ColorEffectConfig(uint32_t Effect);

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

#endif /* __STM32F4_DISCOVERY_CAMERA_H */

/*****END OF FILE******/
