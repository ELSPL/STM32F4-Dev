/**
  ******************************************************************************
  * @file    stm32f4_global.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains global variable definitions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#ifdef USE_STM32F4_RTC
#include "stm32f4_discovery_rtc.h"
#endif
/* Global Variables ----------------------------------------------------------- */
/** @addtogroup STM32F4_GLOBAL_Variables
 * @{
 */

uint32_t priviledge_status;
/**
 * @} STM32F4_GLOBAL_Variables End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup GLOBAL_Public_Functions
 * @{
 */

/********************************************************************//**
* @brief Bit-Banding Functions
**********************************************************************/
/**
 * @brief This function Reads bit value at given address
 * @param Address   SRAM or Peripheral Address
 *                  Can choose any address between :
 *                  0x20000000 - 0x2001FFFC             |  for SRAM
 *                  0x40000000 - 0x40007FFC : APB1      |
 *                  0x40010000 - 0x40057fff : APB2      | for Peripheral
 *                  0x40020000 - 0x4007FFFC : AHB1      |
 *                  or Use Register Address: (uint32_t)&(GPIOD->BSRR)
 * @param pin       Pin Value from Word
 * @return  Bit Value
 */
uint32_t Bitband_Getbit(uint32_t Address, uint8_t pin)
{
    uint32_t test = 0;
    if (Address >= 0x20000000 && Address <= 0x2001FFFC)
    {
        test = BITBAND_SRAM_GetBit(Address, pin);
    }
    else if((Address >= 0x40000000 && Address <= 0x40007FFC) || (Address >= 0x40010000 && Address <= 0x40057fff)
            ||(Address >= 0x40020000 && Address <= 0x4007FFFC))
    {
        test = BITBAND_Peri_GetBit(Address, pin);
    }
    return test;
}


/**
 * @brief This function Sets bit value at given address
 * @param Address   SRAM or Peripheral Address
 *                  Can choose any address between :
 *                  0x20000000 - 0x2001FFFC             |  for SRAM
 *                  0x40000000 - 0x40007FFC : APB1      |
 *                  0x40010000 - 0x40057fff : APB2      | for Peripheral
 *                  0x40020000 - 0x4007FFFC : AHB1      |
 *                  or Use Register Address: (uint32_t)&(GPIOD->BSRR)
 * @param pin       Pin Value from Word
 * @return  None
 */
void Bitband_Setbit(uint32_t Address, uint8_t pin)
{
    if (Address >= 0x20000000 && Address <= 0x2001FFFC)
    {
        BITBAND_SRAM_SetBit(Address, pin);
    }
    else if((Address >= 0x40000000 && Address <= 0x40007FFC) || (Address >= 0x40010000 && Address <= 0x40057fff)
                ||(Address >= 0x40020000 && Address <= 0x4007FFFC))
    {
        BITBAND_Peri_SetBit(Address, pin);
    }
}


/**
 * @brief This function Clears bit value at given address
 * @param Address   SRAM or Peripheral Address
 *                  Can choose any address between :
 *                  0x20000000 - 0x2001FFFC             |  for SRAM
 *                  0x40000000 - 0x40007FFC : APB1      |
 *                  0x40010000 - 0x40057fff : APB2      | for Peripheral
 *                  0x40020000 - 0x4007FFFC : AHB1      |
 *                  or Use Register Address: (uint32_t)&(GPIOD->BSRR)
 * @param pin       Pin Value from Word
 * @return  None
 */
void Bitband_clearbit(uint32_t Address, uint8_t pin)
{
    if (Address >= 0x20000000 && Address <= 0x2001FFFC)
    {
        BITBAND_SRAM_ClearBit(Address, pin);
    }
    else if((Address >= 0x40000000 && Address <= 0x40007FFC) || (Address >= 0x40010000 && Address <= 0x40057fff)
                ||(Address >= 0x40020000 && Address <= 0x4007FFFC))
    {
        BITBAND_Peri_ClearBit(Address, pin);
    }
}

/*********************************************************************************/
/* @brief MPU Functions */
/**********************************************************************************/

/**
 * @brief MPU enable function
 */
void MPU_Enable(void)
{
    MPU->CTRL = MPU_CTRL_ENABLE_Msk|MPU_CTRL_PRIVDEFENA_Msk ;
}


/**
 * @brief MPU disable function
 */
void MPU_Disable(void)
{
    __DMB();       // Make sure outstanding transfers are done
    MPU->CTRL = 0; // Disable the MPU
}


/**
  * @brief This function creates MPU region
  * @param region_num  provide region number, From 0 to 7(Maximum)
            @arg1      Region 0 - Code:            Flash      0x0800 0000 - 0x080F FFFF       1MB
            @arg2      Region 1 - On chip SRAM :   SRAM       0x2000 0000 - 0x2001 FFFF       128KB
            @arg3      Region 2 - On chip SRAM:    SRAM       0x2000 2000 - 0x2000 2020       32B (unaccessible)
            @arg4      Region 3 - APB Peripherals:            0x4000 0000 - 0x4001 FFFF       128KB
  * @param addr        This is the starting address of specified region
            @arg1      FLASH_ADDRESS_START
            @arg2      RAM1_ADDRESS_START
            @arg3      RAM2_ADDRESS_START
            @arg4      PERIPH_ADDRESS_START
  * @param size        This is size of the region that is used in MPU.
            @arg1      FLASH_SIZE
            @arg2      RAM1_SIZE
            @arg3      RAM2_SIZE
            @arg4      PERIPH_SIZE
  * @param attr        Specify attribute of the region
  *         @arg1      MPU_NO_ACCESS
            @arg2      MPU_REGION_PRIVILEGED_READ_WRITE
            @arg3      MPU_REGION_READ_WRITE
            @arg4      MPU_REGION_PRIVILEGED_READ_ONLY
            @arg5      MPU_REGION_READ_ONLY
  */
