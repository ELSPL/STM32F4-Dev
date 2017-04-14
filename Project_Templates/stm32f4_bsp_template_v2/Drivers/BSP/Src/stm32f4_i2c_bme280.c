/******************************************************************//**
* @file		stm32f4_i2c_bme280.c
* @brief	Contains all functions support for I2C based BME280
*           Pressure and Temperature Sensor library
* @version	1.0
* @date		23. Jan. 2017
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup BME280
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_i2c_bme280.h"


/** @defgroup STM32F4_I2C_BME280_Exported_Handle
 * @{
 */
I2C_HandleTypeDef hi2c1_bme280;

/**
  * @}
  */

BME280_calibration_param cal_param;
int32_t adc_t, adc_p, adc_h,t_fine;

/** @defgroup STM32F4_I2C_BME280_Private_Functions
  * @{
  */

/**
 * @brief This function is support function for BSP_BME280_I2C1_Init
 *        This function configures GPIO of I2C1
 * @param hi2c  Provide I2C handle
 *      @arg  &hi2c1_bme280
 * @retval None
 */
static void BME280_I2C1_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();

    /* Peripheral interrupt init*/
//    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
//    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  }
}


/**
 * @brief To transmit data on I2C
 * @param Wr_Addr Register address on which Wr_Data will be written
 * @param Wr_Data data
 * @return  value   if data written successful, it will return 0 otherwise 1
 */
static uint8_t I2C_WriteRegister(uint8_t Wr_Addr, uint8_t Wr_Data)
{
  uint8_t I2C_Tx_Buf[2]={0};

  I2C_Tx_Buf[0] = Wr_Addr;
  I2C_Tx_Buf[1] = Wr_Data;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_bme280,BME280_I2C_ADDRESS1,I2C_Tx_Buf,2,2000) == HAL_OK)
  {
    return (0);
  }
  else
  {
    return (1);
  }
}

/**
 * @brief To read data from the I2C based device
 * @param Rd_Addr Register address from data to be read
 * @return value  If data read successful, it will return data otherwise 1
 */
static uint8_t I2C_ReadRegister(uint8_t Rd_Addr)
{
  uint8_t I2C_Tx_Buf[2]={0};
  uint8_t I2C_Rx_Buf[2]={0};

  I2C_Tx_Buf[0] = Rd_Addr;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_bme280,BME280_I2C_ADDRESS1,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_bme280,BME280_I2C_ADDRESS1,I2C_Rx_Buf,1,2000) == HAL_OK)
  {
    return (I2C_Rx_Buf[0]);
  }
  else
  {
    return (1);
  }
}

/**
  * @}
  */


/** @defgroup STM32F4_I2C_BME280_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes I2C1
 * @retval None
 */
void BSP_BME280_I2C1_Init(void)
{
  __GPIOB_CLK_ENABLE();

  hi2c1_bme280.Instance = I2C1;
  hi2c1_bme280.Init.ClockSpeed = 100000;
  hi2c1_bme280.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1_bme280.Init.OwnAddress1 = 0;
  hi2c1_bme280.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_bme280.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1_bme280.Init.OwnAddress2 = 0;
  hi2c1_bme280.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1_bme280.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  BME280_I2C1_MspInit(&hi2c1_bme280);
  HAL_I2C_Init(&hi2c1_bme280);
}

/**
 * @brief This function is for De-Initializing I2C1
 * @param hi2c Provide I2C handle
 *      @arg  &hi2c1_bme280
 * @retval None
 */
void BSP_BME280_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
//    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
//    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
  }
}


/**
 * @brief To Read the Device ID
 * @return Device ID
 */
uint8_t BME280_GetID(void)
{
    return I2C_ReadRegister(BME280_CHIP_ID_REG);
}

/**
 * @brief To Reset the Device
 */
void BME280_SoftReset(void)
{
  I2C_WriteRegister(BME280_RST_REG, BME280_SOFT_RESET);
}

/**
 * @brief To get the device status
 * @return  status register value
 */
uint8_t BME280_GetStatus(void)
{
  return I2C_ReadRegister(BME280_STAT_REG);
}


/**
 * @brief To get the Temperature and Pressure CTRL_MEAS register value
 * @return  CTRL_MEAS register value
 */
uint8_t BME280_GetCtrlMeasurement(void)
{
  return I2C_ReadRegister(BME280_CTRL_MEAS_REG);
}


/**
 * @brief To read humidity CTRL_MEAS register value
 * @return CTRL_MEAS register value
 */
uint8_t BME280_GetCtrlHumidity(void)
{
  return I2C_ReadRegister(BME280_CTRL_HUMIDITY_REG);
}


