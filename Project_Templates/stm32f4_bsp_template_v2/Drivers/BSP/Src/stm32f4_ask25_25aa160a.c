/**
  ******************************************************************************
  * @file    stm32f4_ask25_25aa160a.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    21-April-2015
  * @brief   This file provides set of firmware functions to manage SPI EEPROM
  *          available on STM32F4-ASK25 Kit from Edutech.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_ask25_25aa160a.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25_25AA160A
  * @{
  */

/** @defgroup STM32F4_ASK25_25AA160A
  * @brief This file provides set of firmware functions to manage SPI EEPROM
  *        available on STM32F4-ASK25 Kit from Edutech.
  * @{
  */


/** @defgroup STM32F4_ASK25_25AA160A_Exported_Handle
 * @{
 */
SPI_HandleTypeDef hspi2_25aa160a;

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_25AA160A_Private_FunctionPrototypes
  * @{
  */
const int8_t *status_reg[4]={"   STATUS REGISTER:\r\n\n",
                           "W/R                    W/R  W/R  R    R\r\n",
                           "D7   D6   D5   D4      D3   D2   D1   D0\r\n",
                           "WPEN X    X    X   --  BP1  BP0  WEL  WIP",
};


void print_status_reg(void)
{
  uint8_t count;

  vcp_clr_scr_rst_cur();

  for(count = 0; count<4; ++count)
  {
    vuprintf(status_reg[count]);
  }
}

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_25AA160A_Private_Functions
  * @{
  */
/**
 * @brief This function is support function for ASK25_SPI2_Init
 *        This function configures GPIO of SPI2
 * @param hspi  Provide SPI handle
 *      @arg  &hspi2_25aa160a
 * @retval None
 */
static void ASK25_SPI2_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(hspi->Instance==SPI2)
  {
    /* Peripheral clock enable */
    __SPI2_CLK_ENABLE();

    /**SPI2 GPIO Configuration
    PB10     ------> SPI2_SCK
    PC2     ------> SPI2_MISO
    PC3     ------> SPI2_MOSI
    PB14     ------> SPI2_CS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PB14 */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
//    HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(SPI2_IRQn);
  }
}


/**
 * @brief Drive CS output pin to low/high level to select slave device
 *        via /CS pin state
 * @param state   state State of CS output pin that will be driven:
 *        @arg 0 (DISABLE): Drive CS pin to low level
 *        @arg 1 (ENABLE) : Drive CS pin to high level
 * @return  None
 */
static void SPI2_CS_Force (FunctionalState state)
{
  if (state)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  }
}

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_25AA160A_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes SPI2
 * @retval None
 */
void ASK25_SPI2_Init(void)
{
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();

  hspi2_25aa160a.Instance = SPI2;
  hspi2_25aa160a.Init.Mode = SPI_MODE_MASTER;
  hspi2_25aa160a.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2_25aa160a.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2_25aa160a.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2_25aa160a.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2_25aa160a.Init.NSS = SPI_NSS_SOFT;
  hspi2_25aa160a.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi2_25aa160a.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2_25aa160a.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi2_25aa160a.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;

  ASK25_SPI2_MspInit(&hspi2_25aa160a);
  HAL_SPI_Init(&hspi2_25aa160a);
}


/**
 * @brief This function is for De-Initializing SPI2
 * @param hspi Provide SPI handle
 *      @arg  &hspi2_25aa160a
 * @retval None
 */
void ASK25_SPI2_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI2)
  {
    /* Peripheral clock disable */
    __SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB10     ------> SPI2_SCK
    PC2     ------> SPI2_MISO
    PC3     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2 | GPIO_PIN_3);

    /* Peripheral interrupt Deinit*/
//    HAL_NVIC_DisableIRQ(SPI2_IRQn);
  }
}


/**
 * @brief Get the status about locked or unlocked memory blocks.
 * @return  status register byte
 */
uint8_t ASK25_25AA160A_Read_Status_Reg (void)
{
  uint8_t reg[1];
  HAL_StatusTypeDef status=HAL_ERROR;
  uint8_t inst[1];

  inst[0]= EEP_RDSR;

  SPI2_CS_Force(DISABLE);
  HAL_SPI_Transmit(&hspi2_25aa160a, inst, 1,500);
  status = HAL_SPI_Receive(&hspi2_25aa160a, reg, 1,500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
  }

  return(reg[0]);
}


/**
 * @brief Reads and Display Status Register in format
 * @return None
 */
