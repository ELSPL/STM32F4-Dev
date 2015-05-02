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

/**
 * @} GLOBAL_Public_Functions End
 */





/***********************************END OF FILE*********************************/
