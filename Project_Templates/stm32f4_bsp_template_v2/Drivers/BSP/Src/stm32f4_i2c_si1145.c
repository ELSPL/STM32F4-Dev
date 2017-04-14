/******************************************************************//**
* @file		stm32f4_i2c_si1145.c
* @brief	Contains all functions support for I2C based SI1145
*           sunlinght Sensor library on LPC17xx
* @version	1.0
* @date		18. JUL. 2016
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup SI1145
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_i2c_si1145.h"

/** @defgroup STM32F4_I2C_SI1145_Exported_Handle
 * @{
 */
I2C_HandleTypeDef hi2c1_si1145;

/**
  * @}
  */

/** @defgroup STM32F4_I2C_SI1145_Private_Functions
  * @{
  */

/**
 * @brief This function is support function for BSP_Si1145_I2C1_Init
 *        This function configures GPIO of I2C1
 * @param hi2c  Provide I2C handle
 *      @arg  &hi2c1_si1145
 * @retval None
 */
static void Si1145_I2C1_MspInit(I2C_HandleTypeDef* hi2c)
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
  * @}
  */


/** @defgroup STM32F4_I2C_SI1145_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes I2C1
 * @retval None
 */
void BSP_Si1145_I2C1_Init(void)
{
  __GPIOB_CLK_ENABLE();

  hi2c1_si1145.Instance = I2C1;
  hi2c1_si1145.Init.ClockSpeed = 100000;
  hi2c1_si1145.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1_si1145.Init.OwnAddress1 = 0;
  hi2c1_si1145.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_si1145.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1_si1145.Init.OwnAddress2 = 0;
  hi2c1_si1145.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1_si1145.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  Si1145_I2C1_MspInit(&hi2c1_si1145);
  HAL_I2C_Init(&hi2c1_si1145);
}

/**
 * @brief This function is for De-Initializing I2C1
 * @param hi2c Provide I2C handle
 *      @arg  &hi2c1_si1145
 * @retval None
 */
void BSP_Si1145_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
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
 * @brief To transmit data on I2C
 * @param Wr_Addr	Register address on which Wr_Data will be written
 * @param Wr_Data	data
 * @return	value 	if data written successful, it will return 0 otherwise 1
 */
static uint8_t I2C_WriteRegister(uint8_t Wr_Addr, uint8_t Wr_Data)
{
  uint8_t I2C_Tx_Buf[2]={0};

  I2C_Tx_Buf[0] = Wr_Addr;
  I2C_Tx_Buf[1] = Wr_Data;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_si1145,SI1145_ADDRESS,I2C_Tx_Buf,2,2000) == HAL_OK)
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
 * @param Rd_Addr	Register address from data to be read
 * @return value 	If data read successful, it will return 0 otherwise 1
 */
static uint8_t I2C_ReadRegister(char Rd_Addr)
{
  uint8_t I2C_Tx_Buf[2]={0};
  uint8_t I2C_Rx_Buf[2]={0};

  I2C_Tx_Buf[0] = Rd_Addr;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_si1145,SI1145_ADDRESS,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_si1145,SI1145_ADDRESS,I2C_Rx_Buf,1,2000) == HAL_OK)
  {
    return (I2C_Rx_Buf[0]);
  }
  else
  {
    return (1);
  }
}

/**
 * @brief Write parameter data to register
 * @param Reg	enter register address
 * @param Value	register value
 * @return	register value
 */
static uint8_t WriteParamData(uint8_t Reg,uint8_t Value)
{
  //write Value into PARAMWR reg first
  I2C_WriteRegister(SI1145_WR, Value);
  I2C_WriteRegister(SI1145_COMMAND, Reg | SI1145_SET);
  //SI1145 writes value out to PARAM_RD,read and confirm its right
  return I2C_ReadRegister(SI1145_RD);
}

/**
 * @brief Reinitialize the sensor and set the register value to read the proper data
 */
