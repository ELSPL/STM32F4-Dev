/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "stm32f4_discovery_timer.h"
#include "stm32f4_discovery_vcp.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
void configureTimerForRunTimeStats(void)
{
  BSP_TIM_Basic_Config(&htim6_basic,20000);
}

unsigned long getRunTimeCounterValue(void)
{
  uint32_t tcvalue;
  tcvalue = __HAL_TIM_GetCounter(&htim6_basic);
  return tcvalue;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */

  BSP_SleepMode();
}
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
  trace_printf("DIE:ERROR: Task \"%s\" overflowed its stack\n\r", pcTaskName);
  taskDISABLE_INTERRUPTS();
  while (1);
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
  trace_printf("DIE:ERROR: Malloc Failure!\r\n");
  taskDISABLE_INTERRUPTS();
  while (1);
}
/* USER CODE END 5 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  static portCHAR buff[900] = {0};
  unsigned portCHAR ch=0;
  uint8_t flag = 0;

  vuprintf("\x1b[1;1HPress any of the following keys for the corresponding functionality: ");
  vuprintf("\n\r1: To call vTaskList() and display current task status ");
  vuprintf("\n\r2: To call vTaskGetRunTimeStats() and to display execution time\n\r");

  while(1)
  {
    if(flag == 0)
    {
      ch = 0;

      ch = vgetche(NONE_BLOCKING);
      if(ch & 0x3)
      {
        vuprintf("\x1b[2J"); //clear screen
        vuprintf("\x1b[H");  //reset cursor
      }
    }
    switch(ch)
    {
    case '1':
      osThreadList( (signed char *) buff );
      vuprintf("\x1b[1;1H\n\rThe current task list is as follows....");
      vuprintf("\n\r----------------------------------------------");
      vuprintf("\n\rName          State  Priority  Stack   Number");
      vuprintf("\n\r----------------------------------------------\n\r");
      vuprintf(buff);
      vuprintf("\n\r----------------------------------------------\n\r");
      vuprintf("Press ESC key to escape functionality\n\r");
      flag = 1;
      break;

    case '2':
      vTaskGetRunTimeStats((signed char *) buff);
      vuprintf("\x1b[1;1H\n\rThe current task list is as follows....");
      vuprintf("\n\r----------------------------------------------");
      vuprintf("\n\rTask          Abs Time(in ms)   %c Time       ",'%');
      vuprintf("\n\r----------------------------------------------\n\r");
      vuprintf(buff);
      vuprintf("\n\r----------------------------------------------\n\r");
      vuprintf("Press ESC key to escape functionality\n\r");
      flag = 1;
      break;

    case 'e':
      _exit(0);
      break;

    default:
      break;
    }
    vgetche(NONE_BLOCKING);
    if(vEscFlag)
    {
      vEscFlag=0;
      flag=0;
      vuprintf("\x1b[2J"); //clear screen
      vuprintf("\x1b[H");  //reset cursor
      vuprintf("\x1b[1;1HPress any of the following keys for the corresponding functionality: ");
      vuprintf("\n\r1: To call vTaskList() and display current task status ");
      vuprintf("\n\r2: To call vTaskGetRunTimeStats() and to display execution time\n\r");
    }
    osDelay(400);  // should be less than other tasks execution
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