void ASK25_25AA160A_Read_Status (void)
{
  uint8_t wpen=0, bp1=0, bp0=0, wel=0, wip=0;
  uint8_t dat;

  vcp_clr_scr_rst_cur();
  dat = ASK25_25AA160A_Read_Status_Reg();
  if(dat&0x80){wpen=1;}else{wpen=0;}
  if(dat&0x08){bp1 =1;}else{bp1 =0;}
  if(dat&0x04){bp0 =1;}else{bp0 =0;}
  if(dat&0x02){wel =1;}else{wel =0;}
  if(dat&0x01){wip =1;}else{wip =0;}
  print_status_reg();
  vuprintf("\n\r%d01    X    X    X       %d01    %d01    %d01    %d01\n\r",wpen,bp1,bp0,wel,wip);
  vgetche(BLOCKING);
}


/**
 * @brief To lock or unlock data sectors
 * @param status_reg  Write status value
 */
HAL_StatusTypeDef ASK25_25AA160A_Write_Status_Reg (uint8_t status_reg)
{
  uint8_t tx_buff[2];
  uint8_t WE;
  HAL_StatusTypeDef status=HAL_ERROR;

  WE = EEP_WREN;
  tx_buff[0] = EEP_WRSR;                 /* Write Status 8bit msb                     */
  tx_buff[1] = status_reg;               /* STATUS REGISTER                           */
                                         /* W/R                    W/R  W/R  R    R   */
                                         /* D7   D6   D5   D4      D3   D2   D1   D0  */
                                         /* WPEN X    X    X   --  BP1  BP0  WEL  WIP */

  SPI2_CS_Force(DISABLE);
  status = HAL_SPI_Transmit(&hspi2_25aa160a, &WE, 1, 500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
  }
  else
    return (status);

  SPI2_CS_Force(DISABLE);
  status = HAL_SPI_Transmit(&hspi2_25aa160a, tx_buff, 2, 500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
    HAL_Delay(5);     // Delay for 5ms Write Cycle
    return (HAL_OK);
  }
  else
    return (status);
}


/**
 * @brief Write Byte value at desired address(0x000 to 0x7FF)
 * @param eep_address   EEPROM 16bit address
 * @param byte_data     byte data to be written
 * @return  status
 */
HAL_StatusTypeDef ASK25_25AA160A_Write_Byte (uint16_t eep_address, uint8_t byte_data)
{
  HAL_StatusTypeDef status=HAL_ERROR;
  uint8_t WE;
  uint8_t Tx_Buf[4];

  WE = EEP_WREN;
  Tx_Buf[0] = EEP_WRITE;
  Tx_Buf[1] = (uint8_t)(eep_address>>8);    // 1st byte extract
  Tx_Buf[2] = (uint8_t) eep_address;        // 2nd byte extract
  Tx_Buf[3] = byte_data;

  SPI2_CS_Force(DISABLE);
  status=HAL_SPI_Transmit(&hspi2_25aa160a, &WE, 1, 500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
  }
  else
    return(status);

  SPI2_CS_Force(DISABLE);
  status=HAL_SPI_Transmit(&hspi2_25aa160a, Tx_Buf, 4, 500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
    HAL_Delay(5);     // Delay for 5ms Write Cycle
    return(HAL_OK);
  }
  else
    return(status);
}


/**
 * @brief Write value array at desired address(0x000 to 0x7FF)
 * @param eep_address   EEPROM 16bit address
 * @param data_start    buffer address
 * @param length        size of buffer
 * @return  Write status
 */
HAL_StatusTypeDef ASK25_25AA160A_Write (uint16_t eep_address, uint8_t *data_start, uint16_t length)
{
  HAL_StatusTypeDef status=HAL_ERROR;
  uint8_t WE, ip_len, new_length;
  uint16_t new_address;
  uint8_t Tx_Buf[3];

  WE = EEP_WREN;

  Tx_Buf[0] = EEP_WRITE;
  Tx_Buf[1] = (uint8_t)(eep_address>>8);    // 1st byte extract
  Tx_Buf[2] = (uint8_t) eep_address;        // 2nd byte extract

  /* Intern page length(ip_len) gives length from address that can be occupied in page */
  ip_len = 0x10 - (eep_address % 0x10);
  new_address = eep_address + ip_len;

  if(length > ip_len)
  {
    new_length = length - ip_len;

    SPI2_CS_Force(DISABLE);
    status=HAL_SPI_Transmit(&hspi2_25aa160a, &WE, 1, 500);
    if(status == HAL_OK)
    {
      SPI2_CS_Force(ENABLE);
    }
    else
      return(status);

    SPI2_CS_Force(DISABLE);
    status=HAL_SPI_Transmit(&hspi2_25aa160a, Tx_Buf, 3, 500);
    status=HAL_SPI_Transmit(&hspi2_25aa160a, data_start, ip_len, 500);
    if(status == HAL_OK)
    {
      SPI2_CS_Force(ENABLE);
    }
    else
      return(status);

    // Recursive function
    HAL_Delay(5);
    ASK25_25AA160A_Write(new_address, (data_start + ip_len), new_length);
  }

  if(length <= ip_len)
  {
    SPI2_CS_Force(DISABLE);
    status=HAL_SPI_Transmit(&hspi2_25aa160a, &WE, 1, 500);
    if(status == HAL_OK)
    {
      SPI2_CS_Force(ENABLE);
    }
    else
      return(status);

    SPI2_CS_Force(DISABLE);
    status=HAL_SPI_Transmit(&hspi2_25aa160a, Tx_Buf, 3, 500);
    status=HAL_SPI_Transmit(&hspi2_25aa160a, data_start, length, 500);
    if(status == HAL_OK)
    {
      SPI2_CS_Force(ENABLE);
      HAL_Delay(5);     // Delay for 5ms Write Cycle
    }
    else
      return(status);
  }
}


