/******************************************************************//**
* @file		stm32f4_i2c_color_sensor.h
* @brief	Contains all I2C based color sensor declarations
* @version	1.0
* @date		18. JUL. 2016
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/
#ifndef __STM32F4_I2C_COLOR_SENSOR_H_
#define __STM32F4_I2C_COLOR_SENSOR_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Peripheral group ----------------------------------------------------------- */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_COLOR_SENSOR
  * @{
  */

/** @defgroup STM32F4_COLOR_SENSOR_Exported_Constants
  * @{
  */



/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_uart.h"

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_COLOR_SENSOR_Public_Macros
 * @{
 */

extern I2C_HandleTypeDef hi2c1_color;

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_COLOR_SENSOR_Public_Macros
 * @{
 */

/** I2C ADDRESS DEFINITIONS  **/
#define COLOR_SENSOR_ADDR    (0x39<<1)

/* Control Register */
#define COLOR_SENSOR_REG_CTL 0x80
#define REG_CTL_POWER        0x01
#define REG_CTL_ADC_EN       0x02

/* Color read Register */
//#define COLOR_SENSOR_REG_CMD 0x90         //The REG_BLOCK_READ and REG_GREEN_LOW direction are the same

#define REG_GREEN_LOW        0x90
#define REG_GREEN_HIGH       0x91
#define REG_RED_LOW          0x92
#define REG_RED_HIGH         0x93
#define REG_BLUE_LOW         0x94
#define REG_BLUE_HIGH        0x95
#define REG_CLEAR_LOW        0x96
#define REG_CLEAR_HIGH       0x97

/* Color read Register */
#define COLOR_SENSOR_REG_CMD 0xB0

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) < (Y)) ? (Y) : (X))

/** Color Sensor Type**/
typedef enum
{
    GREEN = 0,
    RED,
    BLUE,
    CLEAR,
    NUM_COLORS
}ColorSense_TypeDef;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_I2C_COLOR_SENSOR_Public_Functions
 * @{
 */
void BSP_COLOR_SENSOR_I2C1_Init(void);

;
uint8_t I2C_Read_Bytes (uint16_t Rd_address, uint8_t* buf_data, uint16_t length);
void ColorSensor_Init(void);
uint16_t ColorSensor_ReadColor(ColorSense_TypeDef colour);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /*__STM32F4_I2C_COLOR_SENSOR_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
