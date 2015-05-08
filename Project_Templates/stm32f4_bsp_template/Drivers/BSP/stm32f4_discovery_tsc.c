/**
  ******************************************************************************
  * @file    stm32f4_discovery_tsc.c
  * @author  Dwijay.Edutech learning solutions
  * @version V1.0.0
  * @date    07-May-2015
  * @brief   This file includes the LCD Touch screen driver for (LCD_STMPE811QTR)
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_tsc.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TSC
  * @brief This file includes the TSC driver for (LCD_STMPE811QTR)
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Private_TypesDef
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_TSC_Private_Macros
  * @{
  */
#define TIMEOUT_MAX    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Private_Variables
  * @{
  */
TS_STATE TS_State;              /*<! The global structure holding the TS state */
uint32_t IOE_TimeOut = TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Private_Functions
  * @{
  */
static uint16_t IOE_TS_Read_Z(void);

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
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

    /* Peripheral DMA init*/

    hdma_i2c1_tx.Instance = DMA1_Stream6;
    hdma_i2c1_tx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_i2c1_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hdma_i2c1_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_i2c1_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&hdma_i2c1_tx);

    __HAL_LINKDMA(hi2c,hdmatx,hdma_i2c1_tx);

    hdma_i2c1_rx.Instance = DMA1_Stream0;
    hdma_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_i2c1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hdma_i2c1_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_i2c1_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    HAL_DMA_Init(&hdma_i2c1_rx);

    __HAL_LINKDMA(hi2c,hdmarx,hdma_i2c1_rx);
  }
}

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_Functions
  * @{
  */


/* I2C1 init function */
void MX_I2C1_Init(void)
{
  /* DMA controller clock enable */
  __DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

  __GPIOB_CLK_ENABLE();

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  HAL_I2C_MspInit(&hi2c1);
  HAL_I2C_Init(&hi2c1);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
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

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(hi2c->hdmatx);
    HAL_DMA_DeInit(hi2c->hdmarx);
  }
}

/**
  * @brief  Initializes and Configures the two IO_Expanders Functionalities
  *         (IOs, Touch Screen ..) and configures all STM32F4_DISCOVERY necessary
  *         hardware (GPIOs, APB clocks ..).
  * @param  None
  * @retval IOE_OK if all initializations done correctly. Other value if error.
  */
uint8_t IOE_Config(void)
{
  /* Configure the needed pins */
  MX_I2C1_Init();

  /* Read IO Expander 1 ID  */
  if (IOE_IsOperational(IOE_1_ADDR)) {
    return IOE1_NOT_OPERATIONAL;
  }

  /* Generate IOExpander Software reset */
  IOE_Reset(IOE_1_ADDR);

  /* ---------------------- IO Expander 1 configuration --------------------- */
  /* Enable the GPIO, Touch Screen and ADC functionalities */
  IOE_FnctCmd(IOE_1_ADDR, IOE_IO_FCT | IOE_TS_FCT | IOE_ADC_FCT, ENABLE);

  /* Touch Screen controller configuration */
  IOE_TS_Config();

  /* Configuration is OK */
  return IOE_OK;
}


/**
  * @brief  Configures The selected interrupts on the IO Expanders.
  * @param  IOE_ITSRC_Source: the source of the interrupts. Could be one or a
  *         combination of the following parameters:
  *   @arg  IOE_ITSRC_JOYSTICK: Joystick IO intputs.
  *   @arg  IOE_ITSRC_TSC: Touch Screen interrupts.
  *   @arg  IOE_ITSRC_INMEMS: MEMS interrupt lines.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ITConfig(uint32_t IOE_ITSRC_Source)
{
  /* Configure the Interrupt output pin to generate low level (INT_CTRL) */
  IOE_ITOutConfig(Polarity_High, Type_Level);

  /* Manage the Touch Screen Interrupts */
  if (IOE_ITSRC_Source & IOE_ITSRC_TSC) {
    /* Enable the Global interrupt */
    IOE_GITCmd(IOE_1_ADDR, ENABLE);

    /* Enable the Global GPIO Interrupt */
    IOE_GITConfig(IOE_1_ADDR, (uint8_t)(IOE_GIT_TOUCH | IOE_GIT_FTH), ENABLE);

    /* Read the GPIO_IT_STA to clear all pending bits if any */
    I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_INT_STA);
  }

  /* Configure the Interrupt line as EXTI source */
  IOE_EXTI_Config();

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Returns Status and positions of the Touch screen.
  * @param  None
  * @retval Pointer to TS_STATE structure holding Touch Screen information.
  */
