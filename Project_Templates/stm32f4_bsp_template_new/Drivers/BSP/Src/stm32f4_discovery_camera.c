/**
  ******************************************************************************
  * @file    stm32f4_discovery_camera.c
  * @author  Dwijay.Edutech Learning solutions
  * @version V1.0
  * @date    16-May-2015
  * @brief   This file includes the driver for Camera module mounted on
  *          STM32F4 Expansion Base board.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_camera.h"
#include "stm32f4_discovery_lcd.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_CAMERA
  * @brief  This file includes the camera driver for ST expansion board.
  *
  * @paragraph                    User NOTES
    1. How to use this driver:
    --------------------------
       - This driver is used to drive the Camera.
       - The OV2640 component driver MUST be included with this driver.

    2. Driver description:
    ---------------------
      + Initialization steps:
         o Initialize the Camera using the BSP_CAMERA_Init() function.
         o Start the Camera capture or snapshot using CAMERA_Start() function.
         o Suspend, resume or stop the Camera capture using the following functions:
          - BSP_CAMERA_Suspend()
          - BSP_CAMERA_Resume()
          - BSP_CAMERA_Stop()

      + Options
         o Increase or decrease on the fly the brightness and/or contrast
           using the following function:
           - BSP_CAMERA_ContrastBrightnessConfig
         o Add a special effect on the fly using the following functions:
           - BSP_CAMERA_BlackWhiteConfig()
           - BSP_CAMERA_ColorEffectConfig()
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Variables
  * @{
  */
DCMI_HandleTypeDef hdcmi_camera;
DMA_HandleTypeDef hdma_dcmi;

CAMERA_DrvTypeDef  *camera_drv;
uint32_t current_resolution;
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_FunctionPrototypes
  * @{
  */
static void DCMI_MspInit(DCMI_HandleTypeDef* hdcmi);
static uint32_t GetSize(uint32_t resolution);
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Functions
  * @{
  */
static void DCMI_MspInit(DCMI_HandleTypeDef* hdcmi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hdcmi->Instance==DCMI)
  {
    /* Peripheral clock enable */
    __DCMI_CLK_ENABLE();

    /**DCMI GPIO Configuration
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PB6     ------> DCMI_D5
    PB7     ------> DCMI_VSYNC
    PE0     ------> DCMI_D2
    PE1     ------> DCMI_D3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_0
                          |GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PD12 PD6 */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_dcmi.Instance = DMA2_Stream1;
    hdma_dcmi.Init.Channel = DMA_CHANNEL_1;
    hdma_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dcmi.Init.MemInc = DMA_MINC_DISABLE;
    hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dcmi.Init.Mode = DMA_CIRCULAR;
    hdma_dcmi.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dcmi.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_dcmi.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_dcmi.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_dcmi.Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&hdma_dcmi);

    __HAL_LINKDMA(hdcmi,DMA_Handle,hdma_dcmi);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(DCMI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);
  }
}


/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Public_Functions
  * @{
  */
/**
  * @brief  Initializes the Camera.
  * @param  Camera: Pointer to the Camera configuration structure
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Init(uint32_t Resolution)
{
  uint8_t ret = CAMERA_ERROR;

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /* DMA controller clock enable */
  __DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

  hdcmi_camera.Instance = DCMI;
  hdcmi_camera.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
  hdcmi_camera.Init.PCKPolarity = DCMI_PCKPOLARITY_FALLING;
  hdcmi_camera.Init.VSPolarity = DCMI_VSPOLARITY_HIGH;
  hdcmi_camera.Init.HSPolarity = DCMI_HSPOLARITY_HIGH;
  hdcmi_camera.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi_camera.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi_camera.Init.JPEGMode = DCMI_JPEG_DISABLE;

  DCMI_MspInit(&hdcmi_camera);
  HAL_DCMI_Init(&hdcmi_camera);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); // RST PIN
  HAL_Delay(10);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);  // PWM EN PIN
  HAL_Delay(50);
  if(ov9655_drv.ReadID(CAMERA_I2C_ADDRESS) == OV9655_ID)
  {
    /* Initialize the Camera driver structure */
    camera_drv = &ov9655_drv;

    /* Camera Init */
    camera_drv->Init(CAMERA_I2C_ADDRESS, Resolution);

    /* Return CAMERA_OK status */
    ret = CAMERA_OK;
  }

  current_resolution = Resolution;

  return ret;
}