/**
 * @brief To get the config register value
 * @return  Config register value
 */
uint8_t BME280_GetConfig(void)
{
  return I2C_ReadRegister(BME280_CONFIG_REG);
}


/**
 * @brief To read the sensor data(Temperature,Pressure and humidity)
 * @return value    if fail to read sensor data then it will return 1
 */
uint8_t BME280_ReadMeasurements(void)
{
    uint8_t I2C_Tx_Buf[1] = {BME280_PRESSURE_MSB_REG};
    uint8_t I2C_Rx_Buf[8] = {0};

    /* write byte to addr  */
    if(HAL_I2C_Master_Transmit(&hi2c1_bme280,BME280_I2C_ADDRESS1,I2C_Tx_Buf,1,2000) != HAL_OK)
    {
      return 1;
    }

    if (HAL_I2C_Master_Receive(&hi2c1_bme280,BME280_I2C_ADDRESS1,I2C_Rx_Buf,8,2000) == HAL_OK)
    {
        adc_h = I2C_Rx_Buf[BME280_DATA_FRAME_HUMIDITY_LSB_BYTE];
        adc_h |= (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_HUMIDITY_MSB_BYTE] << 8;

        adc_t  = (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_TEMPERATURE_XLSB_BYTE] >> 4;
        adc_t |= (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_TEMPERATURE_LSB_BYTE] << 4;
        adc_t |= (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_TEMPERATURE_MSB_BYTE] << 12;

        adc_p  = (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_PRESSURE_XLSB_BYTE] >> 4;
        adc_p |= (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_PRESSURE_LSB_BYTE] << 4;
        adc_p |= (uint32_t)I2C_Rx_Buf[BME280_DATA_FRAME_PRESSURE_MSB_BYTE] << 12;
    }
    else
    {
        return (1);
    }
}


/**
 * @brief To Read the calibration data for Temperature, Pressure and humidity
 */
void BME280_ReadCalibrationParams(void)
{
   uint8_t lsb, msb;

   msb = I2C_ReadRegister(BME280_TEMPERATURE_CALIB_DIG_T1_MSB_REG);
   cal_param.dig_T1 = (uint16_t) msb;
   lsb = I2C_ReadRegister(BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG);
   cal_param.dig_T1 = (cal_param.dig_T1 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_TEMPERATURE_CALIB_DIG_T2_MSB_REG);
   cal_param.dig_T2 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_TEMPERATURE_CALIB_DIG_T2_LSB_REG);
   cal_param.dig_T2 = (cal_param.dig_T2 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_TEMPERATURE_CALIB_DIG_T3_MSB_REG);
   cal_param.dig_T3 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_TEMPERATURE_CALIB_DIG_T3_LSB_REG);
   cal_param.dig_T3 = (cal_param.dig_T3 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P1_MSB_REG);
   cal_param.dig_P1 = (uint16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P1_LSB_REG);
   cal_param.dig_P1 = (cal_param.dig_P1 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P2_MSB_REG);
   cal_param.dig_P2 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P2_LSB_REG);
   cal_param.dig_P2 = (cal_param.dig_P2 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P3_MSB_REG);
   cal_param.dig_P3 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P3_LSB_REG);
   cal_param.dig_P3 = (cal_param.dig_P3 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P4_MSB_REG);
   cal_param.dig_P4 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P4_LSB_REG);
   cal_param.dig_P4 = (cal_param.dig_P4 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P5_MSB_REG);
   cal_param.dig_P5 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P5_LSB_REG);
   cal_param.dig_P5 = (cal_param.dig_P5 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P6_MSB_REG);
   cal_param.dig_P6 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P6_LSB_REG);
   cal_param.dig_P6 = (cal_param.dig_P6 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P7_MSB_REG);
   cal_param.dig_P7 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P7_LSB_REG);
   cal_param.dig_P7 = (cal_param.dig_P7 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P8_MSB_REG);
   cal_param.dig_P8 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P8_LSB_REG);
   cal_param.dig_P8 = (cal_param.dig_P8 << 8) + lsb;


   msb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P9_MSB_REG);
   cal_param.dig_P9 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_PRESSURE_CALIB_DIG_P9_LSB_REG);
   cal_param.dig_P9 = (cal_param.dig_P9 << 8) + lsb;

   lsb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H1_REG);
   cal_param.dig_H1 = (uint8_t) lsb;


   msb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H2_MSB_REG);
   cal_param.dig_H2 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H2_LSB_REG);
   cal_param.dig_H2 = (cal_param.dig_H2 << 8) + lsb;

   lsb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H3_REG);
   cal_param.dig_H3 = (uint8_t) lsb;


   msb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H4_MSB_REG);
   cal_param.dig_H4 = (int16_t) msb;
   lsb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H4_LSB_REG);
   cal_param.dig_H4 = (cal_param.dig_H4 << 4) | (lsb & 0xF);

   msb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H5_MSB_REG);
   cal_param.dig_H5 = (int16_t) msb;
   cal_param.dig_H5 = (cal_param.dig_H5 << 4) | (lsb >> 4);

   lsb = I2C_ReadRegister(BME280_HUMIDITY_CALIB_DIG_H6_REG);
   cal_param.dig_H6 = (int8_t) lsb;
}