TS_STATE* IOE_TS_GetState(void)
{
  uint16_t xDiff, yDiff , x , y;
  static uint16_t _x = 0, _y = 0;

  /* Check if the Touch detect event happened */
  TS_State.TouchDetected = (I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_CTRL) & 0x80);

  if (TS_State.TouchDetected) {
    x = IOE_TS_Read_X();
    y = IOE_TS_Read_Y();
    xDiff = x > _x? (x - _x): (_x - x);
    yDiff = y > _y? (y - _y): (_y - y);
    if (xDiff + yDiff > 5) {
      _x = x;
      _y = y;
    }
  }
  /* Update the X position */
  TS_State.X = _x;

  /* Update the Y position */
  TS_State.Y = _y;
  /* Update the Z position index */
  TS_State.Z = IOE_TS_Read_Z();

  /* Clear the interrupt pending bit and enable the FIFO again */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x01);
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x00);

  /* Return pointer to the updated structure */
  return &TS_State;
}


/**
  * @brief  Checks the selected Global interrupt source pending bit
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  Global_IT: the Global interrupt source to be checked, could be:
  *   @arg  Global_IT_GPIO : All IOs interrupt
  *   @arg  Global_IT_ADC : ADC interrupt
  *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
  *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
  *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
  *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
  *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
  *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
  * @retval Status of the checked flag. Could be SET or RESET.
  */
FlagStatus IOE_GetGITStatus(uint8_t DeviceAddr, uint8_t Global_IT)
{
  __IO uint8_t tmp = 0;

  /* get the Interrupt status */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_INT_STA);

  if ((tmp & (uint8_t)Global_IT) != 0) {
    return SET;
  } else {
    return RESET;
  }
}


/**
  * @brief  Clears the selected Global interrupt pending bit(s)
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  Global_IT: the Global interrupt to be cleared, could be any combination
  *         of the following values:
  *   @arg  Global_IT_GPIO : All IOs interrupt
  *   @arg  Global_IT_ADC : ADC interrupt
  *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
  *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
  *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
  *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
  *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
  *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ClearGITPending(uint8_t DeviceAddr, uint8_t Global_IT)
{
  /* Write 1 to the bits that have to be cleared */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_INT_STA, Global_IT);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Checks the status of the selected IO interrupt pending bit
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_IT: the IO interrupt to be checked could be IO_ITx Where x can be
  *         from 0 to 7.
  * @retval Status of the checked flag. Could be SET or RESET.
  */
FlagStatus IOE_GetIOITStatus(uint8_t DeviceAddr, uint8_t IO_IT)
{
  uint8_t tmp = 0;

  /* get the Interrupt status */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_STA);

  if ((tmp & (uint8_t)IO_IT) != 0) {
    return SET;
  } else {
    return RESET;
  }
}


/**
  * @brief  Checks if the selected device is correctly configured and
  *         communicates correctly on the I2C bus.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @retval IOE_OK if IOE is operational. Other value if failure.
  */
uint8_t IOE_IsOperational(uint8_t DeviceAddr)
{
  /* Return Error if the ID is not correct */
  if ( IOE_ReadID(DeviceAddr) != (uint16_t)STMPE811_ID ) {
    /* Check if a Timeout occurred */
    if (IOE_TimeOut == 0) {
      return(IOE_TimeoutUserCallback());
    } else {
      return IOE_FAILURE; /* ID is not Correct */
    }
  } else {
    return IOE_OK; /* ID is correct */
  }
}


/**
  * @brief  Resets the IO Expander by Software (SYS_CTRL1, RESET bit).
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_Reset(uint8_t DeviceAddr)
{
  /* Power Down the IO_Expander */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL1, 0x02);

  /* wait for a delay to insure registers erasing */
  HAL_Delay(20);

  /* Power On the Codec after the power off => all registers are reinitialized*/
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL1, 0x00);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Reads the selected device's ID.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @retval The Device ID (two bytes).
  */
uint16_t IOE_ReadID(uint8_t DeviceAddr)
{
  uint16_t tmp = 0;

  /* Read device ID  */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, 0);
  tmp = (uint32_t)(tmp << 8);
  tmp |= (uint32_t)I2C_ReadDeviceRegister(DeviceAddr, 1);

  /* Return the ID */
  return (uint16_t)tmp;
}


