/**
  ******************************************************************************
  * @file    stm32f4_global.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains global macros and typedefs.
  *          It is intended to be used in ISO C conforming development
  *          environments and checks for this insofar as it is possible to do so.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_GLOBAL_H
#define __STM32F4_GLOBAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @defgroup STM32F4_GLOBAL_Macros
* @{
*/

#ifndef ENABLE
#define ENABLE      1
#endif
#ifndef DISABLE
#define DISABLE     0
#endif

/* _BIT(n) sets the bit at position "n"
* _BIT(n) is intended to be used in "OR" and "AND" expressions:
* e.g., "(_BIT(3) | _BIT(7))".
*/
#undef _BIT
/* Set bit macro */
#define _BIT(n) (1<<n)

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
* _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
* e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
*/
#undef _SBF
/* Set bit field macro */
#define _SBF(f,v) (v<<f)

/* _BITMASK constructs a symbol with 'field_width' least significant
* bits set.
* e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
* The symbol is intended to be used to limit the bit field width
* thusly:
* <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
* If "any_expression" results in a value that is larger than can be
* contained in 'x' bits, the bits above 'x - 1' are masked off.  When
* used with the _SBF example above, the example would be written:
* a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
* This ensures that the value written to a_reg is no wider than
* 16 bits, and makes the code easier to read and understand.
*/
#undef _BITMASK
/* Bitmask creation macro */
#define _BITMASK(field_width) ( _BIT(field_width) - 1)


/* NULL pointer */
#ifndef NULL
#define NULL ((void*) 0)
#endif

/* FALSE pointer */
#ifndef FALSE
#define FALSE   (0)
#endif

/* TRUE pointer */
#ifndef TRUE
#define TRUE    (1)
#endif

/* YES pointer */
#ifndef YES
#define YES     (1)
#endif

/* NO pointer */
#ifndef NO
#define NO      (0)
#endif

/* ON pointer */
#ifndef ON
#define ON      (1)
#endif

/* OFF pointer */
#ifndef OFF
#define OFF     (0)
#endif

/* TOP pointer */
#ifndef TOP_LINE
#define TOP_LINE    (0)
#endif

/* BOTTOM pointer */
#ifndef BOTTOM_LINE
#define BOTTOM_LINE (1)
#endif

/* FORWARD pointer */
#ifndef FORWARD
#define FORWARD     (1)
#endif

/* REVERSE pointer */
#ifndef REVERSE
#define REVERSE     (0)
#endif

/* CAPITALISE MACRO */
#ifndef to_upper
#define to_upper(c)   ('a' <= (c) && (c) <= 'z' ? (c)-32 : (c))
#endif

/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof (array) / sizeof (array[0]))

/* Static data/function define */
#define STATIC static
/* External data/function define */
#define EXTERN extern

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))


/********************************************************************//**
* @brief Bit-Banding Macro Functions
**********************************************************************/
/******************************************************************************/
/*                      Bit Banding declarations                              */
/******************************************************************************/
/* The processor memory map includes two bit-band regions. These occupy the lowest
* 1MB of the SRAM and peripheral memory regions respectively.
* + SRAM: Bit-band region:      0x20000000 - 0x200FFFFF
*   Bit-band alias:             0x22000000 - 0x23FFFFFF
* + PERI: Bit-band region:      0x40000000 - 0x400FFFFF
*   Bit-band alias:             0x42000000 - 0x43FFFFFF
* The mapping formula:
* bit_word_offset = (byte_offset * 32) + (bit_number * 4)
* bit_word_address = bit_band_base + byte_offset
* where:
* + bit_word_offset: the position of the target bit in the bit-band memory region
* + bit_word_addr: the address of the word in the alias memory region that maps to the target bit
* + bit_band_base: the starting address of the alias region
* + byte_offset: the number of byte in the bit-band region that contains the targeted bit
* + bit_number: is the bit position (0-7) of the targeted bit
*
* Note:
* The SRAM bit band accessible area on STM32F407 is available in two ranges:
* + 0x20000000 - 0x2001BFFF: for SRAM1 Base
* + 0x2001C000 - 0x2001FFFC: for SRAM2 Base
*
* The Peripheral bit band accessible are on STM32F407 is available in three range:
* + 0x40000000 - 0x40007FFC : APB1
* + 0x40010000 - 0x40057fff : APB2
* + 0x40020000 - 0x4007FFFC : AHB1
*
*/

/** SRAM bit band region address calculation */
#define BITBAND_SRAM(VarAddr,bitnumber) ((SRAM_BB_BASE + ((VarAddr-SRAM_BASE)<<5) + (bitnumber<<2)))

/* Basic bit band function definitions */
#define BITBAND_SRAM_ClearBit(VarAddr,bitnumber)(*(__IO uint32_t *) (BITBAND_SRAM(VarAddr,bitnumber)) = 0)
#define BITBAND_SRAM_SetBit(VarAddr,bitnumber) (*(__IO uint32_t *) (BITBAND_SRAM(VarAddr,bitnumber)) = 1)
#define BITBAND_SRAM_GetBit(VarAddr,bitnumber) (*(__IO uint32_t *) (BITBAND_SRAM(VarAddr,bitnumber)))