/**
 * @brief DeInit DCMI Module
 * @param hdcmi pass DCMI handle
 */
void BSP_DCMI_MspDeInit(DCMI_HandleTypeDef* hdcmi)
{
  if(hdcmi->Instance==DCMI)
  {
    /* Peripheral clock disable */
    __DCMI_CLK_DISABLE();

    /**DCMI GPIO Configuration
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PB6     ------> DCMI_D5
    PB7     ------> DCMI_VSYNC
    PE0     ------> DCMI_D2
    PE1     ------> DCMI_D3
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_0
                          |GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(hdcmi->DMA_Handle);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(DCMI_IRQn);
  }
}


/**
  * @brief  Starts the Camera capture in continuous mode.
  * @param  buff: pointer to the Camera output buffer
  * @retval None
  */
void BSP_CAMERA_ContinuousStart(uint8_t *buff)
{
  /* Start the Camera capture */
  HAL_DCMI_Start_DMA(&hdcmi_camera, DCMI_MODE_CONTINUOUS, (uint32_t)buff, GetSize(current_resolution));
}

/**
  * @brief  Starts the Camera capture in snapshot mode.
  * @param  buff: pointer to the Camera output buffer
  * @retval None
  */
void BSP_CAMERA_SnapshotStart(uint8_t *buff)
{
  /* Start the Camera capture */
  HAL_DCMI_Start_DMA(&hdcmi_camera, DCMI_MODE_SNAPSHOT, (uint32_t)buff, GetSize(current_resolution));
}

/**
  * @brief  Suspends the Camera capture.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_Suspend(void)
{
  /* Disable the DMA */
  __HAL_DMA_DISABLE(&hdma_dcmi);
  /* Disable the DCMI */
  __HAL_DCMI_DISABLE(&hdcmi_camera);
}

/**
  * @brief  Resumes the Camera capture.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_Resume(void)
{
  BSP_CAMERA_Suspend();   // if user press the camera button resume again then image is not collepse
  BSP_LCD_Clear(Black);   // clear the previous DMA buffer
  /* Enable the DCMI */
  __HAL_DCMI_ENABLE(&hdcmi_camera);
  /* Enable the DMA */
  __HAL_DMA_ENABLE(&hdma_dcmi);
}

/**
  * @brief  Stops the Camera capture.
  * @param  None
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Stop(void)
{
  DCMI_HandleTypeDef *phdcmi;

  uint8_t ret = CAMERA_ERROR;

  /* Get the DCMI handle structure */
  phdcmi = &hdcmi_camera;

  if(HAL_DCMI_Stop(phdcmi) == HAL_OK)
  {
     ret = CAMERA_OK;
  }

  return ret;
}


/**
  * @brief  Get the capture size.
  * @param  current_resolution: the current resolution.
  * @retval cpature size
  */
static uint32_t GetSize(uint32_t resolution)
{
  uint32_t size = 0;

  /* Get capture size */
  switch (resolution)
  {
  case CAMERA_R160x120:
    {
      size =  0x2580;
    }
    break;
  case CAMERA_R320x240:
    {
      size =  0x9600;
    }
    break;
  default:
    {
      break;
    }
  }

  return size;
}


/**
  * @brief  Configures the Camera contrast and brightness.
  * @param  contrast_level: Contrast level 0% to 100%
  * @param  brightness_level: Brightness level 0% to 100%
  * @retval None
  */
void BSP_CAMERA_ContrastBrightnessConfig(uint8_t contrast_level, uint8_t brightness_level)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_CONTRAST_BRIGHTNESS, contrast_level, brightness_level);
  }
}


/**
 * @brief This function is used to mirror and vertical flip the output
 * @param mirror_state  Enter parameter as ENABLE/DISABLE
 * @param flip_state    Enter parameter as ENABLE/DISABLE
 * @retval  None
 */
void BSP_CAMERA_MirrorFlipConfig(uint8_t mirror_state, uint8_t flip_state)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_MIRROR_FLIP, mirror_state, flip_state);
  }
}

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

/*****END OF FILE****/