/**
  * @brief  Configures the selected IO Expander functionalities.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IOE_TEMPSENS_FCT: the functions to be configured. could be any
  *         combination of the following values:
  *   @arg  IOE_IO_FCT : IO function
  *   @arg  IOE_TS_FCT : Touch Screen function
  *   @arg  IOE_ADC_FCT : ADC function
  *   @arg  IOE_TEMPSENS_FCT : Temperature Sensor function
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_FnctCmd(uint8_t DeviceAddr, uint8_t Fct, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Get the register value */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL2);

  if (NewState != DISABLE) {
    /* Set the Functionalities to be Enabled */
    tmp &= ~(uint8_t)Fct;
  } else {
    /* Set the Functionalities to be Disabled */
    tmp |= (uint8_t)Fct;
  }

  /* Set the register value */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_SYS_CTRL2, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the selected pin direction (to be an input or an output)
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_Pin: IO_Pin_x: Where x can be from 0 to 7.
  * @param  Direction: could be Direction_IN or Direction_OUT.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOPinConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Direction)
{
  uint8_t tmp = 0;

  /* Get all the Pins direction */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_DIR);

  if (Direction != Direction_IN) {
    tmp |= (uint8_t)IO_Pin;
  } else {
    tmp &= ~(uint8_t)IO_Pin;
  }

  /* Write the register new value */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_DIR, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Enables or disables the Global interrupt.
  * @param  DeviceAddr: The address of the IOExpander, could be :I OE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  NewState: could be ENABLE or DISABLE.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_GITCmd(uint8_t DeviceAddr, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Read the Interrupt Control register  */
  I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_INT_CTRL);

  if (NewState != DISABLE) {
    /* Set the global interrupts to be Enabled */
    tmp |= (uint8_t)IOE_GIT_EN;
  } else {
    /* Set the global interrupts to be Disabled */
    tmp &= ~(uint8_t)IOE_GIT_EN;
  }

  /* Write Back the Interrupt Control register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_INT_CTRL, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Configures the selected source to generate or not a global interrupt
  * @param DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *        or IOE_2_ADDR.
  * @param Global_IT: the interrupt source to be configured, could be:
  *   @arg  Global_IT_GPIO : All IOs interrupt
  *   @arg  Global_IT_ADC : ADC interrupt
  *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
  *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
  *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
  *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
  *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
  *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_GITConfig(uint8_t DeviceAddr, uint8_t Global_IT, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Get the current value of the INT_EN register */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_INT_EN);

  if (NewState != DISABLE) {
    /* Set the interrupts to be Enabled */
    tmp |= (uint8_t)Global_IT;
  } else {
    /* Set the interrupts to be Disabled */
    tmp &= ~(uint8_t)Global_IT;
  }
  /* Set the register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_INT_EN, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Configures the selected pins to generate an interrupt or not.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_IT: The IO interrupt to be configured. This parameter could be any
  *         combination of the following values:
  *   @arg  IO_IT_x: where x can be from 0 to 7.
  * @param  NewState: could be ENABLE or DISABLE.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOITConfig(uint8_t DeviceAddr, uint8_t IO_IT, FunctionalState NewState)
{
  uint8_t tmp = 0;

  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_EN);

  if (NewState != DISABLE) {
    /* Set the interrupts to be Enabled */
    tmp |= (uint8_t)IO_IT;
  } else {
    /* Set the interrupts to be Disabled */
    tmp &= ~(uint8_t)IO_IT;
  }

  /* Set the register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_INT_EN, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Configures the touch Screen Controller (Single point detection)
  * @param  None
  * @retval IOE_OK if all initializations are OK. Other value if error.
  */