/**
 * @brief To set the sampling rate for the Pressure data
 * @param Value
 */
void BME280_SetOversamplingPressure(uint8_t Value)
{
    uint8_t ctrlm;

    ctrlm = BME280_GetCtrlMeasurement();
    ctrlm &= ~BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__MSK;
    ctrlm |= Value << BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__POS;

    I2C_WriteRegister(BME280_CTRL_MEAS_REG, ctrlm);
    ctrlm = BME280_GetCtrlMeasurement();
}


/**
 * @brief To set the sampling rate for the Temperature data
 * @param Value
 */
void BME280_SetOversamplingTemperature(uint8_t Value)
{
    int8_t ctrlm;

    ctrlm = BME280_GetCtrlMeasurement();
    ctrlm &= ~BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__MSK;
    ctrlm |= Value << BME280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__POS;

    I2C_WriteRegister(BME280_CTRL_MEAS_REG, ctrlm);
}


/**
 * @brief To set the sampling rate for the Humidity data
 * @param Value
 */
void BME280_SetOversamplingHumidity(uint8_t Value)
{
    I2C_WriteRegister(BME280_CTRL_HUMIDITY_REG, Value );
}


/**
 * @brief To set the sampling mode
 * @param Value Oversampling mode
 *          @arg BME280_NORMAL_MODE
 *          @arg BME280_FORCED_MODE
 *          @arg BME280_SLEEP_MODE
 */
void BME280_SetOversamplingMode(BME280_Mode_Type Value)
{
    uint8_t ctrlm;

    ctrlm = BME280_GetCtrlMeasurement();
    ctrlm |= Value;

    I2C_WriteRegister(BME280_CTRL_MEAS_REG, ctrlm);
}


/**
 * @brief To set the filter coefficient value
 * @param Value Filter cofficient
 *          @arg    BME280_FILTER_COEFF_OFF
 *          @arg    BME280_FILTER_COEFF_2
 *          @arg    BME280_FILTER_COEFF_4
 *          @arg    BME280_FILTER_COEFF_8
 *          @arg    BME280_FILTER_COEFF_16
 */
void BME280_SetFilterCoefficient(BME280_Filter_Coeff_Type Value)
{
    uint8_t cfgv;

    cfgv = BME280_GetConfig();
    cfgv &= ~BME280_CONFIG_REG_FILTER__MSK;
    cfgv |= Value << BME280_CONFIG_REG_FILTER__POS;
}


/**
 * @brief To set standby time
 * @param Value standny time
 *          @arg    BME280_STANDBY_TIME_1_MS
 *          @arg    BME280_STANDBY_TIME_63_MS
 *          @arg    BME280_STANDBY_TIME_125_MS
 *          @arg    BME280_STANDBY_TIME_250_MS
 *          @arg    BME280_STANDBY_TIME_500_MS
 *          @arg    BME280_STANDBY_TIME_1000_MS
 *          @arg    BME280_STANDBY_TIME_10_MS
 *          @arg    BME280_STANDBY_TIME_20_MS
 */
void BME280_SetStandbyTime(BME280_Standby_Type Value)
{
    uint8_t cfgv;

    cfgv = BME280_GetConfig();
    cfgv &= ~BME280_CONFIG_REG_TSB__MSK;
    cfgv |= Value << BME280_CONFIG_REG_TSB__POS;
}


/**
 * @brief To check the conversion is running or not
 * @return  value
 */
uint8_t BME280_IsMeasuring(void)
{
    uint8_t output;

    output = BME280_GetStatus();
    return (output & BME280_STAT_REG_MEASURING__MSK);
}



/**
 * @brief Temperature calculation
 * @return temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
 */
static int32_t BME280_Compensate_T(void)
{
  int32_t temp1, temp2, T;

  temp1 = ((((adc_t>>3) -((int32_t)cal_param.dig_T1<<1))) * ((int32_t)cal_param.dig_T2)) >> 11;
  temp2 = (((((adc_t>>4) - ((int32_t)cal_param.dig_T1)) * ((adc_t>>4) - ((int32_t)cal_param.dig_T1))) >> 12) * ((int32_t)cal_param.dig_T3)) >> 14;
  t_fine = temp1 + temp2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}


