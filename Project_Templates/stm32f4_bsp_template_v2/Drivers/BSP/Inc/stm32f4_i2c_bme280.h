/******************************************************************//**
* @file		stm32f4_i2c_bme280.h
* @brief	Contains all I2C based BME280 pressure sensor declarations
* @version	1.0
* @date		18. JUL. 2016
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/
#ifndef __STM32F4_I2C_BME280_H_
#define __STM32F4_I2C_BME280_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Peripheral group ----------------------------------------------------------- */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_BME280
  * @{
  */

/** @defgroup STM32F4_BME280_Exported_Constants
  * @{
  */



/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_uart.h"

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_BME280_Public_Macros
 * @{
 */

extern I2C_HandleTypeDef hi2c1_bme280;

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_BME280_Public_Macros
 * @{
 */
#define BME280_CHIP_ID 0x60


/** I2C ADDRESS DEFINITIONS  **/
#define BME280_I2C_ADDRESS1                                         (0x76<<1)
#define BME280_I2C_ADDRESS2                                         (0x77<<1)


#define BME280_SOFT_RESET   0xB6        // Soft reset


/******************************************************************************/
/*                          DELAY DEFINITIONS  */
/******************************************************************************/
#define T_INIT_MAX                                                  20 /* 20/16 = 1.25 ms */
#define T_MEASURE_PER_OSRS_MAX                                      37 /* 37/16 = 2.3125 ms*/
#define T_SETUP_PRESSURE_MAX                                        10 /* 10/16 = 0.625 ms */
#define T_SETUP_HUMIDITY_MAX                                        10 /* 10/16 = 0.625 ms */

/******************************************************************************/
/*        DEFINITIONS FOR ARRAY SIZE OF DATA   */
/******************************************************************************/
#define        BME280_HUMIDITY_DATA_SIZE                            2
#define        BME280_TEMPERATURE_DATA_SIZE                         3
#define        BME280_PRESSURE_DATA_SIZE                            3
#define        BME280_DATA_FRAME_SIZE                               8

/* data frames includes temperature, pressure and humidity */
#define        BME280_CALIB_DATA_SIZE                               26

#define        BME280_TEMPERATURE_MSB_DATA                          0
#define        BME280_TEMPERATURE_LSB_DATA                          1
#define        BME280_TEMPERATURE_XLSB_DATA                         2
#define        BME280_PRESSURE_MSB_DATA                             0
#define        BME280_PRESSURE_LSB_DATA                             1
#define        BME280_PRESSURE_XLSB_DATA                            2
#define        BME280_HUMIDITY_MSB_DATA                             0
#define        BME280_HUMIDITY_LSB_DATA                             1


/******************************************************************************/
/*               CALIBRATION REGISTER ADDRESS DEFINITIONS  */
/******************************************************************************/
/*calibration parameters */
#define BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG                     0x88
#define BME280_TEMPERATURE_CALIB_DIG_T1_MSB_REG                     0x89
#define BME280_TEMPERATURE_CALIB_DIG_T2_LSB_REG                     0x8A
#define BME280_TEMPERATURE_CALIB_DIG_T2_MSB_REG                     0x8B
#define BME280_TEMPERATURE_CALIB_DIG_T3_LSB_REG                     0x8C
#define BME280_TEMPERATURE_CALIB_DIG_T3_MSB_REG                     0x8D

#define BME280_PRESSURE_CALIB_DIG_P1_LSB_REG                        0x8E
#define BME280_PRESSURE_CALIB_DIG_P1_MSB_REG                        0x8F
#define BME280_PRESSURE_CALIB_DIG_P2_LSB_REG                        0x90
#define BME280_PRESSURE_CALIB_DIG_P2_MSB_REG                        0x91
#define BME280_PRESSURE_CALIB_DIG_P3_LSB_REG                        0x92
#define BME280_PRESSURE_CALIB_DIG_P3_MSB_REG                        0x93
#define BME280_PRESSURE_CALIB_DIG_P4_LSB_REG                        0x94
#define BME280_PRESSURE_CALIB_DIG_P4_MSB_REG                        0x95
#define BME280_PRESSURE_CALIB_DIG_P5_LSB_REG                        0x96
#define BME280_PRESSURE_CALIB_DIG_P5_MSB_REG                        0x97
#define BME280_PRESSURE_CALIB_DIG_P6_LSB_REG                        0x98
#define BME280_PRESSURE_CALIB_DIG_P6_MSB_REG                        0x99
#define BME280_PRESSURE_CALIB_DIG_P7_LSB_REG                        0x9A
#define BME280_PRESSURE_CALIB_DIG_P7_MSB_REG                        0x9B
#define BME280_PRESSURE_CALIB_DIG_P8_LSB_REG                        0x9C
#define BME280_PRESSURE_CALIB_DIG_P8_MSB_REG                        0x9D
#define BME280_PRESSURE_CALIB_DIG_P9_LSB_REG                        0x9E
#define BME280_PRESSURE_CALIB_DIG_P9_MSB_REG                        0x9F

#define BME280_HUMIDITY_CALIB_DIG_H1_REG                            0xA1
#define BME280_HUMIDITY_CALIB_DIG_H2_LSB_REG                        0xE1
#define BME280_HUMIDITY_CALIB_DIG_H2_MSB_REG                        0xE2
#define BME280_HUMIDITY_CALIB_DIG_H3_REG                            0xE3
#define BME280_HUMIDITY_CALIB_DIG_H4_MSB_REG                        0xE4
#define BME280_HUMIDITY_CALIB_DIG_H4_LSB_REG                        0xE5
#define BME280_HUMIDITY_CALIB_DIG_H5_MSB_REG                        0xE6
#define BME280_HUMIDITY_CALIB_DIG_H6_REG                            0xE7
/******************************************************************************/
/*                       REGISTER ADDRESS DEFINITIONS  */
/******************************************************************************/
#define BME280_CHIP_ID_REG                                          0xD0  /*Chip ID Register */
#define BME280_RST_REG                                              0xE0  /*Softreset Register */
#define BME280_STAT_REG                                             0xF3  /*Status Register */
#define BME280_CTRL_MEAS_REG                                        0xF4  /*Ctrl Measure Register */
#define BME280_CTRL_HUMIDITY_REG                                    0xF2  /*Ctrl Humidity Register*/
#define BME280_CONFIG_REG                                           0xF5  /*Configuration Register */
#define BME280_PRESSURE_MSB_REG                                     0xF7  /*Pressure MSB Register */
#define BME280_PRESSURE_LSB_REG                                     0xF8  /*Pressure LSB Register */
#define BME280_PRESSURE_XLSB_REG                                    0xF9  /*Pressure XLSB Register */
#define BME280_TEMPERATURE_MSB_REG                                  0xFA  /*Temperature MSB Reg */
#define BME280_TEMPERATURE_LSB_REG                                  0xFB  /*Temperature LSB Reg */
#define BME280_TEMPERATURE_XLSB_REG                                 0xFC  /*Temperature XLSB Reg */
#define BME280_HUMIDITY_MSB_REG                                     0xFD  /*Humidity MSB Reg */
#define BME280_HUMIDITY_LSB_REG                                     0xFE  /*Humidity LSB Reg */
/******************************************************************************/
/*                BIT MASK, LENGTH AND POSITION DEFINITIONS  */
/******************************************************************************/
/* Status Register */
#define BME280_STAT_REG_MEASURING__POS                              3
#define BME280_STAT_REG_MEASURING__MSK                              0x08
#define BME280_STAT_REG_MEASURING__LEN                              1
#define BME280_STAT_REG_MEASURING__REG                              BME280_STAT_REG

#define BME280_STAT_REG_IM_UPDATE__POS                              0
#define BME280_STAT_REG_IM_UPDATE__MSK                              0x01
#define BME280_STAT_REG_IM_UPDATE__LEN                              1
#define BME280_STAT_REG_IM_UPDATE__REG                              BME280_STAT_REG
/******************************************************************************/
/*  BIT MASK, LENGTH AND POSITION DEFINITIONS FOR TEMPERATURE OVERSAMPLING  */
/******************************************************************************/
/* Control Measurement Register */
#define BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__POS              5
#define BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__MSK              0xE0
#define BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__LEN              3
#define BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__REG              BME280_CTRL_MEAS_REG
/******************************************************************************/
/*     BIT MASK, LENGTH AND POSITION DEFINITIONS FOR PRESSURE OVERSAMPLING  */
/******************************************************************************/
#define BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__POS                 2
#define BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__MSK                 0x1C
#define BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__LEN                 3
#define BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG                 BME280_CTRL_MEAS_REG
/******************************************************************************/
/*        BIT MASK, LENGTH AND POSITION DEFINITIONS FOR POWER MODE  */
/******************************************************************************/
#define BME280_CTRL_MEAS_REG_POWER_MODE__POS                        0
#define BME280_CTRL_MEAS_REG_POWER_MODE__MSK                        0x03
#define BME280_CTRL_MEAS_REG_POWER_MODE__LEN                        2
#define BME280_CTRL_MEAS_REG_POWER_MODE__REG                        BME280_CTRL_MEAS_REG
/******************************************************************************/
/*    BIT MASK, LENGTH AND POSITION DEFINITIONS FOR HUMIDITY OVERSAMPLING  */
/******************************************************************************/
#define BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY__POS             0
#define BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY__MSK             0x07
#define BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY__LEN             3
#define BME280_CTRL_HUMIDITY_REG_OVERSAMP_HUMIDITY__REG             BME280_CTRL_HUMIDITY_REG
/******************************************************************************/
/*        BIT MASK, LENGTH AND POSITION DEFINITIONS FOR STANDBY TIME  */
/******************************************************************************/
/* Configuration Register */
#define BME280_CONFIG_REG_TSB__POS                                  5
#define BME280_CONFIG_REG_TSB__MSK                                  0xE0
#define BME280_CONFIG_REG_TSB__LEN                                  3
#define BME280_CONFIG_REG_TSB__REG                                  BME280_CONFIG_REG
/******************************************************************************/
/*       BIT MASK, LENGTH AND POSITION DEFINITIONS FOR FILTER  */
/******************************************************************************/
#define BME280_CONFIG_REG_FILTER__POS                               2
#define BME280_CONFIG_REG_FILTER__MSK                               0x1C
#define BME280_CONFIG_REG_FILTER__LEN                               3
#define BME280_CONFIG_REG_FILTER__REG                               BME280_CONFIG_REG
/******************************************************************************/
/*         BIT MASK, LENGTH AND POSITION DEFINITIONS FOR SPI ENABLE  */
/******************************************************************************/
#define BME280_CONFIG_REG_SPI3_ENABLE__POS                          0
#define BME280_CONFIG_REG_SPI3_ENABLE__MSK                          0x01
#define BME280_CONFIG_REG_SPI3_ENABLE__LEN                          1
#define BME280_CONFIG_REG_SPI3_ENABLE__REG                          BME280_CONFIG_REG
/******************************************************************************/
/* BIT MASK, LENGTH AND POSITION DEFINITIONS FOR PRESSURE AND TEMPERATURE DATA  */
/******************************************************************************/
/* Data Register */
#define BME280_PRESSURE_XLSB_REG_DATA__POS                          4
#define BME280_PRESSURE_XLSB_REG_DATA__MSK                          0xF0
#define BME280_PRESSURE_XLSB_REG_DATA__LEN                          4
#define BME280_PRESSURE_XLSB_REG_DATA__REG                          BME280_PRESSURE_XLSB_REG

#define BME280_TEMPERATURE_XLSB_REG_DATA__POS                       4
#define BME280_TEMPERATURE_XLSB_REG_DATA__MSK                       0xF0
#define BME280_TEMPERATURE_XLSB_REG_DATA__LEN                       4
#define BME280_TEMPERATURE_XLSB_REG_DATA__REG                       BME280_TEMPERATURE_XLSB_REG

#define BME280_STANDARD_OVERSAMP_HUMIDITY 0x01

/** BME280 Sensor mode Tyep **/
typedef enum
{
    BME280_SLEEP_MODE = 0x00,
    BME280_FORCED_MODE = 0x01,
    BME280_NORMAL_MODE = 0x03
}BME280_Mode_Type;


/** BME280 Standby Type **/
typedef enum
{
    BME280_STANDBY_TIME_1_MS = 0x00,
    BME280_STANDBY_TIME_63_MS = 0x01,
    BME280_STANDBY_TIME_125_MS = 0x02,
    BME280_STANDBY_TIME_250_MS = 0x03,
    BME280_STANDBY_TIME_500_MS = 0x04,
    BME280_STANDBY_TIME_1000_MS = 0x05,
    BME280_STANDBY_TIME_10_MS = 0x06,
    BME280_STANDBY_TIME_20_MS = 0x07
}BME280_Standby_Type ;


/** BME280 Oversampling rate type **/
typedef enum {
    BME280_OVERSAMP_SKIPPED = 0x00,
    BME280_OVERSAMP_1X = 0x01,
    BME280_OVERSAMP_2X = 0x02,
    BME280_OVERSAMP_4X = 0x03,
    BME280_OVERSAMP_8X = 0x04,
    BME280_OVERSAMP_16X = 0x05
}BME280_Oversamp_Type;


/** BME280 Workmode Type **/
typedef enum
{
    BME280_ULTRALOWPOWER_MODE = 0x00,
    BME280_LOWPOWER_MODE = 0x01,
    BME280_STANDARDRESOLUTION_MODE = 0x02,
    BME280_HIGHRESOLUTION_MODE = 0x03,
    BME280_ULTRAHIGHRESOLUTION_MODE = 0x04,
}BME280_WorkMode_Type;


/** BME280 pressure workmode type **/
typedef enum
{
    BME280_ULTRALOWPOWER_OSRS_P = 0x01,
    BME280_LOWPOWER_OSRS_P = 0x02,
    BME280_STANDARDRESOLUTION_OSRS_P = 0x03,
    BME280_HIGHRESOLUTION_OSRS_P = 0x04,
    BME280_ULTRAHIGHRESOLUTION_OSRS_P = 0x05,
}BME280_Pressure_WorkMode_Type;


/** BME280 Temperature workmode type **/
typedef enum
{
    BME280_ULTRALOWPOWER_OSRS_T = 0x01,
    BME280_LOWPOWER_OSRS_T = 0x01,
    BME280_STANDARDRESOLUTION_OSRS_T = 0x01,
    BME280_HIGHRESOLUTION_OSRS_T = 0x01,
    BME280_ULTRAHIGHRESOLUTION_OSRS_T = 0x02,
}BME280_Temperature_WorkMode_Type;




/** BME280 Filter Type **/
typedef enum
{
    BME280_FILTER_COEFF_OFF = 0x00,
    BME280_FILTER_COEFF_2 = 0x01,
    BME280_FILTER_COEFF_4 = 0x02,
    BME280_FILTER_COEFF_8 = 0x03,
    BME280_FILTER_COEFF_16 = 0x04
}BME280_Filter_Coeff_Type;


/** BME280 Dataframe Type **/
typedef enum
{
    BME280_DATA_FRAME_PRESSURE_MSB_BYTE = 0,
    BME280_DATA_FRAME_PRESSURE_LSB_BYTE = 1,
    BME280_DATA_FRAME_PRESSURE_XLSB_BYTE = 2,
    BME280_DATA_FRAME_TEMPERATURE_MSB_BYTE = 3,
    BME280_DATA_FRAME_TEMPERATURE_LSB_BYTE = 4,
    BME280_DATA_FRAME_TEMPERATURE_XLSB_BYTE = 5,
    BME280_DATA_FRAME_HUMIDITY_MSB_BYTE = 6,
    BME280_DATA_FRAME_HUMIDITY_LSB_BYTE = 7
}BME280_DataFrame_Type;


/** BME280 Calibration Type**/
typedef enum
{
    BME280_TEMPERATURE_CALIB_DIG_T1_LSB = 0,
    BME280_TEMPERATURE_CALIB_DIG_T1_MSB = 1,
    BME280_TEMPERATURE_CALIB_DIG_T2_LSB = 2,
    BME280_TEMPERATURE_CALIB_DIG_T2_MSB = 3,
    BME280_TEMPERATURE_CALIB_DIG_T3_LSB = 4,
    BME280_TEMPERATURE_CALIB_DIG_T3_MSB = 5,
    BME280_PRESSURE_CALIB_DIG_P1_LSB = 6,
    BME280_PRESSURE_CALIB_DIG_P1_MSB = 7,
    BME280_PRESSURE_CALIB_DIG_P2_LSB = 8,
    BME280_PRESSURE_CALIB_DIG_P2_MSB = 9,
    BME280_PRESSURE_CALIB_DIG_P3_LSB = 10,
    BME280_PRESSURE_CALIB_DIG_P3_MSB = 11,
    BME280_PRESSURE_CALIB_DIG_P4_LSB = 12,
    BME280_PRESSURE_CALIB_DIG_P4_MSB = 13,
    BME280_PRESSURE_CALIB_DIG_P5_LSB = 14,
    BME280_PRESSURE_CALIB_DIG_P5_MSB = 15,
    BME280_PRESSURE_CALIB_DIG_P6_LSB = 16,
    BME280_PRESSURE_CALIB_DIG_P6_MSB = 17,
    BME280_PRESSURE_CALIB_DIG_P7_LSB = 18,
    BME280_PRESSURE_CALIB_DIG_P7_MSB = 19,
    BME280_PRESSURE_CALIB_DIG_P8_LSB = 20,
    BME280_PRESSURE_CALIB_DIG_P8_MSB = 21,
    BME280_PRESSURE_CALIB_DIG_P9_LSB = 22,
    BME280_PRESSURE_CALIB_DIG_P9_MSB = 23,
    BME280_HUMIDITY_CALIB_DIG_H1 = 25,

    BME280_HUMIDITY_CALIB_DIG_H2_LSB = 0,
    BME280_HUMIDITY_CALIB_DIG_H2_MSB = 1,
    BME280_HUMIDITY_CALIB_DIG_H3 = 2,
    BME280_HUMIDITY_CALIB_DIG_H4_MSB = 3,
    BME280_HUMIDITY_CALIB_DIG_H4_LSB = 4,
    BME280_HUMIDITY_CALIB_DIG_H5_MSB = 5,
    BME280_HUMIDITY_CALIB_DIG_H6 = 6,
    BME280_MASK_DIG_H4 = 0x0F
}BME280_CalibReg_type;


/******************************************************************************/
/*  @brief This structure holds all device specific calibration parameters */
/******************************************************************************/
typedef struct bme280_calibration_param_t {
  unsigned int dig_T1;
  int dig_T2;
  int dig_T3;

  unsigned int dig_P1;
  int  dig_P2;
  int dig_P3;
  int dig_P4;
  int dig_P5;
  int dig_P6;
  int dig_P7;
  int dig_P8;
  int dig_P9;

  char dig_H1;
  int dig_H2;
  char dig_H3;
  int dig_H4;
  int dig_H5;
  short dig_H6;
}BME280_calibration_param;
/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_I2C_BME280_Public_Functions
 * @{
 */
void BSP_BME280_I2C1_Init(void);

uint8_t BME280_GetID(void);
void BME280_SoftReset(void);
uint8_t BME280_GetID(void);

void BME280_SoftReset(void);
uint8_t BME280_GetStatus(void);
void BME280_ReadData(void);
uint8_t BME280_GetConfig(void);
uint8_t BME280_GetCtrlMeasurement(void);
uint8_t BME280_GetCtrlHumidity(void);
void BME280_ReadCalibrationParams(void);
uint8_t BME280_ReadMeasurements(void);
void BME280_SetOversamplingPressure(uint8_t Value);
void BME280_SetOversamplingTemperature(uint8_t Value);
void BME280_SetOversamplingHumidity(uint8_t Value);
void BME280_SetPowerMode(uint8_t Value);
void BME280_SetFilterCoefficient(uint8_t Value);
void BME280_SetStandbyTime(uint8_t Value);
unsigned char BME280_IsMeasuring(void);
void BME280_INIT(void);
void BME280_GetTemperature(void);
void BME280_GetHumidity(void);
void BME280_GetPressure(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /*__STM32F4_I2C_BME280_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