uint8_t IOE_TS_Config(void)
{
  uint8_t tmp = 0;

  /* Enable TSC Fct: already done in IOE_Config */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_SYS_CTRL2);
  tmp &= ~(uint32_t)(IOE_TS_FCT | IOE_ADC_FCT);
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_SYS_CTRL2, tmp);

  /* Enable the TSC global interrupts */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_INT_EN);
  tmp |= (uint32_t)(IOE_GIT_TOUCH | IOE_GIT_FTH );
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_INT_EN, tmp);

  /* Select Sample Time, bit number and ADC Reference */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_ADC_CTRL1, 0x49);

  /* Wait for ~20 ms */
  HAL_Delay(20);

  /* Select the ADC clock speed: 3.25 MHz */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_ADC_CTRL2, 0x01);

  /* Select TSC pins in non default mode */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_AF);
  tmp &= ~(uint8_t)TOUCH_IO_ALL;
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_GPIO_AF, tmp);

  /* Select 2 nF filter capacitor */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_CFG, 0x9A);

  /* Select single point reading  */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_TH, 0x01);

  /* Write 0x01 to clear the FIFO memory content. */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x01);

  /* Write 0x00 to put the FIFO back into operation mode  */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_FIFO_STA, 0x00);

  /* set the data format for Z value: 7 fractional part and 1 whole part */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_FRACT_XYZ, 0x01);

  /* set the driving capability of the device for TSC pins: 50mA */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_I_DRIVE, 0x01);

  /* Use no tracking index, touchscreen controller operation mode (XYZ) and
     enable the TSC */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_TSC_CTRL, 0x01);

  /*  Clear all the status pending bits */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_INT_STA, 0xFF);

  /* Initialize the TS structure to their default values */
  TS_State.TouchDetected = TS_State.X = TS_State.Y = TS_State.Z = 0;

  /* All configuration done */
  return IOE_OK;
}

/**
  * @brief  Configures the selected pin to be in Alternate function or not
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_Pin: IO_Pin_x, Where x can be from 0 to 7.
  * @param  NewState: State of the AF for the selected pin, could be
  *         ENABLE or DISABLE.
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOAFConfig(uint8_t DeviceAddr, uint8_t IO_Pin, FunctionalState NewState)
{
  uint8_t tmp = 0;

  /* Get the current state of the GPIO_AF register */
  tmp = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_AF);

  if (NewState != DISABLE) {
    /* Enable the selected pins alternate function */
    tmp |= (uint8_t)IO_Pin;
  } else {
    /* Disable the selected pins alternate function */
    tmp &= ~(uint8_t)IO_Pin;
  }

  /* Write back the new value in GPIO_AF register */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_AF, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}

/**
  * @brief  Configures the Edge for which a transition is detectable for the
  *         the selected pin.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  IO_Pin: IO_Pin_x, Where x can be from 0 to 7.
  * @param  Edge: The edge which will be detected. This parameter can be one or a
  *         a combination of following values: EDGE_FALLING and EDGE_RISING .
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_IOEdgeConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Edge)
{
  uint8_t tmp1 = 0, tmp2 = 0;

  /* Get the registers values */
  tmp1 = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_FE);
  tmp2 = I2C_ReadDeviceRegister(DeviceAddr, IOE_REG_GPIO_RE);

  /* Disable the Falling Edge */
  tmp1 &= ~(uint8_t)IO_Pin;
  /* Disable the Falling Edge */
  tmp2 &= ~(uint8_t)IO_Pin;

  /* Enable the Falling edge if selected */
  if (Edge & EDGE_FALLING) {
    tmp1 |= (uint8_t)IO_Pin;
  }

  /* Enable the Rising edge if selected */
  if (Edge & EDGE_RISING) {
    tmp2 |= (uint8_t)IO_Pin;
  }

  /* Write back the registers values */
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_FE, tmp1);
  I2C_WriteDeviceRegister(DeviceAddr, IOE_REG_GPIO_RE, tmp2);

  /* if OK return 0 */
  return IOE_OK;
}

/**
  * @brief  Configures the Interrupt line active state and format (level/edge)
  * @param  Polarity: could be
  *   @arg  Polarity_Low: Interrupt line is active Low/Falling edge
  *   @arg  Polarity_High: Interrupt line is active High/Rising edge
  * @param  Type: Interrupt line activity type, could be one of the following values
  *   @arg  Type_Level: Interrupt line is active in level model
  *   @arg  Type_Edge: Interrupt line is active in edge model
  * @retval IOE_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE_ITOutConfig(uint8_t Polarity, uint8_t Type)
{
  uint8_t tmp = 0;

  /*  Get the register IOE_REG_INT_CTRL value */
  tmp = I2C_ReadDeviceRegister(IOE_1_ADDR, IOE_REG_INT_CTRL);

  /* Mask the polarity and type bits */
  tmp &= ~(uint8_t)0x06;

  /* Modify the Interrupt Output line configuration */
  tmp |= (uint8_t)(Polarity | Type);

  /* Set the register */
  I2C_WriteDeviceRegister(IOE_1_ADDR, IOE_REG_INT_CTRL, tmp);

  /* If all OK return IOE_OK */
  return IOE_OK;
}


