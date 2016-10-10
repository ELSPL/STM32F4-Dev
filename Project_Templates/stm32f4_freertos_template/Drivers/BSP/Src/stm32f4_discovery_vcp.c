/**
  ******************************************************************************
  * @file    stm32f4_discovery_vcp.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file provides set of firmware functions to manage VCP as an
  *          alternative to STDIO functions.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_vcp.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_VCP
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_VCP
  * @brief This file provides set of firmware functions to manage VCP as an
  *        alternative to STDIO functions such as printf and scanf.
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_VCP_Exported_Handle
 * @{
 */

uint8_t vEscFlag = 0;

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_VCP_Private_Functions
  * @{
  */

/**
 * @brief VT100- code to set cursor to Home
 * @return None
 */
static void vcp_reset_cursor (void)
{
  vuprintf("\x1b[H");   /* escape sequence for vt220 ^[H sets cursor to Home */
}


/**
 * @brief VT100- code to Clear Screen
 * @return  None
 */
static void vcp_clear_screen (void)
{
  vuprintf("\x1b[2J");   /* escape sequence for vt220 ESC[2J clears screen */
}

/**
  * @}
  */

#ifdef USE_STM32F4_VCP
/** @defgroup STM32F4_DISCOVERY_VCP_Functions
  * @{
  */
void BSP_VCP_Init (void)
{
  USBD_Init(&USBD_Device, &VCP_Desc, 0);

  USBD_RegisterClass(&USBD_Device, &USBD_CDC);
  USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_Template_fops);
  USBD_Start(&USBD_Device);
}

#endif /* USE_STM32F4_VCP */

/**
 * @brief The vgetche() function returns the next character read from the
 *        console and echoes that character to the screen.Characters from
 *        space(20hex) to (7E) are echo to the screen.The vgetche() function
 *        is not define by the ANSI C standard.
 * @param mode  @arg  NONE_BLOCKING
 *              @arg  BLOCKING
 * @return Character or format
 */
uint8_t vgetche(TRANSFER_BLOCK_Type mode)
{
  uint8_t key[1], status=0;

  if(mode == NONE_BLOCKING)
  {
    status = VCP_Read(key, 1);
  }
  else if(mode == BLOCKING)
  {
    while(status != 1)
    {
      status = VCP_Read(key, 1);
    }
  }

  /* Got some data */
  if(status == 1)
  {
    if (key[0] == In_CR)
    {
      return(key[0]);
    }
    else if (key[0] == In_DELETE || key[0] == In_BACKSPACE)
    {
      return(key[0]);
    }
    else if (key[0] == In_ESC)
    {
      vEscFlag = 1;
      return (0);
    }
    else if (key[0] >= ' ')
    {
      return (key[0]);
    }
    else
    {
      VCP_Write(&key[0], 1);
    }
  }
  return(0);
}


/**
 * @brief One line editor
 * @param s     Pointer to Received buffer
 * @param lim   Length of Received buffer
 * @return  Character count
 */
uint8_t vget_line(int8_t s[], uint8_t lim)
{
  int8_t kb;                 /* input character */

  uint8_t pointer=0;          /* Pointer in buffer */
  uint8_t count=0;            /* character count */
  uint8_t temp;

  while(1)
  {
    kb = vgetche(BLOCKING);

    if (kb == In_ESC)                /* if ESCAPE pressed then exit */
    {
      return (kb);
    }

    else if (kb == In_CR)            /* CARRIAGE return ? */
    {
      s[pointer] = '\0';             /* put null char on last position */
      break;                         /* yes, exit from this loop */
    }

    else if(kb == In_DELETE || kb == In_BACKSPACE)
    {
      if(pointer==0)                 /* any characters entered */
      {
        continue;                    /* no, so get another character */
      }

      temp = Out_BACKSPACE;
      VCP_Write(&temp, 1);   /* go back one position */
      temp = Out_SPACE;
      VCP_Write(&temp, 1);   /* erase char */
      temp = Out_BACKSPACE;
      VCP_Write(&temp, 1);   /* go back one more position */

      pointer--;                     /* decrement pointer */
      count--;                       /* decrement character count */

      continue;
    }

    else if(pointer < lim)
    {
      s[pointer] = kb;               /* save character and increment pointer */
      pointer++;
      count++;                       /* increment count */
      VCP_Write(&kb, 1);    /* echo character */

      continue;                      /* and get some more */
    }
    else
    {
      temp = In_BELL;
      VCP_Write(&temp, 1);   /* ring the bell */
    }
  }
  return(count);
}