void MPU_Region_Config(uint8_t region_num, uint32_t addr, uint32_t size, uint32_t attr)
{
    MPU->RNR = region_num;
    MPU->RBAR = addr;
    MPU->RASR = size | attr;
}
/**
 * @brief MPU region configuration function
 */
void MPU_Config(void)
{
  MPU_Disable();

  /* Configure FLASH region as REGION N0, 1MB of size and R/W region */
  MPU_Region_Config(0,FLASH_ADDRESS_START,FLASH_SIZE,MPU_REGION_PRIVILEGED_READ_WRITE);

  /* Configure RAM region as Region N1, 128kB of size and R/W region */
  MPU_Region_Config(1,RAM1_ADDRESS_START,RAM1_SIZE,MPU_REGION_PRIVILEGED_READ_WRITE);

  /* Configure small RAM region as REGION N2, 32Byte of size, no access region and Execute Never region */
  MPU_Region_Config(2,RAM2_ADDRESS_START,RAM2_SIZE,MPU_NO_ACCESS);

  /* Configure Peripheral region as REGION N3, 128KB of size, R/W region */
  MPU_Region_Config(3,PERIPH_ADDRESS_START,PERIPH_SIZE,MPU_REGION_PRIVILEGED_READ_WRITE);


  /* Enable the memory fault exception */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

  /* Enable MPU */
  MPU_Enable();
}

/********************************************************************/
/* @brief Low Power Configuration Function                          */
/********************************************************************/

/**********************************************************************************
 *   In Stop mode, all clocks in the 1.2V domain are stopped, the PLL, the HSI,
 *   and the HSE RC oscillators are disabled. Internal SRAM and register contents
 *   are preserved.
 **********************************************************************************/

/**
 * @brief Low power sleep mode
 * NOTE: This is Low power sleep mode, system can be wake up using any interrupt
 */
void BSP_SleepMode(void)
{
  /* suspand systick */
  HAL_SuspendTick();

/*
 *  Enters Sleep mode
 *  PWR_LOWPOWERREGULATOR_ON: SLEEP mode with low power regulator ON
 *  PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
 */
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_SLEEPENTRY_WFI);

  /* Resume systick */
  HAL_ResumeTick();
}

/*********************************************************************************
 *  The Standby mode allows to achieve the lowest power consumption. It is based
 *  on the Cortex-M4 deep sleep mode, with the voltage regulator disabled.
 *  The 1.2V domain is consequently powered off. The PLL, the HSI oscillator and
 *  the HSE oscillator are also switched off. SRAM and register contents are lost
 *  except for the RTC registers, RTC backup registers, backup SRAM and Standby
 *  circuitry.
 *********************************************************************************/
#ifdef USE_STM32F4_RTC
/**
 * @brief Low Power standby mode
 * @param WakeUptime_ms provide time in 1 to 10 millisecond
 *                      and 1 second to  60 seconds (in msec unit)
 * NOTE: Auto-wake-up (AWU) from low-power mode using RTC wakeup timer
 *       After RTC wakeup timer interrupt the system will reset
 */
void BSP_StandbyMode_AWU(uint16_t WakeUptime_ms)
{
  /* Initalize RTC */
  BSP_RTC_Init();

  /* Start RTC wakeup timer */
  BSP_RTC_WakeUpTimer_Init(WakeUptime_ms);

 /**
  *  Clear all related wakeup flags,
  *  Re-enable all used wakeup sources,
  *  Enter the Standby mode.
  */
  /* Clear RTC wakeup flag */
  __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc_bsp,RTC_FLAG_WUTF);
  /* Restart RTC wakeup timer */
  BSP_RTC_WakeUpTimer_Init(WakeUptime_ms);

  /* Enter Standby mode */
  HAL_PWR_EnterSTANDBYMode();
}
#endif

/**
 * @brief Low Power standby mode
 * NOTE: This is Low power standby mode with wakeup button(PA0)
 *       After WAKEUP button interrupt the system will reset
 */
void BSP_StandbyMode_PB(void)
{
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

  /* Clear standby and wakeup flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB | PWR_FLAG_WU);
  HAL_PWR_EnterSTANDBYMode();
}

/********************************************************************/
/* @brief Mode Privilege Function                                   */
/********************************************************************/

/**
 * To change mode we need to be privilege first
 * If thread mode is in un-privilege level so we cannot execute privilege/system commands.
 * By default exception handlers are in privilege level so through
 * SVC exception we will change the Thread mode to Privilege level.
 */

/**
 * @brief  SVC function to change the Thread mode to Privilege level
 */
void __SVC()
{
  __ASM volatile ("svc 0x01");
}

/**
 * @brief Change mode to privilege handler
 */
void SVC_Handler(void)
{
  /* Change Thread mode to privileged */
  __set_CONTROL(2);
}

/**
 * @brief This function check the privilege mode status
 * @return priviledge_status
 */
uint8_t BSP_Check_Priviledge_Status(void)
{
  priviledge_status = __get_CONTROL();
  return (uint8_t)(priviledge_status & ~(THREAD_PRIVILEDGED_MASK));
}

/**
 * @brief Change the mode privilege to unprivilege
 */
void BSP_Set_UnprivilegeMode(void)
{
  __set_CONTROL((priviledge_status & THREAD_PRIVILEDGED_MASK)|THREAD_UNPRIVILEGED);
}
/**
 * @} GLOBAL_Public_Functions End
 */

/***********************************END OF FILE*********************************/
