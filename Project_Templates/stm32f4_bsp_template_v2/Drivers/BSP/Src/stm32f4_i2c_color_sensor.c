/******************************************************************//**
* @file		stm32f4_i2c_color_sensor.c
* @brief	Contains all functions support for I2C based Color
*           Sensor library
* @version	1.0
* @date		23. Jan. 2017
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup COLOR_SENSOR
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_i2c_color_sensor.h"


/** @defgroup STM32F4_I2C_COLOR_SENSOR_Exported_Handle
 * @{
 */
I2C_HandleTypeDef hi2c1_color;

/**
  * @}
  */


/** @defgroup STM32F4_I2C_COLOR_SENSOR_Private_Functions
  * @{
  */

/**
 * @brief This function is support function for BSP_COLOR_SENSOR_I2C1_Init
 *        This function configures GPIO of I2C1
 * @param hi2c  Provide I2C handle
 *      @arg  &hi2c1_color_sensor
 * @retval None
 */
static void COLOR_SENSOR_I2C1_MspInit(I2C_HandleTypeDef* hi2c)
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
  if(HAL_I2C_Master_Transmit(&hi2c1_color,COLOR_SENSOR_ADDR,I2C_Tx_Buf,2,2000) == HAL_OK)
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
  if(HAL_I2C_Master_Transmit(&hi2c1_color,COLOR_SENSOR_ADDR,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_color,COLOR_SENSOR_ADDR,I2C_Rx_Buf,1,2000) == HAL_OK)
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


/** @defgroup STM32F4_I2C_COLOR_SENSOR_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes I2C1
 * @retval None
 */
void BSP_COLOR_SENSOR_I2C1_Init(void)
{
  __GPIOB_CLK_ENABLE();

  hi2c1_color.Instance = I2C1;
  hi2c1_color.Init.ClockSpeed = 100000;
  hi2c1_color.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1_color.Init.OwnAddress1 = 0;
  hi2c1_color.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_color.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1_color.Init.OwnAddress2 = 0;
  hi2c1_color.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1_color.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  COLOR_SENSOR_I2C1_MspInit(&hi2c1_color);
  HAL_I2C_Init(&hi2c1_color);
}

/**
 * @brief This function is for De-Initializing I2C1
 * @param hi2c Provide I2C handle
 *      @arg  &hi2c1_color_sensor
 * @retval None
 */
void BSP_COLOR_SENSOR_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
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

void ColorSensor_Init(void)
{
    /* turn on the color sensor */
    BSP_COLOR_SENSOR_I2C1_Init();
    if(I2C_WriteRegister(COLOR_SENSOR_REG_CTL,REG_CTL_POWER|REG_CTL_ADC_EN) != 0)
    {
      while(1);
      Uprintf(&huart4,"Unable to initialize color sensor\r\n");
    }
}

uint16_t ColorSensor_ReadColor(ColorSense_TypeDef colour)
{
    uint8_t ColorRegistersCommand,colorvalue[2];
    uint16_t color;
    ColorRegistersCommand = REG_GREEN_LOW + (2 * (colour));

    colorvalue[0] = I2C_ReadRegister(ColorRegistersCommand);
    colorvalue[1] = I2C_ReadRegister((ColorRegistersCommand + 1));

    color = ((colorvalue[1]<<8)*256)|colorvalue[0];
    return color;
}

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