void SI1145_ReInit(void)
{
  /*ENABLE UV reading
  these reg must be set to the fixed value */
  I2C_WriteRegister(SI1145_UCOEFF0, 0x29);
  I2C_WriteRegister(SI1145_UCOEFF1, 0x89);
  I2C_WriteRegister(SI1145_UCOEFF2, 0x02);
  I2C_WriteRegister(SI1145_UCOEFF3, 0x00);
  WriteParamData(SI1145_CHLIST, SI1145_CHLIST_ENUV |SI1145_CHLIST_ENALSIR | SI1145_CHLIST_ENALSVIS |SI1145_CHLIST_ENPS1);

  /* set LED1 CURRENT(22.4mA)(It is a normal value for many LED) */
  WriteParamData(SI1145_PS1_ADCMUX, SI1145_ADCMUX_LARGE_IR);
  I2C_WriteRegister(SI1145_PS_LED21, SI1145_LED_CURRENT_22MA);
  WriteParamData(SI1145_PSLED12_SELECT, SI1145_PSLED12_SELECT_PS1_LED1); //

  /* PS ADC SETTING */
  WriteParamData(SI1145_PS_ADC_GAIN, SI1145_ADC_GAIN_DIV1);
  WriteParamData(SI1145_PS_ADC_COUNTER, SI1145_ADC_COUNTER_511ADCCLK);
  WriteParamData(SI1145_PS_ADC_MISC, SI1145_ADC_MISC_HIGHRANGE|SI1145_ADC_MISC_ADC_RAWADC);

  /* VIS ADC SETTING */
  WriteParamData(SI1145_ALS_VIS_ADC_GAIN, SI1145_ADC_GAIN_DIV1);
  WriteParamData(SI1145_ALS_VIS_ADC_COUNTER, SI1145_ADC_COUNTER_511ADCCLK);
  WriteParamData(SI1145_ALS_VIS_ADC_MISC, SI1145_ADC_MISC_HIGHRANGE);

  /* IR ADC SETTING */
  WriteParamData(SI1145_ALS_IR_ADC_GAIN, SI1145_ADC_GAIN_DIV1);
  WriteParamData(SI1145_ALS_IR_ADC_COUNTER, SI1145_ADC_COUNTER_511ADCCLK);
  WriteParamData(SI1145_ALS_IR_ADC_MISC, SI1145_ADC_MISC_HIGHRANGE);

  /* interrupt enable */
  I2C_WriteRegister(SI1145_INT_CFG, SI1145_INT_CFG_INTOE);
  I2C_WriteRegister(SI1145_IRQ_ENABLE, SI1145_IRQEN_ALS);

  /* AUTO RUN */

  I2C_WriteRegister(SI1145_MEAS_RATE0, 0xFF);
  I2C_WriteRegister(SI1145_COMMAND, SI1145_PSALS_AUTO);
}

/**
 * @brief Reset the Sensor
 */
void SI1145_Reset(void)
{
  I2C_WriteRegister(SI1145_MEAS_RATE0,0x00);  // Set zero value in the MEAS_RATE0 register
  I2C_WriteRegister(SI1145_MEAS_RATE1,0x00); 	// Set zero value in the MEAS_RATE1 register
  I2C_WriteRegister(SI1145_IRQ_ENABLE,0x00); 	// Disable the interrupt
  I2C_WriteRegister(SI1145_IRQ_MODE1,0x00);
  I2C_WriteRegister(SI1145_IRQ_MODE2,0x00);
  I2C_WriteRegister(SI1145_INT_CFG,0x00);	   	//INT pin output disable
  I2C_WriteRegister(SI1145_IRQ_STATUS,0xFF);
  I2C_WriteRegister(SI1145_COMMAND,SI1145_RESET);		// Software reset

  HAL_Delay(10);								// wait for 10msec

  I2C_WriteRegister(SI1145_HW_KEY,0x17);		//proper si1145 operations

  HAL_Delay(10);								//wait for 10msec
}


/**
 * @brief Initialize the sensor
 * @return	value
 */
uint8_t SI1145_Init(void)
{
  //Init IIC  and reset si1145
  //
  if(I2C_ReadRegister(SI1145_PART_ID)!=0X45)
  {
    return 0;
  }
  SI1145_Reset();
  HAL_Delay(10);
  //
  //INIT
  //
  SI1145_ReInit();
  return 1;
}

/**
 * @brief Read the UV register value
 * @return	UV value
 */
uint16_t SI1145_GetUV(void)
{
  uint8_t data[2];
  uint16_t uv_data;                       // uv value
  //Read uv register
  data[0] = I2C_ReadRegister(SI1145_AUX_DATA0_UVINDEX0);
  data[1] = I2C_ReadRegister(SI1145_AUX_DATA1_UVINDEX1);
  // Merge bytes
  uv_data = data[0] | (data[1] << 8);
  return uv_data;
}

/**
 * @brief Read the Visible light register
 * @return	vis value
 */
uint16_t SI1145_GetVIS(void)
{
  uint8_t data[2];
  uint16_t vis_value;                       // vis value

  // Read registers
  data[0] = I2C_ReadRegister(SI1145_ALS_VIS_DATA0);
  data[1] = I2C_ReadRegister(SI1145_ALS_VIS_DATA1);
  // Merge bytes
  vis_value = data[0] | (data[1] << 8);

  return vis_value;
}

/**
 * @brief read the IR register value
 * @return	IR value
 */
uint16_t SI1145_GetIR(void)
{
  uint8_t data[2];
  uint16_t ir_value;                       // ir value

  // Read registers
  data[0] = I2C_ReadRegister(SI1145_ALS_IR_DATA0);
  data[1] = I2C_ReadRegister(SI1145_ALS_IR_DATA1);
  // Merge bytes
  ir_value = data[0] | (data[1] << 8);

  return ir_value;
}

/**
 * @brief Read the Proximity register value
 * @return Prox value
 */
uint16_t SI1145_GetPROX(void)
{
  uint8_t data[2];
  uint16_t prox_value;                       // prox value

  // Read registers
  data[0] = I2C_ReadRegister(SI1145_PS1_DATA0);
  data[1] = I2C_ReadRegister(SI1145_PS1_DATA0);
  // Merge bytes
  prox_value = data[0] | (data[1] << 8);

  return prox_value;
}

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