/**
 * @brief Humidity calculation
 * @return humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
 *         Output value of “47445” represents 47445/1024 = 46.333 %RH
 */
static uint32_t BME280_Compensate_H()
{
    int32_t h1;
    h1 = (t_fine - ((int32_t)76800));
    h1 = (((((adc_h << 14) - (((int32_t)cal_param.dig_H4) << 20) - (((int32_t)cal_param.dig_H5) * h1)) +
    ((int32_t)16384)) >> 15) * (((((((h1 * ((int32_t)cal_param.dig_H6)) >> 10) * (((h1 *
    ((int32_t)cal_param.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
    ((int32_t)cal_param.dig_H2) + 8192) >> 14));
    h1 = (h1 - (((((h1 >> 15) * (h1 >> 15)) >> 7) * ((int32_t)cal_param.dig_H1)) >> 4));
    h1 = (h1 < 0 ? 0 : h1);
    h1 = (h1 > 419430400 ? 419430400 : h1);
  return (int32_t)(h1>>12);
}


/**
 * @brief Pressure calculation
 * @return pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
 */
static uint32_t BME280_Compensate_P()
{
    int32_t press1, press2;
    uint32_t P;

    press1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
    press2 = (((press1>>2) * (press1>>2)) >> 11 ) * ((int32_t)cal_param.dig_P6);
    press2 = press2 + ((press1*((int32_t)cal_param.dig_P5))<<1);
    press2 = (press2>>2)+(((int32_t)cal_param.dig_P4)<<16);
    press1 = (((cal_param.dig_P3 * (((press1>>2) * (press1>>2)) >> 13 )) >> 3) + ((((int32_t)cal_param.dig_P2) * press1)>>1))>>18;
    press1 =((((32768+press1))*((int32_t)cal_param.dig_P1))>>15);
    if (press1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    P = (((uint32_t)(((int32_t)1048576)-adc_p)-(press2>>12)))*3125;
    if (P < 0x80000000)
    {
        P = (P << 1) / ((uint32_t)press1);
    }
    else
    {
        P = (P / (uint32_t)press1) * 2;
    }
    press1 = (((int32_t)cal_param.dig_P9) * ((int32_t)(((P>>3) * (P>>3))>>13)))>>12;
    press2 = (((int32_t)(P>>2)) * ((int32_t)cal_param.dig_P8))>>13;
    P = (uint32_t)((int32_t)P + ((press1 + press2 + cal_param.dig_P7) >> 4));
    return P;
}


/**
 * @brief initialize BME280
 */
void BME280_INIT(void)
{
  BME280_SetStandbyTime(BME280_STANDBY_TIME_1_MS);                              // Standby time 1ms
  BME280_SetFilterCoefficient(BME280_FILTER_COEFF_16);                          // IIR Filter coefficient 16
  BME280_SetOversamplingPressure(BME280_OVERSAMP_16X);                          // Pressure x16 oversampling
  BME280_SetOversamplingTemperature(BME280_OVERSAMP_2X);                        // Temperature x2 oversampling
  BME280_SetOversamplingHumidity(BME280_OVERSAMP_1X);                           // Humidity x1 oversampling
  BME280_SetOversamplingMode(BME280_NORMAL_MODE);
}


/**
 * @brief Print Temperature value on Uart0
 */
void BME280_GetTemperature(void)
{
    uint16_t temp=0,intdata1=0,intdata2=0;

    temp =BME280_Compensate_T();
    intdata1 = temp/100;
    intdata2 = temp%100;
    uprintf(&huart4,"\x1b[3;1H Temperature(DegC): %d04.%d02\r\n",intdata1,intdata2);
}


/**
 * @brief Print Humidity value on Uart0
 */
void BME280_GetHumidity(void)
{
    uint16_t temp=0,intdata1=0,intdata2=0;

    temp = BME280_Compensate_H();
    intdata1 = temp/1024;
    intdata2 = temp%1024;
    uprintf(&huart4,"\x1b[5;1H Humidity(%%RH): %d04.%d03\r\n",intdata1,intdata2);
}

/**
 * @brief Print Pressure value on Uart0
 */
void BME280_GetPressure(void)
{
    uint16_t temp=0,intdata1=0,intdata2=0;

    temp = BME280_Compensate_P();
    intdata1 = temp/100;
    intdata2 = temp%100;
    uprintf(&huart4,"\x1b[4;1H Pressure(hPa): %d04.%d02\r\n",intdata1,intdata2);
 }


/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

