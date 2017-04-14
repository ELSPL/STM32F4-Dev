/******************************************************************//**
* @file		stm32f4_i2c_bmp280.c
* @brief	Contains all functions support for I2C based BMP280
*           Pressure and Temperature Sensor library
* @version	1.0
* @date		23. Jan. 2017
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup BMP280
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_i2c_bmp280.h"


/** @defgroup STM32F4_I2C_BMP280_Exported_Handle
 * @{
 */
I2C_HandleTypeDef hi2c1_bmp280;

/**
  * @}
  */

BMP280_calibration_param cal_param;
int32_t adc_t, adc_p, t_fine;

/** @defgroup STM32F4_I2C_BMP280_Private_Functions
  * @{
  */

/**
 * @brief This function is support function for BSP_BMP280_I2C1_Init
 *        This function configures GPIO of I2C1
 * @param hi2c  Provide I2C handle
 *      @arg  &hi2c1_bmp280
 * @retval None
 */
static void BMP280_I2C1_MspInit(I2C_HandleTypeDef* hi2c)
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
  if(HAL_I2C_Master_Transmit(&hi2c1_bmp280,BMP280_I2C_ADDRESS2,I2C_Tx_Buf,2,2000) == HAL_OK)
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
  if(HAL_I2C_Master_Transmit(&hi2c1_bmp280,BMP280_I2C_ADDRESS2,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_bmp280,BMP280_I2C_ADDRESS2,I2C_Rx_Buf,1,2000) == HAL_OK)
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


/** @defgroup STM32F4_I2C_BMP280_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes I2C1
 * @retval None
 */
void BSP_BMP280_I2C1_Init(void)
{
  __GPIOB_CLK_ENABLE();

  hi2c1_bmp280.Instance = I2C1;
  hi2c1_bmp280.Init.ClockSpeed = 100000;
  hi2c1_bmp280.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1_bmp280.Init.OwnAddress1 = 0;
  hi2c1_bmp280.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_bmp280.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1_bmp280.Init.OwnAddress2 = 0;
  hi2c1_bmp280.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1_bmp280.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  BMP280_I2C1_MspInit(&hi2c1_bmp280);
  HAL_I2C_Init(&hi2c1_bmp280);
}

/**
 * @brief This function is for De-Initializing I2C1
 * @param hi2c Provide I2C handle
 *      @arg  &hi2c1_bmp280
 * @retval None
 */
void BSP_BMP280_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
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
uint8_t BMP280_GetID(void)
{
  return I2C_ReadRegister(BMP280_CHIP_ID_REG);
}

/**
 * @brief To Reset the Device
 */
void BMP280_SoftReset(void)
{
  I2C_WriteRegister(BMP280_RST_REG, BMP280_SOFT_RESET_CODE);
}

/**
 * @brief To get the device status
 * @return  status register value
 */
uint8_t BMP280_GetStatus(void)
{
  return I2C_ReadRegister(BMP280_STAT_REG);
}


/**
 * @brief To get the Temperature and Pressure CTRL_MEAS register value
 * @return  CTRL_MEAS register value
 */
uint8_t BMP280_GetCtrlMeasurement(void)
{
  return I2C_ReadRegister(BMP280_CTRL_MEAS_REG);
}


/**
 * @brief To get the config register value
 * @return  Config register value
 */
uint8_t BMP280_GetConfig(void)
{
  return I2C_ReadRegister(BMP280_CONFIG_REG);
}

/**
 * @brief To read the sensor data(Temperature,Pressure and humidity)
 * @return value  if fail to read sensor data then it will return 1
 */
uint8_t BMP280_ReadMeasurements(void)
{
  uint8_t I2C_Tx_Buf[1] = {BMP280_PRESSURE_MSB_REG};
  uint8_t I2C_Rx_Buf[6] = {0};

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_bmp280,BMP280_I2C_ADDRESS2,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_bmp280,BMP280_I2C_ADDRESS2,I2C_Rx_Buf,8,2000) == HAL_OK)
  {
    adc_t  = (uint32_t)I2C_Rx_Buf[BMP280_DATA_FRAME_TEMPERATURE_XLSB_BYTE] >> 4;
    adc_t |= (uint32_t)I2C_Rx_Buf[BMP280_DATA_FRAME_TEMPERATURE_LSB_BYTE] << 4;
    adc_t |= (uint32_t)I2C_Rx_Buf[BMP280_DATA_FRAME_TEMPERATURE_MSB_BYTE] << 12;

    adc_p  = (uint32_t)I2C_Rx_Buf[BMP280_DATA_FRAME_PRESSURE_XLSB_BYTE] >> 4;
    adc_p |= (uint32_t)I2C_Rx_Buf[BMP280_DATA_FRAME_PRESSURE_LSB_BYTE] << 4;
    adc_p |= (uint32_t)I2C_Rx_Buf[BMP280_DATA_FRAME_PRESSURE_MSB_BYTE] << 12;
  }
  else
  {
    return (1);
  }
}


