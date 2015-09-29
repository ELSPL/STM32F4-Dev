/**
  ******************************************************************************
  * @file    udp_echoclient.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    29-September-2015
  * @brief   Header file for udp_echoclient.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDP_ECHOCLIENT_H__
#define __UDP_ECHOCLIENT_H__

/** @addtogroup STM32F4_DISCOVERY_ETH
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup STM32F4_DISCOVERY_ETH_Exported_Constants
 * @{
 */

#define DEST_IP_ADDR0   192
#define DEST_IP_ADDR1   168
#define DEST_IP_ADDR2   0
#define DEST_IP_ADDR3   82

#define UDP_SERVER_PORT    7   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    7   /* define the UDP remote connection port */

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup STM32F4_DISCOVERY_ETH_Exported_functions
 * @{
 */
void udp_echoclient_connect(void);
void udp_echoclient_send(void);

/**
  * @}
  */

/**
  * @}
  */
#endif /* __UDP_ECHOCLIENT_H__ */
