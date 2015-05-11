/**
  ******************************************************************************
  * @file    stm32f4_discovery_tsc.h
  * @author  Dwijay.edutech leaning solutions
  * @version V1.0.0
  * @date    07-May-2015
  * @brief   This file contains all the functions prototypes for the
  *          stm32f4_discovery_tsc.c driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_TSC_H
#define __STM32F4_DISCOVERY_TSC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_TSC
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_TSC_Exported_Types
  * @{
  */

extern I2C_HandleTypeDef hi2c1_tsc;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_i2c1_rx;

/**
 * @brief  Touch Screen Information structure
 */
typedef struct
{
 uint16_t TouchDetected;
 uint16_t X;
 uint16_t Y;
 uint16_t Z;
}TS_STATE;


/**
  * @brief  IO_Expander Error codes
  */
typedef enum
{
  IOE_OK = 0,
  IOE_FAILURE,
  IOE_TIMEOUT,
  PARAM_ERROR,
  IOE1_NOT_OPERATIONAL,
}IOE_Status_TypDef;


/**
  * @brief  IO bit values
  */
typedef enum
{
  BitReset = 0,
  BitSet = 1
}IOE_BitValue_TypeDef;


/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Exported_Constants
  * @{
  */

/********************************************************************//**
* @brief Hardware Configuration
**********************************************************************/
/**
  * @brief  The 7 bits IO Expanders adresses and chip IDs
  */
#define IOE_1_ADDR                 0x82
#define STMPE811_ID                0x0811


/********************************************************************//**
* @brief Functional and Interrupt Management
**********************************************************************/
/**
  * @brief  IO Expander Functionalities definitions
  */
#define IOE_ADC_FCT              0x01
#define IOE_TS_FCT               0x02
#define IOE_IO_FCT               0x04
#define IOE_TEMPSENS_FCT         0x08

/**
  * @brief  Interrupt source configuration definitons
  */
#define IOE_ITSRC_TSC           0x01  /* IO_Exapnder 1 */
#define IOE_ITSRC_INMEMS        0x02  /* IO_Exapnder 1 */
#define IOE_ITSRC_TEMPSENS      0x08  /* IO_Exapnder 2 */

/**
  * @brief  Glaobal Interrupts definitions
  */
#define IOE_GIT_GPIO             0x80
#define IOE_GIT_ADC              0x40
#define IOE_GIT_TEMP             0x20
#define IOE_GIT_FE               0x10
#define IOE_GIT_FF               0x08
#define IOE_GIT_FTH              0x02
#define IOE_GIT_TOUCH            0x01


/********************************************************************//**
* @brief STMPE811 device register definition
**********************************************************************/
/**
  * @brief  Identification registers
  */
#define IOE_REG_CHP_ID             0x00
#define IOE_REG_ID_VER             0x02

/**
  * @brief  General Control Registers
  */
#define IOE_REG_SYS_CTRL1          0x03
#define IOE_REG_SYS_CTRL2          0x04
#define IOE_REG_SPI_CFG            0x08

/**
  * @brief  Interrupt Control register
  */
#define IOE_REG_INT_CTRL           0x09
#define IOE_REG_INT_EN             0x0A
#define IOE_REG_INT_STA            0x0B
#define IOE_REG_GPIO_INT_EN        0x0C
#define IOE_REG_GPIO_INT_STA       0x0D

/**
  * @brief  GPIO Registers
  */
#define IOE_REG_GPIO_SET_PIN       0x10
#define IOE_REG_GPIO_CLR_PIN       0x11
#define IOE_REG_GPIO_MP_STA        0x12
#define IOE_REG_GPIO_DIR           0x13
#define IOE_REG_GPIO_ED            0x14
#define IOE_REG_GPIO_RE            0x15
#define IOE_REG_GPIO_FE            0x16
#define IOE_REG_GPIO_AF            0x17

/**
  * @brief  ADC Registers
  */
#define IOE_REG_ADC_INT_EN         0x0E
#define IOE_REG_ADC_INT_STA        0x0F
#define IOE_REG_ADC_CTRL1          0x20
#define IOE_REG_ADC_CTRL2          0x21
#define IOE_REG_ADC_CAPT           0x22
#define IOE_REG_ADC_DATA_CH0       0x30 /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH1       0x32 /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH2       0x34 /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH3       0x36 /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH4       0x38 /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH5       0x3A /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH6       0x3B /* 16-Bit register */
#define IOE_REG_ADC_DATA_CH7       0x3C /* 16-Bit register */

/**
  * @brief  TouchScreen Registers
  */
#define IOE_REG_TSC_CTRL           0x40
#define IOE_REG_TSC_CFG            0x41
#define IOE_REG_WDM_TR_X           0x42
#define IOE_REG_WDM_TR_Y           0x44
#define IOE_REG_WDM_BL_X           0x46
#define IOE_REG_WDM_BL_Y           0x48
#define IOE_REG_FIFO_TH            0x4A
#define IOE_REG_FIFO_STA           0x4B
#define IOE_REG_FIFO_SIZE          0x4C
#define IOE_REG_TSC_DATA_X         0x4D
#define IOE_REG_TSC_DATA_Y         0x4F
#define IOE_REG_TSC_DATA_Z         0x51
#define IOE_REG_TSC_DATA_XYZ       0x52
#define IOE_REG_TSC_FRACT_XYZ      0x56
#define IOE_REG_TSC_DATA           0x57
#define IOE_REG_TSC_I_DRIVE        0x58
#define IOE_REG_TSC_SHIELD         0x59

/**
  * @brief  Temperature Sensor registers
  */
#define IOE_REG_TEMP_CTRL          0x60
#define IOE_REG_TEMP_DATA          0x61
#define IOE_REG_TEMP_TH            0x62

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Exported_Macros
  * @{
  */
/********************************************************************//**
* @brief Functions parameters defines
**********************************************************************/
/**
  * @brief Touch Screen Pins definition
  */
#define TOUCH_YD                    IO_Pin_1 /* IO_Exapnader_1 */ /* Input */
#define TOUCH_XD                    IO_Pin_2 /* IO_Exapnader_1 */ /* Input */
#define TOUCH_YU                    IO_Pin_3 /* IO_Exapnader_1 */ /* Input */
#define TOUCH_XU                    IO_Pin_4 /* IO_Exapnader_1 */ /* Input */
#define TOUCH_IO_ALL                (uint32_t)(IO_Pin_1 | IO_Pin_2 | IO_Pin_3 | IO_Pin_4)

/**
  * @brief  IO Pins
  */
#define IO_Pin_0                 0x01
#define IO_Pin_1                 0x02
#define IO_Pin_2                 0x04
#define IO_Pin_3                 0x08
#define IO_Pin_4                 0x10
#define IO_Pin_5                 0x20
#define IO_Pin_6                 0x40
#define IO_Pin_7                 0x80
#define IO_Pin_ALL               0xFF

/**
  * @brief  IO Pin directions
  */
#define Direction_IN             0x00
#define Direction_OUT            0x01

/**
  * @brief  Interrupt Line output parameters
  */
#define Polarity_Low             0x00
#define Polarity_High            0x04
#define Type_Level               0x00
#define Type_Edge                0x02

/**
  * @brief IO Interrupts
  */
#define IO_IT_0                  0x01
#define IO_IT_1                  0x02
#define IO_IT_2                  0x04
#define IO_IT_3                  0x08
#define IO_IT_4                  0x10
#define IO_IT_5                  0x20
#define IO_IT_6                  0x40
#define IO_IT_7                  0x80
#define ALL_IT                   0xFF
#define IOE_TS_IT                (uint8_t)(IO_IT_0 | IO_IT_1 | IO_IT_2)

/**
  * @brief  Edge detection value
  */
#define EDGE_FALLING              0x01
#define EDGE_RISING               0x02

/**
  * @brief  Global interrupt Enable bit
  */
#define IOE_GIT_EN                0x01

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Exported_Functions
  * @{
  */
void MX_I2C1_Init(void);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);

/**
  * @brief  Configuration and initialization functions
  */
uint8_t IOE_Config(void);
uint8_t IOE_ITConfig(uint32_t IOE_ITSRC_Source);

/**
  * @brief  Timeout user callback function. This function is called when a timeout
  *         condition occurs during communication with IO Expander. Only protoype
  *         of this function is decalred in IO Expander driver. Its implementation
  *         may be done into user application. This function may typically stop
  *         current operations and reset the I2C peripheral and IO Expander.
  *         To enable this function use uncomment the define USE_TIMEOUT_USER_CALLBACK
  *         at the top of this file.
  */
#ifdef USE_TIMEOUT_USER_CALLBACK
 uint8_t IOE_TimeoutUserCallback(void);
#else
 #define IOE_TimeoutUserCallback()  IOE_TIMEOUT
#endif /* USE_TIMEOUT_USER_CALLBACK */

 /**
   * @brief IO pins control functions
   */
 uint8_t IOE_ReadIOPin(uint32_t IO_Pin);

 /**
   * @brief Touch Screen controller functions
   */
 TS_STATE* IOE_TS_GetState(void);

 /**
   * @brief Interrupts Mangement functions
   */
 FlagStatus IOE_GetGITStatus(uint8_t DeviceAddr, uint8_t Global_IT);
 uint8_t IOE_ClearGITPending(uint8_t DeviceAddr, uint8_t IO_IT);
 FlagStatus IOE_GetIOITStatus(uint8_t DeviceAddr, uint8_t IO_IT);
 uint8_t IOE_ClearIOITPending(uint8_t DeviceAddr, uint8_t IO_IT);

 /**
   * @brief Temperature Sensor functions
   */
 uint32_t IOE_TempSens_GetData(void);

 /**
   * @brief IO-Expander Control functions
   */
 uint8_t IOE_IsOperational(uint8_t DeviceAddr);
 uint8_t IOE_Reset(uint8_t DeviceAddr);
 uint16_t IOE_ReadID(uint8_t DeviceAddr);

 uint8_t IOE_FnctCmd(uint8_t DeviceAddr, uint8_t Fct, FunctionalState NewState);
 uint8_t IOE_IOPinConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Direction);
 uint8_t IOE_GITCmd(uint8_t DeviceAddr, FunctionalState NewState);
 uint8_t IOE_GITConfig(uint8_t DeviceAddr, uint8_t Global_IT, FunctionalState NewState);
 uint8_t IOE_IOITConfig(uint8_t DeviceAddr, uint8_t IO_IT, FunctionalState NewState);

 /**
   * @brief Low Layer functions
   */
 uint8_t IOE_TS_Config(void);
 uint8_t IOE_TempSens_Config(void);
 uint8_t IOE_IOAFConfig(uint8_t DeviceAddr, uint8_t IO_Pin, FunctionalState NewState);
 uint8_t IOE_IOEdgeConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Edge);
 uint8_t IOE_ITOutConfig(uint8_t Polarity, uint8_t Type);

 uint8_t I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue);
 uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr);
 uint16_t I2C_ReadDataBuffer(uint8_t DeviceAddr, uint8_t RegisterAddr);

 uint16_t IOE_TS_Read_X(void);
 uint16_t IOE_TS_Read_Y(void);

/**
  * @}
  */


/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_TSC_H */
/**
  * @}
  */
/*****END OF FILE****/