/**
 * @brief To Read the calibration data for Temperature, Pressure and humidity
 */
void BMP280_ReadCalibrationParams(void)
{
   uint8_t lsb, msb;

   msb = I2C_ReadRegister(BMP280_TEMPERATURE_CALIB_DIG_T1_MSB_REG);
   cal_param.dig_T1 = (uint16_t) msb;
   lsb = I2C_ReadRegister(BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG);
   cal_param.dig_T1 = (cal_param.dig_T1 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_TEMPERATURE_CALIB_DIG_T2_MSB_REG);
   cal_param.dig_T2 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_TEMPERATURE_CALIB_DIG_T2_LSB_REG);
   cal_param.dig_T2 = (cal_param.dig_T2 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_TEMPERATURE_CALIB_DIG_T3_MSB_REG);
   cal_param.dig_T3 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_TEMPERATURE_CALIB_DIG_T3_LSB_REG);
   cal_param.dig_T3 = (cal_param.dig_T3 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P1_MSB_REG);
   cal_param.dig_P1 = (uint16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P1_LSB_REG);
   cal_param.dig_P1 = (cal_param.dig_P1 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P2_MSB_REG);
   cal_param.dig_P2 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P2_LSB_REG);
   cal_param.dig_P2 = (cal_param.dig_P2 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P3_MSB_REG);
   cal_param.dig_P3 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P3_LSB_REG);
   cal_param.dig_P3 = (cal_param.dig_P3 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P4_MSB_REG);
   cal_param.dig_P4 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P4_LSB_REG);
   cal_param.dig_P4 = (cal_param.dig_P4 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P5_MSB_REG);
   cal_param.dig_P5 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P5_LSB_REG);
   cal_param.dig_P5 = (cal_param.dig_P5 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P6_MSB_REG);
   cal_param.dig_P6 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P6_LSB_REG);
   cal_param.dig_P6 = (cal_param.dig_P6 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P7_MSB_REG);
   cal_param.dig_P7 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P7_LSB_REG);
   cal_param.dig_P7 = (cal_param.dig_P7 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P8_MSB_REG);
   cal_param.dig_P8 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P8_LSB_REG);
   cal_param.dig_P8 = (cal_param.dig_P8 << 8) + lsb;


   msb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P9_MSB_REG);
   cal_param.dig_P9 = (int16_t) msb;
   lsb = I2C_ReadRegister(BMP280_PRESSURE_CALIB_DIG_P9_LSB_REG);
   cal_param.dig_P9 = (cal_param.dig_P9 << 8) + lsb;
}


/**
 * @brief To set the sampling rate for the Pressure data
 * @param Value
 */
void BMP280_SetOversamplingPressure(uint8_t Value)
{
  uint8_t ctrlm;

  ctrlm = BMP280_GetCtrlMeasurement();
  ctrlm &= ~BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__MSK;
  ctrlm |= Value << BMP280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__POS;

  I2C_WriteRegister(BMP280_CTRL_MEAS_REG, ctrlm);
  ctrlm = BMP280_GetCtrlMeasurement();
}


/**
 * @brief To set the sampling rate for the Temperature data
 * @param Value
 */
void BMP280_SetOversamplingTemperature(uint8_t Value)
{
  int8_t ctrlm;

  ctrlm = BMP280_GetCtrlMeasurement();
  ctrlm &= ~BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__MSK;
  ctrlm |= Value << BMP280_CTRL_MEAS_REG_OVERSAMP_TEMPERATURE__POS;

  I2C_WriteRegister(BMP280_CTRL_MEAS_REG, ctrlm);
}


/**
 * @brief To set the sensor power mode
 * @param Value Power mode
 *      @arg BMP280_NORMAL_MODE
 *      @arg BMP280_FORCED_MODE
 *      @arg BMP280_SLEEP_MODE
 */
