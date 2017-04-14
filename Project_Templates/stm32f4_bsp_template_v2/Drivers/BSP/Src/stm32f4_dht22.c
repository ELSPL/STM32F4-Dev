/******************************************************************//**
* @file	    stm32f4_dht22.c
* @brief    This is Temperature and Humidity sensor driver file
* @version  V1.0
* @date		Jan 24, 2017
* @author	Bhavin Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------- */
#include "stm32f4_dht22.h"

/**
 * @brief Initialize Pin For DHT22
 * @param mode Pin mode output or input
 *        @arg GPIO_MODE_INPUT
 *        @arg GPIO_MODE_OUTPUT_PP
 */
void DHT22_PinConfig(uint8_t mode)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIOC Clock */

  __GPIOC_CLK_ENABLE();

  /* Configure the GPIO_ALED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/**
 * @brief Initialize Pin for DHT22
 * @return none
 */
void DHT22_Init(void)
{
  DHT22_PinConfig(GPIO_MODE_INPUT);
}


/**
 * @brief Read DHT22 data and display it on UART4
 */
void DHT22_read(void)
{
    uint16_t   lastHumidity1;
    uint16_t   lastHumidity2;
    uint16_t   lastTemperature1;
    uint16_t   lastTemperature2;
    uint8_t    i, j, retryCount;
    uint16_t   currentTemperature=0;
    uint16_t   currentHumidity=0;
    uint32_t   checkSum = 0, csPart1, csPart2, csPart3, csPart4;
    uint32_t   bitTimes[41];

    for (i = 0; i < 41; i++)
    {
        bitTimes[i] = 0;
    }

    retryCount = 0;
    // Pin needs to start HIGH, wait unit it is HIGH with a timeout
    while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == 0)// exit on DHT22 return 'High' Signal within 250us
    {
      if (retryCount > 125)
      {
            uprintf(&huart4,"DHT22 Bus busy!\r\n");
        }
        retryCount ++;
        Delay_Us(2);
    }
  /**
   * Send the activate pulse
   * Step 1: MCU send out start signal to DHT22 and DHT22 send
   * response signal to MCU.
   * If signal is high-voltage-level, it means DHT22 is not
   * working properly, please check the electrical connection status.
   */
    DHT22_PinConfig(GPIO_MODE_OUTPUT_PP); // set pin to output data

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET); // MCU send out start signal to DHT22
    HAL_Delay(18);   // 18 ms wait (spec: at least 1ms)
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);  // MCU pull up
    Delay_Us(40);
    DHT22_PinConfig(GPIO_MODE_INPUT); // set pin to receive data

    // Find the start of the ACK Pulse
    retryCount = 0;
    while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == 1)     // Exit on DHT22 pull low within 40us
    {
        if (retryCount > 40)  // (Spec is 20-40 us high)
        {
            uprintf(&huart4,"DHT22 not responding!\r\n");
        }
        retryCount++;
        Delay_Us(1);
    }

    Delay_Us(80); // DHT pull up ready to transmit data

  /**
   * Reading the 5 byte data stream
   * Step 2: DHT22 send data to MCU
   *    Start bit -> low volage within 50us (actually could be anything from 35-75us)
   *             0         -> high volage within 26-28us (actually could be 10-40us)
   *             1         -> high volage within 70us (actually could be 60-85us)
   */
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            retryCount = 0;
            while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == 0)
            {
                if (retryCount > 75)
                {
                    uprintf(&huart4,"DHT22 timeout waiting for data!\r\n");
                }
                retryCount++;
                Delay_Us(1);
            }
            // We now wait for 40us
            Delay_Us(40);
            if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == 1)
            {
                // If pin is still high, bit value is a 1
                bitTimes[i*8+j] = 1;
            }
            else
            {
                // The bit value is a 0
                bitTimes[i*8+j] = 0;
            }
            uint8_t count = 0;
            while (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6) == 1 && count < 100)
            {
                Delay_Us(1); // Delay for 1 microsecond
                count++;
            }
        }
    }

    DHT22_PinConfig(GPIO_MODE_OUTPUT_PP);    // Re-init DHT22 pin
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET); //set value 1


    // Now bitTimes have the actual bits
    // that were needed to find the end of each data bit
    // Note: the bits are offset by one from the data sheet, not sure why
    currentHumidity    = 0;
    currentTemperature = 0;
    checkSum           = 0;
    // First 16 bits is Humidity
    for (i=0; i<16; i++)
    {
        if (bitTimes[i+1] > 0)
        {
            currentHumidity |= ( 1 << (15-i));
        }
    }

    // Second 16 bits is Temperature
    for (i=0; i<16; i ++)
    {
        if (bitTimes[i+17] > 0)
        {
            currentTemperature |= (1 <<(15-i));
        }
    }

    // Last 8 bit is Checksum
    for (i=0; i<8; i++)
    {
        if (bitTimes[i+33] > 0)
        {
            checkSum |= (1 << (7-i));
        }
    }

    lastHumidity1 = currentHumidity/10;
    lastHumidity2 = currentHumidity%10;

    // if first bit of currentTemperature is 1, it is negative value.
    if ((currentTemperature & 0x8000)==0x8000)
    {
        lastTemperature1 = (currentTemperature & 0x7FFF / 10) * -1;
        lastTemperature2 = currentTemperature%10;
    }
    else
    {
        lastTemperature1 = currentTemperature / 10.0;
        lastTemperature2 = currentTemperature%10;
    }

    // Calculate Check Sum
    csPart1 = currentHumidity >> 8;
    csPart2 = currentHumidity & 0xFF;
    csPart3 = currentTemperature >> 8;
    csPart4 = currentTemperature & 0xFF;
    if (checkSum == ((csPart1 + csPart2 + csPart3 + csPart4) & 0xFF))
    {
        uprintf(&huart4,"\x1b[4;1HTemperature: %d03.%d01 degC , Humidity:%d03.%d01 %%RH\r\n", lastTemperature1,lastTemperature2, lastHumidity1,lastHumidity2);
    }
}




 /* --------------------------------- End Of File ------------------------------ */
