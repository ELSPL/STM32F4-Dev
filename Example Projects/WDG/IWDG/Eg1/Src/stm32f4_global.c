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

/* Global Variables ----------------------------------------------------------- */
/** @addtogroup STM32F4_GLOBAL_Variables
 * @{
 */

/**
 * @} STM32F4_GLOBAL_Variables End
 */
uint8_t WwdgFlag = RESET;

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
/** @brief MPU Functions */
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

/**
 * @} GLOBAL_Public_Functions End
 */





/***********************************END OF FILE*********************************/