void BMP280_SetPowerMode(BMP280_Mode_Type Value)
{
  uint8_t ctrlm;

  ctrlm = BMP280_GetCtrlMeasurement();
  ctrlm |= Value;

  I2C_WriteRegister(BMP280_CTRL_MEAS_REG, ctrlm);
}


/**
 * @brief To set the filter coefficient value
 * @param Value Filter cofficient
 *      @arg  BMP280_FILTER_COEFF_OFF
 *      @arg  BMP280_FILTER_COEFF_2
 *      @arg  BMP280_FILTER_COEFF_4
 *      @arg  BMP280_FILTER_COEFF_8
 *      @arg  BMP280_FILTER_COEFF_16
 */
void BMP280_SetFilterCoefficient(BMP280_Filter_Coeff_Type Value)
{
  uint8_t cfgv;

  cfgv = BMP280_GetConfig();
  cfgv &= ~BMP280_CONFIG_REG_FILTER__MSK;
  cfgv |= Value << BMP280_CONFIG_REG_FILTER__POS;
}


/**
 * @brief To set standby time
 * @param Value standny time
 *      @arg  BMP280_STANDBY_TIME_1_MS
 *      @arg  BMP280_STANDBY_TIME_63_MS
 *      @arg  BMP280_STANDBY_TIME_125_MS
 *      @arg  BMP280_STANDBY_TIME_250_MS
 *      @arg  BMP280_STANDBY_TIME_500_MS
 *      @arg  BMP280_STANDBY_TIME_1000_MS
 *      @arg  BMP280_STANDBY_TIME_10_MS
 *      @arg  BMP280_STANDBY_TIME_20_MS
 */
void BMP280_SetStandbyTime(BMP280_Standby_Type Value)
{
  uint8_t cfgv;

  cfgv = BMP280_GetConfig();
  cfgv &= ~BMP280_CONFIG_REG_STANDBY_DURN__MSK;
  cfgv |= Value << BMP280_CONFIG_REG_STANDBY_DURN__POS;
}


/**
 * @brief To check the conversion is running or not
 * @return  value
 */
uint8_t BMP280_IsMeasuring(void)
{
  uint8_t output;

  output = BMP280_GetStatus();
  return (output & BMP280_STATUS_REG_MEASURING__MSK);
}



/**
 * @brief Temperature calculation
 * @return temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
 */
static int32_t BMP280_Compensate_T(void)
{
  int32_t temp1, temp2, T;

  temp1 = ((((adc_t>>3) -((int32_t)cal_param.dig_T1<<1))) * ((int32_t)cal_param.dig_T2)) >> 11;
  temp2 = (((((adc_t>>4) - ((int32_t)cal_param.dig_T1)) * ((adc_t>>4) - ((int32_t)cal_param.dig_T1))) >> 12) * ((int32_t)cal_param.dig_T3)) >> 14;
  t_fine = temp1 + temp2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}


/**
 * @brief Pressure calculation
 * @return pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
 */
static uint32_t BMP280_Compensate_P()
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
 * @brief initialize BMP280
 */
void BMP280_INIT(void)
{
  BMP280_SetStandbyTime(BMP280_STANDBY_TIME_1_MS);                              // Standby time 1ms
  BMP280_SetFilterCoefficient(BMP280_FILTER_COEFF_16);                          // IIR Filter coefficient 16
  BMP280_SetOversamplingPressure(BMP280_OVERSAMP_16X);                          // Pressure x16 oversampling
  BMP280_SetOversamplingTemperature(BMP280_OVERSAMP_2X);                        // Temperature x2 oversampling
  BMP280_SetPowerMode(BMP280_NORMAL_MODE);
}


/**
 * @brief Print Temperature value on Uart0
 */
void BMP280_GetTemperature(void)
{
  uint16_t temp=0,intdata1=0,intdata2=0;

  temp =BMP280_Compensate_T();
  intdata1 = temp/100;
  intdata2 = temp%100;
  uprintf(&huart4,"\x1b[2;1H Temperature(DegC): %d04.%d02",intdata1,intdata2);
}


/**
 * @brief Print Pressure value on Uart0
 */
void BMP280_GetPressure(void)
{
  uint16_t temp=0,intdata1=0,intdata2=0;

  temp = BMP280_Compensate_P();
  intdata1 = temp/100;
  intdata2 = temp%100;
  uprintf(&huart4,"\t Pressure(hPa): %d04.%d02\r\n",intdata1,intdata2);
 }
/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

