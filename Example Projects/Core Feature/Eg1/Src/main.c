/******************************************************************//**
* @file   main.c
* @brief  Write a program to Write and Read Byte of I2C based EEPROM (ASK-25A)
* @version  v1.0
* @date   Apr 16, 2015
* @author Bhavin R. Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_global.h"
#include "stm32f4_discovery_uart.h"
#include "stm32f4_discovery_vcp.h"
#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"




/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/**
   * @brief Bit band test function
   * @param Address : input address given by user
   * @param pin : Provide the bit number to modify
   */
void Bitband_test(uint32_t Address, uint32_t pin)
  {
    if (Address >= 0x20000000 && Address <= 0x2001FFFC)
    {
      uint32_t temp;

      /* test Bit-banding SRAM first */
      trace_printf("Test Bit band SRAM\n\r");
      /* Read the Value at Address */
      temp = (*(__IO uint32_t *)Address);
      /* Print Variable address with value */
      trace_printf("The value at address %08x: %08x\n\r",Address,temp);
      /* Read 0 bit of Address */
      temp = Bitband_Getbit(Address,pin);
      /* Print bit value of Address */
      trace_printf("The value of bit 0 of Address: %08x\n\r",temp);
      /* Clear the value of bit 0 of Address */
      Bitband_clearbit(Address,pin);
      /* Read the Value at Address */
      temp = (*(__IO uint32_t *)Address);
      /* Print bit value of Address */
      trace_printf("The value of Address after BITBAND_SRAM_ClearBit function: %08x\n\r",temp);
      /* Set the value of bit 0 of Address */
      Bitband_Setbit(Address,pin);
      /* Read the Value at Address */
      temp = (*(__IO uint32_t *)Address);
      /* Print bit value of Address */
      trace_printf("The value of Address after BITBAND_SRAM_SetBit function: %08x\n\r",temp);
      /* Read 0 bit of Address */
      temp = Bitband_Getbit(Address,pin);
      /* Print bit value of Address */
      trace_printf("The value of bit 0 of Address %08x:\n\r",temp);

    }
    else if((Address >= 0x40000000 && Address <= 0x40007FFC) || (Address >= 0x40010000 && Address <= 0x40057fff)
            ||(Address >= 0x40020000 && Address <= 0x4007FFFC))
    {
      while(1)
      {
        int i;
        if (Bitband_Getbit(Address,pin) == SET)
        {
          for(i=0;i<10;i++)
          {
            BSP_LED_Toggle(LED4);
            HAL_Delay(250);
          }
        }
        else
        {
          Bitband_Setbit(Address,pin);
        }
      }
    }
    else
    {
      trace_printf("Improper Address");
    }
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  uint32_t test;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  BSP_UART_Init(115200);

  /* Initialize on board LED4 */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);
  /********************************************************************************************//**
  * @brief  Can choose any address between 0x20000000 - 0x2001FFFC             |  for SRAM
  *
  *                                        0x40000000 - 0x40007FFC : APB1      |
  *                                        0x40010000 - 0x40057fff : APB2      | for Peripheral
  *                                        0x40020000 - 0x4007FFFC : AHB1      |
  *
  *           NOTE:
  *                    For Peripheral bit band testing, only GPIOA.0 pin [(((uint32_t )&(GPIOA->IDR)),0)]
  *                    is used or you can use on board LEDs for testing [(((uint32_t )&(GPIOD->BSRR)),13)]
  *************************************************************************************************/
  Bitband_test(((uint32_t )&(GPIOA->IDR)),0);

  trace_printf("Hello\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/*****END OF FILE****/