/**
  * @brief  Writes a value in a register of the device through I2C.
  * @param  DeviceAddr: The address of the IOExpander, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  RegisterAddr: The target register address
  * @param  RegisterValue: The target register value to be written
  * @retval IOE_OK: if all operations are OK. Other value if error.
  */
uint8_t I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_verif = 0;

  status = HAL_I2C_Mem_Write_DMA(&hi2c1, DeviceAddr, RegisterAddr, I2C_MEMADD_SIZE_8BIT, &RegisterValue, 1);
  HAL_Delay(1);

#ifdef VERIFY_WRITTENDATA
  /* Verify (if needed) that the loaded data is correct  */

  /* Read the just written register*/
  read_verif = I2C_ReadDeviceRegister(DeviceAddr, RegisterAddr);
  /* Load the register and verify its value  */
  if (read_verif != RegisterValue) {
    /* Control data wrongly transfered */
    read_verif = IOE_FAILURE;
  } else {
    /* Control data correctly transfered */
    read_verif = 0;
  }
#endif

  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return (read_verif);
}

uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t IOE_BufferRX[2] = {0x00, 0x00};

  status = HAL_I2C_Mem_Read_DMA(&hi2c1, DeviceAddr, RegisterAddr, I2C_MEMADD_SIZE_8BIT, IOE_BufferRX, 1);
  HAL_Delay(1);
  if (status == HAL_OK)
  {
    return (IOE_BufferRX[0]);
  }
  else
  {
    return (HAL_ERROR);
  }
}


/**
  * @brief  Reads a buffer of 2 bytes from the device registers.
  * @param  DeviceAddr: The address of the device, could be : IOE_1_ADDR
  *         or IOE_2_ADDR.
  * @param  RegisterAddr: The target register address (between 00x and 0x24)
  * @retval A pointer to the buffer containing the two returned bytes (in halfword).
  */
uint16_t I2C_ReadDataBuffer(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
  uint8_t tmp= 0;
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t IOE_BufferRX[2] = {0x00, 0x00};

  status = HAL_I2C_Mem_Read_DMA(&hi2c1, DeviceAddr, RegisterAddr, I2C_MEMADD_SIZE_8BIT, IOE_BufferRX, 2);
  HAL_Delay(1);
  if (status == HAL_OK)
  {
    /* Reorganize received data */
    tmp = IOE_BufferRX[0];
    IOE_BufferRX[0] = IOE_BufferRX[1];
    IOE_BufferRX[1] = tmp;

    /* return a pointer to the IOE_Buffer */
    return (*(__IO uint16_t *) IOE_BufferRX);
  }
  else
  {
    return (HAL_ERROR);
  }
}


/**
  * @brief  Return Touch Screen X position value
  * @param  None
  * @retval X position.
  */
uint16_t IOE_TS_Read_X(void)
{
#if 0
  int32_t x, xr;

  x = I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_X);

  /* first correction */
  xr =  (x * 320) >> 12;
  /* second correction */
  xr = ((xr * 32)/29) - 17;

  if (xr <= 0) {
    xr = 0;
  }

  return (uint16_t)(xr);
#else
  int32_t icount, sum = 0;

  for (icount = 0; icount < 16; icount++) {
    sum += I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_X);
  }
  sum >>= 4;

  if (sum <= 0) {
    sum = 0;
  }

  return (uint16_t)(sum);
#endif
}

/**
  * @brief  Return Touch Screen Y position value
  * @param  None
  * @retval Y position.
  */
uint16_t IOE_TS_Read_Y(void)
{
#if 0

  int32_t y, yr;
  y= I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_Y);

  yr= (y * 240) >> 12;
  yr = ((yr * 240) / 217) - 12;

  if (yr <= 0) {
    yr = 0;
  }
  return (uint16_t)(yr);
#else
  int32_t icount, sum = 0;

  for (icount = 0; icount < 16; icount++) {
    sum += I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_Y);
  }
  sum >>= 4;

  if (sum <= 0) {
    sum = 0;
  }

  return (uint16_t)(sum);
#endif
}

/**
  * @brief  Return Touch Screen Z position value
  * @param  None
  * @retval Z position.
  */
static uint16_t IOE_TS_Read_Z(void)
{
  uint32_t z;
  z = I2C_ReadDataBuffer(IOE_1_ADDR, IOE_REG_TSC_DATA_Z);

  if (z <= 0) {
    z = 0;
  }
  return (uint16_t)(z);
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

/**
  * @}
  */

/*******END OF FILE*******/
