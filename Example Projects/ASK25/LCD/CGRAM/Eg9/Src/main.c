/******************************************************************//**
* @file		main.c
* @brief	Program to display Edutech Logo with TECH
* @version	v1.0
* @date		Apr 16, 2015
* @author	Bhavin R. Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_global.h"
#include "stm32f4_discovery_uart.h"
//#include "stm32f4_discovery.h"
//#include "stm32f4_ask25.h"
#include "stm32f4_ask25_lcd.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  uint8_t Character1[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1};
  uint8_t Character2[8] = {0x0, 0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x1f};
  uint8_t Character3[8] = {0x7, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
  uint8_t Character4[8] = {0x18, 0x1c, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};

  uint8_t Character5[8] = {0x3, 0x7, 0xf, 0xf, 0x1f, 0x1f, 0x1f, 0x1e};
  uint8_t Character6[8] = {0x1e, 0x1f, 0x1b, 0x03, 0x1b, 0x1f, 0x1e,0x00};
  uint8_t Character7[8] = {0x13, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x18,0x1c};
  uint8_t Character8[8] = {0x07, 0x0f, 0x0f, 0x0e, 0x0e, 0x0c, 0x0c,0x18};
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

  /* initialize LCD */
  ASK25_LCD_Init(Inc, DispShiftOff);

  /* Generate Logo */
  ASK25_LCD_CGRAM_CharGen(0, Character1);
  ASK25_LCD_CGRAM_CharGen(1, Character2);
  ASK25_LCD_CGRAM_CharGen(2, Character3);
  ASK25_LCD_CGRAM_CharGen(3, Character4);
  ASK25_LCD_CGRAM_CharGen(4, Character5);
  ASK25_LCD_CGRAM_CharGen(5, Character6);
  ASK25_LCD_CGRAM_CharGen(6, Character7);
  ASK25_LCD_CGRAM_CharGen(7, Character8);

  /* Display EDUTECH Logo */
  ASK25_LCD_Display_Character(0x00,1,1,Off);
  ASK25_LCD_Display_Character(0x01,1,2,Off);
  ASK25_LCD_Display_Character(0x02,1,3,Off);
  ASK25_LCD_Display_Character(0x03,1,4,Off);

  ASK25_LCD_Display_Character(0x04,2,1,Off);
  ASK25_LCD_Display_Character(0x05,2,2,Off);
  ASK25_LCD_Display_Character(0x06,2,3,Off);
  ASK25_LCD_Display_Character(0x07,2,4,Off);

  ASK25_LCD_Display_String("TECH",2,5,Off);

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
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 210;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
