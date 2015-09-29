/**
  ******************************************************************************
  * @file    udp_echoserver.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    29-September-2015
  * @brief   UDP protocol initialization(server)
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup STM32F4_DISCOVERY_ETH
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_ETH
  * @brief This file provides set of firmware functions for UDP protocol
  *
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup STM32F4_DISCOVERY_ETH_Private_define
 * @{
 */

#define UDP_SERVER_PORT    7   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    7   /* define the UDP remote connection port */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32F4_DISCOVERY_ETH_Private_functions
 * @{
 */


/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udp_echoserver_init(void)
{
   struct udp_pcb *upcb;
   err_t err;

   /* Create a new UDP control block  */
   upcb = udp_new();

   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);

      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      }
   }
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

  /* Connect to the remote client */
  udp_connect(upcb, addr, UDP_CLIENT_PORT);

  /* Tell the client that we have accepted it */
  udp_send(upcb, p);

  /* free the UDP connection, so we can accept new clients */
  udp_disconnect(upcb);

  /* Free the p buffer */
  pbuf_free(p);

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
