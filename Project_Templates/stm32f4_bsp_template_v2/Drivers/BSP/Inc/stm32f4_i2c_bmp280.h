/******************************************************************//**
* @file		stm32f4_i2c_bmp280.h
* @brief	Contains all I2C based BMP280 pressure sensor declarations
* @version	1.0
* @date		18. JUL. 2016
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/
#ifndef __STM32F4_I2C_BMP280_H_
#define __STM32F4_I2C_BMP280_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Peripheral group ----------------------------------------------------------- */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_BMP280
  * @{
  */

/** @defgroup STM32F4_BMP280_Exported_Constants
  * @{
  */



/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_uart.h"

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_BMP280_Public_Macros
 * @{
 */

extern I2C_HandleTypeDef hi2c1_bmp280;

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_BMP280_Public_Macros
 * @{
 */
/************************************************/
/**\name  CHIP ID DEFINITION       */
/***********************************************/
#define BMP280_CHIP_ID1   (0x56)
#define BMP280_CHIP_ID2   (0x57)
#define BMP280_CHIP_ID3   (0x58)
/************************************************/
/**\name  I2C ADDRESS DEFINITION       */
/***********************************************/
#define BMP280_I2C_ADDRESS1                  (0x76<<1)
#define BMP280_I2C_ADDRESS2                  (0x77<<1)
/************************************************/
/**\name  POWER MODE DEFINITION       */
/***********************************************/
/* Sensor Specific constants */
#define BMP280_SOFT_RESET_CODE               (0xB6)

/************************************************/
/**\name  OVERSAMPLING DEFINITION       */
/***********************************************/
#define BMP280_OVERSAMP_SKIPPED          (0x00)
#define BMP280_OVERSAMP_1X               (0x01)
#define BMP280_OVERSAMP_2X               (0x02)
#define BMP280_OVERSAMP_4X               (0x03)
#define BMP280_OVERSAMP_8X               (0x04)
#define BMP280_OVERSAMP_16X              (0x05)
/************************************************/
/**\name  WORKING MODE DEFINITION       */
/***********************************************/
#define BMP280_ULTRA_LOW_POWER_MODE          (0x00)
#define BMP280_LOW_POWER_MODE              (0x01)
#define BMP280_STANDARD_RESOLUTION_MODE      (0x02)
#define BMP280_HIGH_RESOLUTION_MODE          (0x03)
#define BMP280_ULTRA_HIGH_RESOLUTION_MODE    (0x04)

#define BMP280_ULTRALOWPOWER_OVERSAMP_PRESSURE          BMP280_OVERSAMP_1X
#define BMP280_ULTRALOWPOWER_OVERSAMP_TEMPERATURE       BMP280_OVERSAMP_1X

#define BMP280_LOWPOWER_OVERSAMP_PRESSURE          BMP280_OVERSAMP_2X
#define BMP280_LOWPOWER_OVERSAMP_TEMPERATURE           BMP280_OVERSAMP_1X

#define BMP280_STANDARDRESOLUTION_OVERSAMP_PRESSURE     BMP280_OVERSAMP_4X
#define BMP280_STANDARDRESOLUTION_OVERSAMP_TEMPERATURE  BMP280_OVERSAMP_1X

#define BMP280_HIGHRESOLUTION_OVERSAMP_PRESSURE         BMP280_OVERSAMP_8X
#define BMP280_HIGHRESOLUTION_OVERSAMP_TEMPERATURE      BMP280_OVERSAMP_1X

#define BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE       BMP280_OVERSAMP_16X
#define BMP280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE    BMP280_OVERSAMP_2X


/************************************************/
/**\name  CALIBRATION PARAMETERS DEFINITION       */
/***********************************************/
/*calibration parameters */
#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BMP280_TEMPERATURE_CALIB_DIG_T1_MSB_REG             (0x89)
#define BMP280_TEMPERATURE_CALIB_DIG_T2_LSB_REG             (0x8A)
#define BMP280_TEMPERATURE_CALIB_DIG_T2_MSB_REG             (0x8B)
#define BMP280_TEMPERATURE_CALIB_DIG_T3_LSB_REG             (0x8C)
#define BMP280_TEMPERATURE_CALIB_DIG_T3_MSB_REG             (0x8D)
#define BMP280_PRESSURE_CALIB_DIG_P1_LSB_REG                (0x8E)
#define BMP280_PRESSURE_CALIB_DIG_P1_MSB_REG                (0x8F)
#define BMP280_PRESSURE_CALIB_DIG_P2_LSB_REG                (0x90)
#define BMP280_PRESSURE_CALIB_DIG_P2_MSB_REG                (0x91)
#define BMP280_PRESSURE_CALIB_DIG_P3_LSB_REG                (0x92)
#define BMP280_PRESSURE_CALIB_DIG_P3_MSB_REG                (0x93)
#define BMP280_PRESSURE_CALIB_DIG_P4_LSB_REG                (0x94)
#define BMP280_PRESSURE_CALIB_DIG_P4_MSB_REG                (0x95)
#define BMP280_PRESSURE_CALIB_DIG_P5_LSB_REG                (0x96)
#define BMP280_PRESSURE_CALIB_DIG_P5_MSB_REG                (0x97)
#define BMP280_PRESSURE_CALIB_DIG_P6_LSB_REG                (0x98)
#define BMP280_PRESSURE_CALIB_DIG_P6_MSB_REG                (0x99)
#define BMP280_PRESSURE_CALIB_DIG_P7_LSB_REG                (0x9A)
#define BMP280_PRESSURE_CALIB_DIG_P7_MSB_REG                (0x9B)
#define BMP280_PRESSURE_CALIB_DIG_P8_LSB_REG                (0x9C)
#define BMP280_PRESSURE_CALIB_DIG_P8_MSB_REG                (0x9D)
#define BMP280_PRESSURE_CALIB_DIG_P9_LSB_REG                (0x9E)
#define BMP280_PRESSURE_CALIB_DIG_P9_MSB_REG                (0x9F)
/************************************************/
/**\name  REGISTER ADDRESS DEFINITION       */
/***********************************************/
#define BMP280_CHIP_ID_REG                   (0xD0)  /*Chip ID Register */
#define BMP280_RST_REG                       (0xE0) /*Softreset Register */
#define BMP280_STAT_REG                      (0xF3)  /*Status Register */
#define BMP280_CTRL_MEAS_REG                 (0xF4)  /*Ctrl Measure Register */
#define BMP280_CONFIG_REG                    (0xF5)  /*Configuration Register */
#define BMP280_PRESSURE_MSB_REG              (0xF7)  /*Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG              (0xF8)  /*Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG             (0xF9)  /*Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG           (0xFA)  /*Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG           (0xFB)  /*Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG          (0xFC)  /*Temperature XLSB Reg */
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION      */
/***********************************************/
/* Status Register */
#define BMP280_STATUS_REG_MEASURING__POS           (3)
#define BMP280_STATUS_REG_MEASURING__MSK           (0x08)
#define BMP280_STATUS_REG_MEASURING__LEN           (1)
#define BMP280_STATUS_REG_MEASURING__REG           (BMP280_STAT_REG)

#define BMP280_STATUS_REG_IM_UPDATE__POS            (0)
#define BMP280_STATUS_REG_IM_UPDATE__MSK            (0x01)
#define BMP280_STATUS_REG_IM_UPDATE__LEN            (1)
#define BMP280_STATUS_REG_IM_UPDATE__REG           (BMP280_STAT_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR TEMPERATURE OVERSAMPLING */
/***********************************************/
/* Control Measurement Register */
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__POS             (5)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__MSK             (0xE0)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__LEN             (3)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG             \
(BMP280_CTRL_MEAS_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR PRESSURE OVERSAMPLING */
/***********************************************/
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__POS             (2)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__MSK             (0x1C)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__LEN             (3)
#define BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG             \
(BMP280_CTRL_MEAS_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR POWER MODE */
/***********************************************/
#define BMP280_CTRL_MEAS_REG_POWER_MODE__POS              (0)
#define BMP280_CTRL_MEAS_REG_POWER_MODE__MSK              (0x03)
#define BMP280_CTRL_MEAS_REG_POWER_MODE__LEN              (2)
#define BMP280_CTRL_MEAS_REG_POWER_MODE__REG             (BMP280_CTRL_MEAS_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR STANDBY DURATION */
/***********************************************/
/* Configuration Register */
#define BMP280_CONFIG_REG_STANDBY_DURN__POS                 (5)
#define BMP280_CONFIG_REG_STANDBY_DURN__MSK                 (0xE0)
#define BMP280_CONFIG_REG_STANDBY_DURN__LEN                 (3)
#define BMP280_CONFIG_REG_STANDBY_DURN__REG                 (BMP280_CONFIG_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR IIR FILTER */
/***********************************************/
#define BMP280_CONFIG_REG_FILTER__POS              (2)
#define BMP280_CONFIG_REG_FILTER__MSK              (0x1C)
#define BMP280_CONFIG_REG_FILTER__LEN              (3)
#define BMP280_CONFIG_REG_FILTER__REG              (BMP280_CONFIG_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR SPI ENABLE*/
/***********************************************/
#define BMP280_CONFIG_REG_SPI3_ENABLE__POS             (0)
#define BMP280_CONFIG_REG_SPI3_ENABLE__MSK             (0x01)
#define BMP280_CONFIG_REG_SPI3_ENABLE__LEN             (1)
#define BMP280_CONFIG_REG_SPI3_ENABLE__REG             (BMP280_CONFIG_REG)
/************************************************/
/**\name  BIT LENGTH,POSITION AND MASK DEFINITION
FOR PRESSURE AND TEMPERATURE DATA REGISTERS */
/***********************************************/
/* Data Register */
#define BMP280_PRESSURE_XLSB_REG_DATA__POS         (4)
#define BMP280_PRESSURE_XLSB_REG_DATA__MSK         (0xF0)
#define BMP280_PRESSURE_XLSB_REG_DATA__LEN         (4)
#define BMP280_PRESSURE_XLSB_REG_DATA__REG         (BMP280_PRESSURE_XLSB_REG)

#define BMP280_TEMPERATURE_XLSB_REG_DATA__POS      (4)
#define BMP280_TEMPERATURE_XLSB_REG_DATA__MSK      (0xF0)
#define BMP280_TEMPERATURE_XLSB_REG_DATA__LEN      (4)
#define BMP280_TEMPERATURE_XLSB_REG_DATA__REG (BMP280_TEMPERATURE_XLSB_REG)

/****************************************************/
/**\name  ARRAY PARAMETER FOR CALIBRATION     */
/***************************************************/
#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB (0)
#define BMP280_TEMPERATURE_CALIB_DIG_T1_MSB (1)
#define BMP280_TEMPERATURE_CALIB_DIG_T2_LSB (2)
#define BMP280_TEMPERATURE_CALIB_DIG_T2_MSB (3)
#define BMP280_TEMPERATURE_CALIB_DIG_T3_LSB (4)
#define BMP280_TEMPERATURE_CALIB_DIG_T3_MSB (5)
#define BMP280_PRESSURE_CALIB_DIG_P1_LSB  (6)
#define BMP280_PRESSURE_CALIB_DIG_P1_MSB  (7)
#define BMP280_PRESSURE_CALIB_DIG_P2_LSB  (8)
#define BMP280_PRESSURE_CALIB_DIG_P2_MSB  (9)
#define BMP280_PRESSURE_CALIB_DIG_P3_LSB  (10)
#define BMP280_PRESSURE_CALIB_DIG_P3_MSB  (11)
#define BMP280_PRESSURE_CALIB_DIG_P4_LSB  (12)
#define BMP280_PRESSURE_CALIB_DIG_P4_MSB  (13)
#define BMP280_PRESSURE_CALIB_DIG_P5_LSB  (14)
#define BMP280_PRESSURE_CALIB_DIG_P5_MSB  (15)
#define BMP280_PRESSURE_CALIB_DIG_P6_LSB  (16)
#define BMP280_PRESSURE_CALIB_DIG_P6_MSB  (17)
#define BMP280_PRESSURE_CALIB_DIG_P7_LSB  (18)
#define BMP280_PRESSURE_CALIB_DIG_P7_MSB  (19)
#define BMP280_PRESSURE_CALIB_DIG_P8_LSB  (20)
#define BMP280_PRESSURE_CALIB_DIG_P8_MSB  (21)
#define BMP280_PRESSURE_CALIB_DIG_P9_LSB  (22)
#define BMP280_PRESSURE_CALIB_DIG_P9_MSB  (23)


/** BMP280 Sensor mode Type **/
typedef enum
{
  BMP280_SLEEP_MODE = 0x00,
  BMP280_FORCED_MODE = 0x01,
  BMP280_NORMAL_MODE = 0x03
}BMP280_Mode_Type;


/** BMP280 Filter Type **/
typedef enum
{
  BMP280_FILTER_COEFF_OFF = 0x00,
  BMP280_FILTER_COEFF_2 = 0x01,
  BMP280_FILTER_COEFF_4 = 0x02,
  BMP280_FILTER_COEFF_8 = 0x03,
  BMP280_FILTER_COEFF_16 = 0x04
}BMP280_Filter_Coeff_Type;

/** BMP280 Standby Type **/
typedef enum
{
  BMP280_STANDBY_TIME_1_MS = 0x00,
  BMP280_STANDBY_TIME_63_MS = 0x01,
  BMP280_STANDBY_TIME_125_MS = 0x02,
  BMP280_STANDBY_TIME_250_MS = 0x03,
  BMP280_STANDBY_TIME_500_MS = 0x04,
  BMP280_STANDBY_TIME_1000_MS = 0x05,
  BMP280_STANDBY_TIME_10_MS = 0x06,
  BMP280_STANDBY_TIME_20_MS = 0x07
}BMP280_Standby_Type ;

/** BMP280 Dataframe Type **/
typedef enum
{
  BMP280_DATA_FRAME_PRESSURE_MSB_BYTE = 0,
  BMP280_DATA_FRAME_PRESSURE_LSB_BYTE = 1,
  BMP280_DATA_FRAME_PRESSURE_XLSB_BYTE = 2,
  BMP280_DATA_FRAME_TEMPERATURE_MSB_BYTE = 3,
  BMP280_DATA_FRAME_TEMPERATURE_LSB_BYTE = 4,
  BMP280_DATA_FRAME_TEMPERATURE_XLSB_BYTE = 5,
}BMP280_DataFrame_Type;

/**************************************************************/
/**\name  STRUCTURE DEFINITIONS                         */
/**************************************************************/
/*!
 * @brief This structure holds all device specific calibration parameters
 */
typedef struct bmp280_calib_param_t {
  uint16_t dig_T1;/**<calibration T1 data*/
  int16_t dig_T2;/**<calibration T2 data*/
  int16_t dig_T3;/**<calibration T3 data*/
  uint16_t dig_P1;/**<calibration P1 data*/
  int16_t dig_P2;/**<calibration P2 data*/
  int16_t dig_P3;/**<calibration P3 data*/
  int16_t dig_P4;/**<calibration P4 data*/
  int16_t dig_P5;/**<calibration P5 data*/
  int16_t dig_P6;/**<calibration P6 data*/
  int16_t dig_P7;/**<calibration P7 data*/
  int16_t dig_P8;/**<calibration P8 data*/
  int16_t dig_P9;/**<calibration P9 data*/

  int32_t t_fine;/**<calibration t_fine data*/
}BMP280_calibration_param;
/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_I2C_BMP280_Public_Functions
 * @{
 */
void BSP_BMP280_I2C1_Init(void);

uint8_t BMP280_GetID(void);
void BMP280_SoftReset(void);
uint8_t BMP280_GetID(void);

void BMP280_SoftReset(void);
uint8_t BMP280_GetStatus(void);
void BMP280_ReadData(void);
uint8_t BMP280_GetConfig(void);
uint8_t BMP280_GetCtrlMeasurement(void);
uint8_t BMP280_GetCtrlHumidity(void);
void BMP280_ReadCalibrationParams(void);
uint8_t BMP280_ReadMeasurements(void);
void BMP280_SetOversamplingPressure(uint8_t Value);
void BMP280_SetOversamplingTemperature(uint8_t Value);
void BMP280_SetPowerMode(uint8_t Value);
void BMP280_SetFilterCoefficient(uint8_t Value);
void BMP280_SetStandbyTime(uint8_t Value);
unsigned char BMP280_IsMeasuring(void);
void BMP280_INIT(void);
void BMP280_GetTemperature(void);
void BMP280_GetPressure(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /*__STM32F4_I2C_BMP280_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