/** Peripheral bit band region address calculation */
#define BITBAND_Peri(VarAddr,bitnumber)((PERIPH_BB_BASE + ((VarAddr-PERIPH_BASE)<<5) + (bitnumber<<2)))
/* Basic bit band function definitions */
#define BITBAND_Peri_ClearBit(VarAddr,bitnumber)(*(__IO uint32_t *) (BITBAND_Peri(VarAddr,bitnumber)) = 0)
#define BITBAND_Peri_SetBit(VarAddr,bitnumber) (*(__IO uint32_t *) (BITBAND_Peri(VarAddr,bitnumber)) = 1)
#define BITBAND_Peri_GetBit(VarAddr,bitnumber) (*(__IO uint32_t *) (BITBAND_Peri(VarAddr,bitnumber)))



/*******************************************************************************//**
* @brief MPU Macro Functions
***********************************************************************************/
/**********************************************************************************/
/*                                Memory Regions                                  */
/**********************************************************************************/
/*

Region 0 - Code:            Flash               0x0800 0000 - 0x080F FFFF       1MB
Region 1 - On chip SRAM :   SRAM                0x2000 0000 - 0x2001 FFFF       128KB
Region 2 - On chip SRAM:                        0x2000 2000 - 0x2000 2020       32B (unaccessible)
Region 3 - APB Peripherals:                     0x4000 0000 - 0x4001 FFFF       128KB

*/

/** MPU region size definition */

#define MPU_DEFS_RASR_SIZE_32B (0x04 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_64B (0x05 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_128B (0x06 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_256B (0x07 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_512B (0x08 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_1KB (0x09 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_2KB (0x0A << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_4KB (0x0B << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_8KB (0x0C << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_16KB (0x0D << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_32KB (0x0E << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_64KB (0x0F << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_128KB (0x10 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_256KB (0x11 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_512KB (0x12 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_1MB (0x13 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_2MB (0x14 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_4MB (0x15 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_8MB (0x16 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_16MB (0x17 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_32MB (0x18 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_64MB (0x19 << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_128MB (0x1A << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_256MB (0x1B << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_512MB (0x1C << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_1GB (0x1D << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_2GB (0x1E << MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_4GB (0x1F << MPU_RASR_SIZE_Pos)

/** Region Definition */

//Region 1
#define FLASH_ADDRESS_START                      (0x08000000)
#define FLASH_SIZE                               (0x27UL << 0UL)
//Region 2
#define RAM1_ADDRESS_START                       (0x20000000UL)
#define RAM1_SIZE                                (0x19UL << 0UL)
//Region 3
#define RAM2_ADDRESS_START                       (0x20002000UL)
#define RAM2_SIZE                                (0x09UL << 0UL)
//Region 4
#define PERIPH_ADDRESS_START                     (0x40000000)
#define PERIPH_SIZE                              (0x19UL << 0UL)

/** Region Access Definition */

#define MPU_NO_ACCESS                            (0x00UL << MPU_RASR_AP_Pos)
#define MPU_REGION_PRIVILEGED_READ_WRITE         (0x01UL << MPU_RASR_AP_Pos)
#define MPU_REGION_READ_WRITE                    (0x03UL << MPU_RASR_AP_Pos)
#define MPU_REGION_PRIVILEGED_READ_ONLY          (0x05UL << MPU_RASR_AP_Pos)
#define MPU_REGION_READ_ONLY                     (0x06UL << MPU_RASR_AP_Pos)

/**
* @}
*/


/** @addtogroup Exported_types
 * @{
 */

/**
* @brief Boolean Type definition
*/

/**
* Read/Write transfer type mode (Block or non-block)
*/
typedef enum
{
 NONE_BLOCKING = 0,    /**< None Blocking type */
 TIME_BLOCKING,      /**< Time Blocking type*/
 BLOCKING        /**< Blocking type */
} TRANSFER_BLOCK_Type;


/** Pointer to Function returning Void (any number of parameters) */
typedef void (*PFV)();

/** Pointer to Function returning int32_t (any number of parameters) */
typedef int32_t(*PFI)();

/**
 * @}
 */


/** @defgroup STM32F4_GLOBAL_Variables
* @{
*/
//extern int variable_name;

/**
* @}
*/


/* Public Functions ----------------------------------------------------------- */
/** @defgroup GLOBAL_Public_Functions
 * @{
 */

/********************************************************************//**
* @brief Bit-Banding Function Declaration
**********************************************************************/
uint32_t Bitband_Getbit(uint32_t Address, uint8_t pin);
void Bitband_Setbit(uint32_t Address, uint8_t pin);
void Bitband_clearbit(uint32_t Address, uint8_t pin);

/********************************************************************//**
* @brief MPU Function Declaration
**********************************************************************/
void MPU_Enable(void);
void MPU_Disable(void);
void MPU_Region_Config(uint8_t region_num, uint32_t addr, uint32_t size, uint32_t attr);
void MPU_Config(void);

/**
 * @} GLOBAL_Public_Functions End
 */




#ifdef __cplusplus
}
#endif
#endif /* __STM32F4_GLOBAL_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