/**
 * @brief Read Byte value at desired address(0x000 to 0x7FF)
 * @param eep_address   EEPROM 16bit address
 * @return  Read Byte is returned
 */
uint8_t ASK25_25AA160A_Read_Byte (uint16_t eep_address)
{
  HAL_StatusTypeDef status=HAL_ERROR;
  uint8_t Tx_Buf[3];
  uint8_t Rx_Buf[1];

  Tx_Buf[0] = EEP_READ;
  Tx_Buf[1] = (uint8_t)(eep_address>>8);  // 1st byte extract
  Tx_Buf[2] = (uint8_t)eep_address;       // 2nd byte extract

  SPI2_CS_Force(DISABLE);
  status = HAL_SPI_Transmit(&hspi2_25aa160a, Tx_Buf, 3, 500);
  status = HAL_SPI_Receive(&hspi2_25aa160a, Rx_Buf, 1, 500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
    HAL_Delay(5);     // Delay for 5ms Read Cycle
  }

  return(Rx_Buf[0]);
}


/**
 * @brief Read Page value at desired address(0x000 to 0x7FF)
 * @param eep_address   EEPROM 16bit address
 * @param dest_addr     buffer address
 * @param length        size of buffer
 * @return  Read status
 */
HAL_StatusTypeDef ASK25_25AA160A_Read (uint16_t eep_address, uint8_t *dest_addr, uint16_t length)
{
  HAL_StatusTypeDef status=HAL_ERROR;
  uint8_t Tx_Buf[3];

  Tx_Buf[0] = EEP_READ;
  Tx_Buf[1] = (uint8_t)(eep_address>>8);  // 1st byte extract
  Tx_Buf[2] = (uint8_t)eep_address;       // 2nd byte extract

  SPI2_CS_Force(DISABLE);
  status = HAL_SPI_Transmit(&hspi2_25aa160a, Tx_Buf, 3, 500);
  status = HAL_SPI_Receive(&hspi2_25aa160a, dest_addr, length, 500);
  if(status == HAL_OK)
  {
    SPI2_CS_Force(ENABLE);
  }

  return(status);
}


/**
 * @brief This function can be used to Display Read data stored in array
 * @param string  Buffer (uint8_t size)
 * @param length  Length of Buffer
 */
void ASK25_25AA160A_Display_Array (uint8_t *string, uint16_t length)
{
  while(length)
  {
    vuprintf("%x02  ",*string++);
    length--;
  }
}


/**
 * @brief This function Ask for Address Range you want to display
 * @param mem_start_address   starting address of EEPROM
 * @param mem_end_address     ending address of EEPROM
 * @retval None
 */
void ASK25_25AA160A_Display_Loc (uint16_t mem_start_address, uint16_t mem_end_address)
{
  uint8_t line=0,count=0;
  uint8_t dat;
  uint16_t addr;

  vuprintf("EEPROM Range = 0x000 - 0x7FF \r\n");

  vcp_clr_scr_rst_cur();
  vuprintf("Start: %x03   End: %x03 \r\n",mem_start_address,mem_end_address);

  for(addr=mem_start_address; addr<mem_end_address+1; addr++)
  {
    if(count == 0) vuprintf("%x03   ",addr);

    dat = ASK25_25AA160A_Read_Byte(addr);   /* read byte from address                  */

    vuprintf("%x02  ",dat);
    count++;

    if(count == 16)                    /* check for last digit entered            */
    {
      line++;
      count = 0;
      vuprintf("\r\n");
    }

    if(line == 20 || addr == mem_end_address)
    {
      vuprintf("\x1b[24;01HPress any key to continue.");
      line = 0;
      vgetche(BLOCKING);
      vcp_clr_scr_rst_cur();
    }
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

/*****************************************END OF FILE******************************************/