/**
 * @brief Modified version of Standard Printf statement
 * @paragraph Supports standard formats "%c %s %d %x"
 *            "%d" and "%x" requires non-standard qualifiers,"%dfn, %xfn":-
 *            f supplies a fill character
 *            n supplies a field width
 *
 *        Supports custom formats  "%b  %u %t %y %a"
 *        "%b"  prints a 2 digit BCD value with leading zero
 *        "%u"  prints the 16 bit unsigned integer in hex format
 * @param format  Character format
 * @param ...     <multiple argument>
 * @return  return with valid character or nothing
 */
HAL_StatusTypeDef vuprintf(const char *format, ...)
{
  uint8_t hex[]= "0123456789ABCDEF";
  uint32_t width_dec[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
  uint32_t width_hex[10] = { 0x1, 0x10, 0x100, 0x1000, 0x10000, 0x100000, 0x1000000, 0x10000000};
  uint32_t temp;

  int8_t format_flag, fill_char;
  uint32_t u_val, div_val;
  uint16_t base;

  int8_t *ptr;

  va_list ap;
  va_start(ap, format);

  for(;;)
  {
    while((format_flag = *format++) != '%')      /* until full format string read */
    {
      if(!format_flag)
      {                        /* until '%' or '\0' */
        return (HAL_OK);
      }
      VCP_Write(&format_flag, 1);
    }

    switch(format_flag = *format++)
    {
      case 'c':
        format_flag = va_arg(ap, int32_t);
        VCP_Write(&format_flag, 1);

        continue;

      default:
        VCP_Write(&format_flag, 1);

        continue;

      case 'b':
        format_flag = va_arg(ap,int32_t);
        VCP_Write(&(hex[(uint16_t)format_flag >> 4]), 1);
        VCP_Write(&(hex[(uint16_t)format_flag & 0x0F]), 1);

        continue;

      case 's':
        ptr = va_arg(ap, int8_t *);
        while(*ptr)
        {
          VCP_Write(&(*ptr++), 1);
        }

        continue;

      case 'u':
        base = 16;
        div_val = 0x100000;
        u_val = va_arg(ap, uint32_t);
        do
        {
          VCP_Write(&(hex[u_val/div_val]), 1);
          u_val %= div_val;
          div_val /= base;
        }while(div_val);

        continue;

      case 'd':
        base = 10;
        fill_char = *format++;
        format_flag = ( *format++ ) - '1';
        div_val = width_dec[format_flag];
        u_val = va_arg(ap,int32_t);

        if(((int)u_val) < 0)
        {
          u_val = - u_val;    /* applied to unsigned type, result still unsigned */
          temp = '-';
          VCP_Write(&temp, 1);
        }

        goto  CONVERSION_LOOP;

      case 'x':
        base = 16;
        fill_char = *format++;
        format_flag = (*format++) - '1';
        div_val = width_hex[format_flag];
        u_val = va_arg(ap, int32_t);

        CONVERSION_LOOP:
        while(div_val > 1 && div_val > u_val)
        {
          div_val /= base;
          VCP_Write(&fill_char, 1);
        }

        do
        {
          VCP_Write(&(hex[u_val/div_val]), 1);
          u_val %= div_val;
          div_val /= base;
        }while(div_val);
    }/* end of switch statement */
  }
  return(HAL_OK);
}


/********************************************************************//**
* @brief VCP Utility functions
**********************************************************************/

/**
 * @brief VT100- code to Clear Screen and Reset Cursor
 * @return  None
 */
void vcp_clr_scr_rst_cur (void)
{
  vcp_clear_screen();
  vcp_reset_cursor();
}


/**
 * @brief Erase Character
 * @return  None
 */
void vErase_Char (void)
{
  vuprintf("%c", Out_BACKSPACE);
  vuprintf("%c", Out_SPACE);
  vuprintf("%c", Out_BACKSPACE);
}


/**
 * @brief Erase Character with Underscore '_'
 * @return  None
 */
void vErase_Char_With_UnderScore (void)
{
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf("_");                       /* and write '_' on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
}


/**
 * @brief Erase Backslash
 * @return None
 */
void vErase_Backslash (void)
{
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf("/");                       /* and write '/' on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf("_");                       /* and write '_' on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
}


/**
 * @brief Erase Semicolon
 * @return  None
 */
void vErase_SemiColon (void)
{
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf(":");                       /* and write ':' on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
  vuprintf("_");                       /* and write '_' on the screen */
  vuprintf("%c", Out_BACKSPACE);       /* erase character on the screen */
}


/**
 * @brief Erase and Ring the Bell
 * @return  None
 */
void vErase_And_RingTheBell (void)
{
  vuprintf("%c", Out_BACKSPACE);         /* back space */
  vuprintf("_");                         /* erase 1st char on the screen */
  vuprintf("%c", Out_BACKSPACE);         /* back space */
  vuprintf("_");                         /* erase 2nd char on the screen */
  vuprintf("%c", Out_BACKSPACE);         /* back space */
  vuprintf("\7");                        /* ring the bell */
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
