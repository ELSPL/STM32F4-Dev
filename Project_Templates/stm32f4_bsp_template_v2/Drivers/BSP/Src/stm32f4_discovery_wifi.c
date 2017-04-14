/******************************************************************//**
* @file   stm32f4_discovery_wifi.c
* @brief  This file contains implementation of STM32F4 WIFI in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_wifi.h"
#include "stm32f4_discovery_uart.h"
#include <string.h>
/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WIFI
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WIFI_Public_Types
 * @{
 */
uint8_t RxBuffer[RX_BUFFER_SIZE];
uint32_t RxBufferTail = 0;
uint32_t RxBufferHead = 0;

static uint8_t tempReTXBuf[SEND_BUF_SIZE];
int32_t tempReTXLen;

RX_Info_Typedef rx_frame[NUM_RX_BUF];

static Serial_RX_State_Typedef serial_rx_state;
static int32_t curr_buf_idx = 0;
static int32_t curr_read_idx = 0;
static int32_t rx_frm_payload_index;
static int32_t rx_frm_data_index;
static uint8_t rx_frm_chksum;

extern int32_t buf_pos;
extern uint8_t RxBuffer[];
extern int32_t ack_recv_status;
extern int8_t mysock;
extern int8_t sockConnected;
extern int8_t sockClosed;

uint16_t timeout = 10000;

uint8_t totalscan = 0;
Scanlist_Typedef sl[30];
int8_t SSID[20];
int32_t SSIDlen = 0;
uint8_t secMode = 0;
uint8_t APOnOff = 1;
int8_t secKey[64];
uint8_t Keylen = 0;

int32_t joinok = 0;
int32_t ipok = 0;
uint32_t selfIP = 0;
uint16_t pktcnt = 0;

int8_t domain[100];
int8_t Portstr[8]={0};
int8_t IPstr[32]={0};
int8_t out[256];

uint8_t DBGU_InputReady = 0;
uint8_t quit_flag = 0;

uint8_t key;
uint8_t seqNo = 0;

int8_t mysock = -1;
uint8_t TxBuf[TEST_BUFFERSIZE];
int8_t uri[100]={0};
int8_t sockConnected = -1;
int8_t sockClosed = -1;
int32_t timeout1 = 5;

#define HTTP_RSP_STR "HTTP/1.1 200 OK\r\n" \
    "Content-Type: text/html\r\n" \
    "Connection: close\r\n" \
    "Content-Length: 47\r\n\r\n" \
    "<html><body>Hello from SN8200 #%2d</body></html>\r\n\r\n"

#define GET_REQUEST \
    "GET / HTTP/1.1\r\n" \
    "Host: 192.168.2.125\r\n" \
    "Accept: text/html\r\n" \
    "\r\n"

uint8_t IsWIFIGetStatusResponsed = 0;
uint8_t IsWIFIJoinResponsed = 0;
uint8_t IsWIFIApCtrlResponsed = 0;

uint8_t IsSNICIPConfigResponsed = 0;
uint8_t IsSNICGetDHCPInfoResponsed = 0;
uint8_t IsSNICTCPConnectToServerResponsed = 0;
uint8_t IsSNICSendFromSocketResponsed = 0;
uint8_t IsSNICUDPSendFromSocketResponsed = 0;
uint8_t IsCreateSocketResponsed = 0;

int32_t destIP = INADDR_NONE;
int32_t srcIP = INADDR_NONE;
int32_t destPort = PORT_NONE;
int32_t srcPort = PORT_NONE;

int32_t udpDestIP = INADDR_NONE;
int32_t udpSrcIP = INADDR_NONE;
int32_t udpDestPort = PORT_NONE;
int32_t udpSrcPort = PORT_NONE;

volatile uint8_t sendUDPDone = 0;
extern uint8_t seqNo;

UART_HandleTypeDef huart1;

/**
 * @} STM32F4_DISCOVERY_WIFI_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WIFI_Private_Functions
 * @{
 */

/**
 * @brief Data to be sent to Discover-WIFI
 * @param buf buffer
 * @param len buffer length
 */
static void SN8200_HAL_SendData(uint8_t *buf, int16_t len)
{
  int32_t i;

  for (i = 0; i < len; i++)
  {
    HAL_UART_Transmit(&huart1, &buf[i],1,10000);
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
  }
}


/**
 * @brief Check RX buffer
 * @return  value
 */
static uint8_t SN8200_HAL_RxBufferEmpty(void)
{
  return (RxBufferHead == RxBufferTail);
}

/**
 * @brief Read data from Received buffer
 * @return Received data
 */
static int32_t SN8200_HAL_ReadByte(void)
{
  uint8_t data = 0;

  if(RxBufferHead != RxBufferTail)
  {
    data = RxBuffer[RxBufferTail];
    RxBufferTail = (RxBufferTail + 1) % RX_BUFFER_SIZE;
  }

  return data;
}

/**
 * @brief initialize UART1 for Discover-WIFI
 * @param huart uart handle
 *        @arg huart1
 */
static void BSP_WIFI_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

/**
 * @brief Tx buffer
 * @param c data to be transmitted
 * @return data
 */
static uint8_t Dummy_TX(uint8_t c)
{
  tempReTXBuf[tempReTXLen++] = c;
  if (c == EOM_CHAR || tempReTXLen >= SEND_BUF_SIZE)
  {
    SN8200_HAL_SendData(tempReTXBuf, tempReTXLen);
    memset(tempReTXBuf, 0, tempReTXLen);
    tempReTXLen = 0;
  }
  return c;
}

/**
 * @brief transmit payload without using ESC
 * @param payload_len length of payload
 * @param payload data
 * @return value
 */
static uint8_t Escape_Payload_And_Transmit_NoESC(int16_t payload_len, uint8_t *payload)
{
  int32_t i;
  uint8_t chksum=0;
  uint8_t tx;
  for (i=0; i<payload_len; ++i)
  {
    tx = *payload++;
    if (tx != SOM_CHAR && tx != EOM_CHAR && tx != ESC_CHAR)
    {
      SERIAL_TX(tx);
      chksum += tx;
    }
    else
    {
      SERIAL_TX(ESC_CHAR);
      SERIAL_TX(0x80|tx);
      chksum += ESC_CHAR;
      chksum += (0x80|tx);
    }
  }
  return chksum;
}
/**
 * @brief length of data to be transmitted
 * @param payload_len
 * @param payload
 * @return length of data
 */
static int16_t Calc_Escaped_Payload_Len_NoESC(int16_t payload_len, uint8_t *payload)
{
  int32_t i;
  int32_t len=0;
  uint8_t c;

  for (i=0; i<payload_len; ++i)
  {
    c = *payload++;
    if (c == SOM_CHAR || c == EOM_CHAR || c == ESC_CHAR)
    {
      len += 2;
    }
    else
    {
      len ++;
    }
  }
  return len;
}

/**
 * @brief Transmit data frame
 * @param cmd_id SN8200 command id
 * @param payload_len  length of data
 * @param payload data
 * @param ack_required ack yes or no
 * @param len calculated length of payload
 * @return value
 */
static int8_t NoESC_Transmit(uint8_t cmd_id, int16_t payload_len, uint8_t *payload, uint8_t ack_required, int16_t len)
{
  uint8_t cksum=0;
  uint8_t hdr = 0x80;
  timeout = 10000;
  SERIAL_TX(SOM_CHAR); // Send SOM character
  SERIAL_TX(0x80|len); // Send payload len
  cksum += (0x80|len);
  hdr |= (ack_required<<6);
  if (len > 0x7f)
  {
      hdr |= (len>>7);
  }
  cksum += (0x80 | hdr);
  SERIAL_TX(0x80|hdr); // ACK, SEQ and frag number
  SERIAL_TX(0x80|cmd_id); // ACK, SEQ and frag number
  cksum += (0x80 | cmd_id);
  cksum += Escape_Payload_And_Transmit_NoESC(payload_len, payload);
  cksum |= 0x80;
  SERIAL_TX(cksum); // checksum
  SERIAL_TX(EOM_CHAR); // Send EOM character
  return 0;
}

/**
 * @brief Process the received data
 * @param rx_ch data
 * @return value
 */
static int32_t RX_Process_Char_NoESC(uint8_t rx_ch)
{
    int32_t ch=-1;
    switch(serial_rx_state)
    {
      case IDLE:
        if (rx_ch == SOM_CHAR)
        {
rx_process_char_no_ESC_RX_SOM:
          serial_rx_state = SOM_RECD;
          rx_frm_payload_index = 0;
          rx_frm_chksum = 0;
          rx_frm_data_index = 0;
        }
        else
            ch = rx_ch;
        break;

      case SOM_RECD:
        if( (rx_ch & 0x80) == 0 )
        {
          goto rx_process_char_no_ESC_err;
        }
        serial_rx_state=LEN_RECD;
        rx_frame[curr_buf_idx].payload_len = rx_ch & 0x7F;
        rx_frm_chksum += rx_ch;
        break;

      case LEN_RECD:
        if( (rx_ch & 0x80) == 0 )
        {
          goto rx_process_char_no_ESC_err;
        }
        serial_rx_state=ACK_SEQ_RECD;
        rx_frame[curr_buf_idx].payload_len |= (rx_ch & 0x3f)<<7;
        rx_frame[curr_buf_idx].ack_reqd = (rx_ch >> 6) & 0x01;
        rx_frm_chksum += rx_ch;
        break;

      case ACK_SEQ_RECD:
        if( (rx_ch & 0x80) == 0 )
        {
          goto rx_process_char_no_ESC_err;
        }
        serial_rx_state=CMD_RECD;
        rx_frame[curr_buf_idx].cmd_id = rx_ch & 0x7F;
        rx_frm_chksum += rx_ch;
        break;

      case CMD_RECD:
        serial_rx_state=PAYLAD_RX;

      case PAYLAD_RX:
        rx_frm_payload_index++;
        if (rx_ch == SOM_CHAR)
        {
          goto rx_process_char_no_ESC_err;
        }
        else if (rx_ch == EOM_CHAR)
        {
          serial_rx_state = IDLE;
        }
        else if (rx_ch == ESC_CHAR)
        {
          serial_rx_state = PAYLAD_RX_ESC;
          rx_frm_chksum += rx_ch;
        }
        else if (rx_frm_payload_index > rx_frame[curr_buf_idx].payload_len)
        {
          serial_rx_state = CHKSUM_RECD;
           rx_frame[curr_buf_idx].chksum = rx_ch;
        }
        else
        {
          rx_frm_chksum += rx_ch;
          rx_frame[curr_buf_idx].rx_payload[rx_frm_data_index++] = rx_ch;
        }
        break;

      case PAYLAD_RX_ESC:
        rx_frm_payload_index++;
        if (rx_ch > 127)
        {
          rx_frm_chksum += rx_ch;
          rx_frame[curr_buf_idx].rx_payload[rx_frm_data_index++] = rx_ch&0x7F;
          serial_rx_state = PAYLAD_RX;
        }
        else
        {
          goto rx_process_char_no_ESC_err;
        }
        break;

      case CHKSUM_RECD:
        if (rx_ch != EOM_CHAR);
        else if( (rx_frame[curr_buf_idx].chksum & 0x7F) == (rx_frm_chksum & 0x07F))
        { //checksum match
          rx_frame[curr_buf_idx].payload_len = rx_frm_data_index;
          rx_frame[curr_buf_idx].ackOk = 1;
          rx_frame[curr_buf_idx].available = 1;
          curr_buf_idx = (curr_buf_idx +1) % NUM_RX_BUF;
          serial_rx_state = IDLE;
        }
        else
        {
          //rx_bad_frame();
        }
        serial_rx_state = IDLE;
        break;

      default:
rx_process_char_no_ESC_err:
        serial_rx_state = IDLE;
        if( rx_ch == SOM_CHAR )
        {
          goto rx_process_char_no_ESC_RX_SOM;
        }
        break;
    }
    if (ch != -1)
    {
      return ch;
    }
    else
        return -1;
}
/**
 * @brief WIFI RX handle
 * @param buf data buffer
 * @param len length of data
 */
static void handleRxWiFi(UART_HandleTypeDef* huart,uint8_t* buf, int16_t len)
{
  uint8_t subCmdId = buf[0];
  switch (subCmdId)
  {
    case WIFI_GET_STATUS_RSP:
      IsWIFIGetStatusResponsed = 1;
      if (buf[2] == MODE_WIFI_OFF)
      {
        uprintf(huart,"WiFi Off.\n\r");
      }
      else
      {
        int8_t val[20] = {0};
        int32_t i=0;
        for(i=0; i<6; i++)
        {
          sprintf(val+3*i, "%02X:", buf[3+i]);
        }
        val[strlen(val)-1] = 0;
        uprintf(huart,"WiFi On.  Mac: %s.  ", val);
        if (buf[2] == MODE_NO_NETWORK)
        {
          uprintf(huart,"Not joined any network.\n\r");
        }
        else
        {
          uprintf(huart,"Joined SSID: %s\n\r", buf+9);
        }
      }
      break;

    case WIFI_JOIN_RSP:
      IsWIFIJoinResponsed = 1;
      if (WIFI_SUCCESS == buf[2])
      {
        uprintf(huart,"Join success\n\r");
      }
      else
      {
        uprintf(huart,"Join fail\n\r");
      }
      break;

    case WIFI_AP_CTRL_RSP:
      IsWIFIApCtrlResponsed = 1;
      if (WIFI_SUCCESS == buf[2])
      {
        if (APOnOff)
        {
          uprintf(huart,"AP is ON\n\r");
        }
        else
        {
          uprintf(huart,"AP is OFF\n\r");
        }
      }
      else
      {
        uprintf(huart,"AP control fail\n\r");
      }
      break;

    case WIFI_NETWORK_STATUS_IND:
      if (WIFI_NETWORK_UP == buf[3])
      {
        uprintf(huart,"Network UP\n\r");
        joinok = 1;
      }
      else
      {
        uprintf(huart,"Network Down\n\r");
      }
      break;

    case WIFI_SCAN_RESULT_IND:
    {
      int8_t cnt = buf[2];
      uint8_t i=3;
      uint8_t j;
      uint8_t ch, sec_tmp;
      uint8_t rssi;
      uint8_t len=32;
      if(cnt == 0)
      {
        for (j = 0; j < totalscan; j++)
        {
          uprintf(huart,"SSID: %s CH: %d02 RSSI: %d03 Sec: %d02 \n\r",sl[j].SSIDname, sl[j].ch,sl[j].rssi,sl[j].sectype);
        }
        memset(sl, 0, totalscan * sizeof(Scanlist_Typedef));
        totalscan = 0; //reset current scan result
      }
      else
      {
        while (cnt--)
        {
          ch = buf[i++];
          rssi = (uint8_t)buf[i++];
          sec_tmp = buf[i++];
          i += 6;
          i++;
          i += 2;
          len = (uint8_t)strlen((int8_t*)buf+i);
          if (len>32)
          {
            break;
          }
          strcpy((int8_t*)sl[totalscan].SSIDname,(int8_t*)buf+i);
          sl[totalscan].ch = ch;
          sl[totalscan].rssi = rssi;
          sl[totalscan].sectype = sec_tmp;
          if (len == 0)
          {
            while (buf[i] == 0) i++;
          }
          else
          {
            i += len+1;
          }
          totalscan++;
        }
      }
      break;
    }
    default:
      break;
  }
  uprintf(huart,".\n\r");
}

/**
 * @brief SNIC RX handle
 * @param buf data buffer
 * @param len length of data
 */
static void handleRxSNIC(UART_HandleTypeDef* huart,uint8_t* buf, int32_t len)
{
  uint8_t subCmdId = buf[0];
  static int32_t times = 0;
  static int32_t isPrintable = 0;

  switch (subCmdId)
  {
    case SNIC_CLOSE_SOCKET_RSP:
      if (SNIC_SUCCESS != buf[2])
      {
        uprintf(huart,"Close socket failed\n\r");
      }
      else
      {
        uprintf(huart,"Socket closed\n\r");
      }
      break;

    case SNIC_IP_CONFIG_RSP:
      IsSNICIPConfigResponsed = 1;
      ipok = 0;
      if (SNIC_SUCCESS == buf[2])
      {
        uprintf(huart,"IPConfig OK\n\r");
        ipok = 1;
      }
      else
      {
        uprintf(huart,"IPConfig fail\n\r");
      }
      break;

    case SNIC_GET_DHCP_INFO_RSP:
      IsSNICGetDHCPInfoResponsed = 1;
      if (SNIC_SUCCESS == buf[2])
      {
        uprintf(huart,"IP assigned as %d03.%d03.%d03.%d03 \n\r", buf[9],buf[10],buf[11],buf[12]);
          //save IP
        memcpy(&selfIP, buf+9, 4);
      }
      else
      {
        uprintf(huart,"IP not assigned\n\r");
      }
      break;

    case SNIC_TCP_CREATE_SOCKET_RSP:

    case SNIC_TCP_CREATE_ADV_TLS_SOCKET_RSP:

    case SNIC_TCP_CREATE_SIMPLE_TLS_SOCKET_RSP:

    case SNIC_UDP_CREATE_SOCKET_RSP:
      IsCreateSocketResponsed = 1;
      if (SNIC_SUCCESS == buf[2])
      {
        mysock = buf[3];
        uprintf(huart,"Socket:%d02 opened\n\r", mysock);
      }
      else
      {
        uprintf(huart,"Socket creation failed\n\r");
      }
      break;

    case SNIC_TCP_CONNECT_TO_SERVER_RSP:
      IsSNICTCPConnectToServerResponsed = 1;
      if (SNIC_CONNECT_TO_SERVER_PENDING == buf[2] || SNIC_SUCCESS == buf[2]);
      else
      {
        uprintf(huart,"Unable to connect server\n\r");
      }
      break;

    case SNIC_TCP_CREATE_CONNECTION_RSP:
      if (SNIC_SUCCESS != buf[2])
      {
        uprintf(huart,"Unable to create TCP server\n\r");
      }
      break;

    case SNIC_TCP_CONNECTION_STATUS_IND:
      if (SNIC_CONNECTION_UP == buf[2])
      {
        uprintf(huart,"Socket connection UP\n\r");
        sockConnected = buf[3];
      }
      else if (SNIC_CONNECTION_CLOSED == buf[2])
      {
        uprintf(huart,"Socket %d02 closed\n\r", buf[3]);
        sockClosed = buf[3];
      }
      break;

    case SNIC_SEND_RSP:
    {
      uint32_t sentsize;
      IsSNICSendFromSocketResponsed = 1;
      if (SNIC_SUCCESS == buf[2])
      {
        pktcnt ++;
        sentsize = ((uint32_t)(buf[3] << 8) | (uint32_t)buf[4]);
        uprintf(huart,"pkt %d03, %d03 bytes sent \n\r", pktcnt, sentsize);
      }
      break;
    }
    case SNIC_CONNECTION_RECV_IND:
    {
      int32_t i=0;
      uint32_t sentsize = ((uint32_t)(buf[3] << 8) | (uint32_t)buf[4]);
      uint32_t sock = (uint32_t)buf[2];
      uprintf(huart,"%d03 bytes received from socket %d02 \n\r", sentsize, sock);
      HAL_Delay(10);
      if (strncmp((int8_t*)buf+5, "GET /", 5) == 0 || strncmp((int8_t*)buf+5, "POST /", 6) == 0)  // Receives a HTTP(S) get/post request.
      {
        static int32_t i=0;
        for (i=0; i<sentsize; i++)
        {
          uprintf(huart,"%c", buf[5+i]);
        }
        i = 0;
        sprintf(out, HTTP_RSP_STR, i++);
        // Send back something
        SendFromSock(huart,sock, (uint8_t*)out, strlen(out), 2, seqNo++);

        // If it is a TLS server, it only accepts one connection, so
        // it is better for the host app to close it after some idle time for new connections.
        //  Sleep(100);
        //  closeSocket((int8u)sock,seqNo++);
      }
      else if (strncmp((int8_t*)buf+5, "HTTP/1.", 7) == 0)  // Receives HTTP response, close socket. If the socket is not closed,
      {
        for (i=0; i<sentsize; i++)
        {
          uprintf(huart,"%c", buf[5+i]);
        }                                              // it can be used for more data communication (using send from socket).
        CloseSocket(huart,(uint8_t)sock,seqNo++); }
      break;
    }
    case SNIC_TCP_CLIENT_SOCKET_IND:
    {
       uint8_t listen_sock = buf[2];
       uprintf(huart,"Accepted connection from %d03.%d03.%d03.%d03\n\r", buf[4], buf[5], buf[6], buf[7]);
       uprintf(huart,"Connection socket: %d02\n\r", buf[3]);
       break;
    }
    case SNIC_UDP_RECV_IND:
       uprintf(huart,"%d02 %d04\n\r", times++, htons(*((uint16_t *)&buf[9])));
       break;

    case SNIC_UDP_SEND_FROM_SOCKET_RSP:
       IsSNICUDPSendFromSocketResponsed = 1;
       if (SNIC_SUCCESS != buf[2])
       {
         uprintf(huart,"UDP Send bad\n\r");
       }
       sendUDPDone = 1;
       break;
    case SNIC_HTTP_REQ|0x80:

    case SNIC_HTTPS_REQ|0x80:

    case SNIC_HTTP_MORE_REQ|0x80:
    {
      int8_t *contentT = "";
      uint16_t len = *((int32_t*)&buf[4]);
      int32_t contTLen = 0;
      uint32_t moreData = len & 0x8000;
      int8_t more[10] = {0};
      uint8_t seq = buf[1];
      int32_t status = *((int32_t*)&buf[2]);
      status = swap16(status);
      len = swap16(len);
      if (subCmdId == (SNIC_HTTP_MORE_REQ|0x80))
      {
        strcpy(more, "more ");
      }
      len &= 0x7fff;
      if (status >= 100)
      {
        contentT = (int8_t*)&buf[6];;
        contTLen = strlen(contentT)+1;
      }
      if (status < 100)
      {
        uprintf(huart,"\nHTTP %sRSP code: %d02, seq#: %d02\n\r", more, status, seq);
        break;
      }
      uprintf(huart,"\nHTTP %sRSP code: %d06 , seq#: %d02 , Content Length: %d03 , Type: %s , More data: %s\n\r", more,
          status, seq, len, contentT, moreData?"yes":"no");
      isPrintable = 0;
      if (contTLen && (strstr(contentT, "text") || strstr(contentT, "xml") ||
          strstr(contentT, "javascript") || strstr(contentT, "html") ||
          strstr(contentT, "json")))
      {
        isPrintable = 1;
        buf[6+contTLen+len] = 0;
        uprintf(huart,"Content: \n%s\n\r", buf+6+contTLen);
      }
      break;
    }
    case SNIC_HTTP_RSP_IND:
    {
      uint8_t seq = buf[1];
      uint32_t moreData;
      len = *((int32_t*)&buf[2]);
      len = swap16(len);
      moreData = len & 0x8000;
      len &= 0x7fff;
      uprintf(huart,"\nHTTP RSP indication, seq#: %d06, content length: %d03, More data: %s\n\r", seq, len, moreData?"yes":"no");
      if (isPrintable)
      {
        buf[4+len] = 0;
        uprintf(huart,"Content: \n%s\n\r", buf+4);
      }
      break;
    }
    default:
      break;
  }
  uprintf(huart,".\n\r");
}


/**
 * @brief Serial command protocol
 * @param commandId select the mode
 * @param paramLength data length
 * @param params data
 * @return value
 */
static uint8_t Sci_Ser_Cmd_Proc(int16_t commandId, int16_t paramLength, uint8_t *params)
{
    switch (commandId)
    {
      case CMD_ID_WIFI:
        handleRxWiFi(&huart4,params,paramLength);
        break;

      case CMD_ID_SNIC:
        handleRxSNIC(&huart4,params,paramLength);
        break;

      default:
        break;
    }
    return 0;
}
/**
 * @brief check receive frame
 * @return value
 */
static uint8_t SN8200_RxFrameEmpty(void)
{
    return (curr_read_idx == curr_buf_idx);
}

/**
 * @brief process received frame
 * @param idx provide the command id for received frame
 * @return
 */
static int8_t Process_RX_Frame(int32_t idx)
{
    if (rx_frame[idx].cmd_id == 0x7F)
    {
      //serial_ack_nak_recd(1);
    }
    else if (rx_frame[idx].cmd_id == 0x00)
    {
      //serial_ack_nak_recd(0);
    }
    else
    {
      if (rx_frame[idx].ack_reqd)
      {
        //send_ack_nak(rx_frame[idx].ackOk);
      }
      if (rx_frame[idx].ackOk)
      {
        Sci_Ser_Cmd_Proc(rx_frame[idx].cmd_id, rx_frame[idx].payload_len, rx_frame[idx].rx_payload);
      }
    }
    return 0;
}

/**
 * @brief serial data transmit using UART
 * @param cmd_id provide command id
 * @param payload data
 * @param payload_len data length
 * @param ack_required acknowledgment
 * @return value
 */
static int32_t Serial_Transmit(uint8_t cmd_id, uint8_t *payload, int16_t payload_len, uint8_t ack_required)
{
    int16_t len;

    if(payload_len > MAX_PAYLOAD_LEN)
        return 0;

    len = Calc_Escaped_Payload_Len_NoESC(payload_len, payload);
    return NoESC_Transmit(cmd_id, payload_len, payload, ack_required, len);
}

/**
 * @brief RX thread process it will first put received data from buffer to RX frame then process the RX frame
 */
static void RX_Thread_Proc(void)
{
    uint8_t data;

    while(!SN8200_HAL_RxBufferEmpty())
    {
      data = SN8200_HAL_ReadByte();
      RX_Process_Char_NoESC(data);
    }

    while(!SN8200_RxFrameEmpty())
    {
      if (rx_frame[curr_read_idx].available)
      {
        Process_RX_Frame(curr_read_idx);
        rx_frame[curr_read_idx].available = 0;
      }
      curr_read_idx = (curr_read_idx+1) % NUM_RX_BUF;
    }
}


/**
 * @brief Check whether "cp" is a valid ascii representation of an
 *        Internet address and convert to a binary address
 * @param cp IP address in ascii represenation (e.g. "127.0.0.1")
 * @param addr pointer to which to save the ip address in network order
 * @return 1 if cp could be converted to addr, 0 on failure
 */
static int8_t inet_aton(const int8_t *cp, struct in_addr *addr)
{
  uint32_t val;
  uint8_t base;
  int8_t c;
  int32_t parts[4];
  int32_t *pp = parts;

  c = *cp;
  for (;;)
  {
    /*
     * Collect number up to ``.''.
     * Values are specified as for C:
     * 0x=hex, 0=octal, 1-9=decimal.
     */
    if (!isdigit(c))
    {
      return (0);
    }
    val = 0;
    base = 10;
    if (c == '0')
    {
      c = *++cp;
      if (c == 'x' || c == 'X')
      {
        base = 16;
        c = *++cp;
      }
      else
      {
        base = 8;
      }
    }
    for (;;)
    {
      if (isdigit(c))
      {
        val = (val * base) + (int32_t)(c - '0');
        c = *++cp;
      }
      else if (base == 16 && isxdigit(c))
      {
        val = (val << 4) | (int32_t)(c + 10 - (islower(c) ? 'a' : 'A'));
        c = *++cp;
      }
      else
      {
        break;
      }
    }
    if (c == '.')
    {
      /*
       * Internet format:
       *  a.b.c.d
       *  a.b.c   (with c treated as 16 bits)
       *  a.b (with b treated as 24 bits)
       */
      if (pp >= parts + 3)
      {
        return (0);
      }
      *pp++ = val;
      c = *++cp;
    }
    else
    {
      break;
    }
  }

  /*
   * Check for trailing characters.
   */
  if (c != '\0' && !isspace(c))
  {
    return (0);
  }
  /*
   * Concoct the address according to
   * the number of parts specified.
   */
  switch (pp - parts + 1)
  {
    case 0:
      return (0);       /* initial nondigit */

    case 1:             /* a -- 32 bits */
      break;

    case 2:             /* a.b -- 8.24 bits */
      if (val > 0xffffffUL)
      {
        return (0);
      }
      val |= parts[0] << 24;
      break;

    case 3:             /* a.b.c -- 8.8.16 bits */
      if (val > 0xffff)
      {
        return (0);
      }
      val |= (parts[0] << 24) | (parts[1] << 16);
      break;

    case 4:             /* a.b.c.d -- 8.8.8.8 bits */
      if (val > 0xff)
      {
        return (0);
      }
      val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
      break;
  }
  if (addr)
  {
    addr->s_addr = htonl(val);
  }
  return (1);
}

/**
 * @brief Internet address
 * @param cp Valid ascii representation of an Internet address
 * @return value
 */
static uint32_t inet_addr(const int8_t *cp)
{
  struct in_addr val;

  if (inet_aton(cp, &val))
  {
    return (val.s_addr);
  }
  return (INADDR_NONE);
}

/**
 * @} STM32F4_DISCOVERY_WIFI_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_WIFI_Public_Functions
 * @{
 */

/**
 * @brief initialize UART interface for Discover-WIFI
 * @param baudrate provide Baudrate value communicate (115200)
 */
void SN8200_HAL_Init(uint32_t baudrate)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = baudrate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  BSP_WIFI_MspInit(&huart1);
  HAL_UART_Init(&huart1);

  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
}

/**
 * @brief This is UART1 receive function
 * @param huart uart handle
 *        @arg huart1
 * @return Received data
 */
uint16_t UART_ReceiveData(UART_HandleTypeDef* huart)
{
  /* Receive Data */
  return (uint16_t)(huart1.Instance->DR & (uint16_t)0x01FF);
}

/**
 * @brief Get TCP info
 * @return value
 */
int8_t GetTCPInfo(UART_HandleTypeDef* huart)
{
  int8_t tempIPstr[32];
  int8_t teststr[8];

  if (strlen(IPstr)==0)
  {
    strcpy(IPstr, "192.168.10.101");
  }
  if (strlen(Portstr)==0)
  {
    strcpy(Portstr, "0x6990");
  }
  uprintf(huart,"Enter server IP to connect: \n\r");
  uget_line(huart,tempIPstr,sizeof(tempIPstr));
//    scanf("%s", tempIPstr);
  uprintf(huart,"\n\r");
  if (strlen(tempIPstr))
  {
    strcpy(IPstr, tempIPstr);
  }
  destIP = inet_addr(IPstr);
  if (destIP == INADDR_NONE || destIP == INADDR_ANY)
  {
    return CMD_ERROR;
  }
  uprintf(huart,"Enter server port number: \n\r");
  uget_line(huart,teststr,sizeof(teststr));
//    scanf("%s", teststr);
  uprintf(huart,"\n\r");
  if (strlen(teststr))
  {
    strcpy(Portstr, teststr);
  }
  destPort = strtol(Portstr, NULL, 0);
  destPort = swap16(destPort);
  if (destPort > 0xFFFF)
  {
    uprintf(huart,"Invalid port, max limit 0xFFFF \n\r");
    return CMD_ERROR;
  }
  return 0;
}

/**
 * @brief Set TCP info
 * @return value
 */
int8_t SetTCPInfo(UART_HandleTypeDef* huart)
{
  int8_t teststr[8];
  if (selfIP == 0)
  {
    uprintf(huart,"IP address has not been obtained.\n\r");
    return CMD_ERROR;
  }
  srcIP = selfIP;

  uprintf(huart,"Enter server port number to set: \n\r");
  uget_line(huart,teststr,sizeof(teststr));
//  scanf("%s", teststr);
  uprintf(huart,"\n\r");
  srcPort = strtol(teststr, NULL, 0);
  srcPort = swap16(srcPort);
  if (srcPort > 0xFFFF)
  {
    uprintf(huart,"Invalid port, max limit 0xFFFF \n\r");
    return CMD_ERROR;
  }
  return 0;
}

/**
 * @brief Get UDP info
 * @return value
 */
int8_t GetUDPInfo(UART_HandleTypeDef* huart)
{
  int8_t tempIPstr[32];
  int8_t teststr[8];
  uprintf(huart,"Enter server IP to connect: \n\r");
  uget_line(huart,tempIPstr,sizeof(tempIPstr));
//  scanf("%s", tempIPstr);
  uprintf(huart,"\n\r");
  udpDestIP = inet_addr(tempIPstr);
  if (udpDestIP == INADDR_NONE)
  {
    return CMD_ERROR;
  }
  uprintf(huart,"Enter server port number: \n\r");
  uget_line(huart,teststr,sizeof(teststr));
//  scanf("%s", teststr);
  uprintf(huart,"\n\r");
  udpDestPort = strtol(teststr, NULL, 0);
  udpDestPort = swap16(udpDestPort);
  if (udpDestPort > 0xFFFF)
  {
    uprintf(huart,"Invalid port, max limit 0xFFFF \n\r");
    return CMD_ERROR;
  }
  return 0;
}

/**
 * @brief Set UDP info
 * @return value
 */
int8_t SetUDPInfo(UART_HandleTypeDef* huart)
{
  int8_t teststr[8];
  if (selfIP == 0)
  {
    uprintf(huart,"IP address has not been obtained.\n\r");
    return CMD_ERROR;
  }
  udpSrcIP = selfIP;
  uprintf(huart,"Enter server port number to set: \n\r");
  uget_line(huart,teststr,sizeof(teststr));
//  scanf("%s", teststr);
  uprintf(huart,"\n\r");
  udpSrcPort = strtol(teststr, NULL, 0);
  udpSrcPort = swap16(udpSrcPort);
  if (udpSrcPort > 0xFFFF)
  {
    uprintf(huart,"Invalid port, max limit 0xFFFF \n\r");
    return CMD_ERROR;
  }
  return 0;
}

/**
 * @brief check receive buffer
 * @return value
 */
int8_t SN8200_API_HasInput(void)
{
    return !SN8200_HAL_RxBufferEmpty();
}

/**
 * @brief Status of Discover-WIFI
 * @param seq sequence number
 */
void GetStatus(UART_HandleTypeDef* huart,uint8_t seq)
{
  timeout = 10000;
  uint8_t buf[4];
  buf[0] = WIFI_GET_STATUS_REQ;
  buf[1] = seq;
  buf[2] = 0;
  Serial_Transmit(CMD_ID_WIFI, buf, 3, ACK_NOT_REQUIRED);
  uprintf(huart,"-GetStatus\n\r");
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsWIFIGetStatusResponsed)
    {
      IsWIFIGetStatusResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
}

/**
 * @brief Turn WIFI on
 * @param seq sequence number
 */
void WifiOn(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t flag = 1;
  uint8_t buf[4];
  buf[0] = WIFI_ON_REQ;
  buf[1] = seq;
  buf[2] = (int8_t)'U';
  buf[3] = (int8_t)'S';
  Serial_Transmit(CMD_ID_WIFI, buf, 4, ACK_NOT_REQUIRED);
  uprintf(huart,"-WifiOn\n\r");
  HAL_Delay(1);
}

/**
 * @brief Turn WIFI off
 * @param seq sequence number
 */
void WifiOff(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[2];
  buf[0] = WIFI_OFF_REQ;
  buf[1] = seq;
  Serial_Transmit(CMD_ID_WIFI, buf, 2, ACK_NOT_REQUIRED);
  uprintf(huart,"-WifiOff\n\r");
}

/**
 * @brief turn Access point on
 * @param seq sequence number
 */
void ApOnOff(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[4];

  APOnOff ^= 1;
  buf[0] = WIFI_AP_CTRL_REQ;
  buf[1] = seq;
  buf[2] = APOnOff;
  buf[3] = 0; //persistency hardcode set as 0 means NOT save to NVM
  Serial_Transmit(CMD_ID_WIFI, buf, 4, ACK_NOT_REQUIRED);
  uprintf(huart,"-AP status\n\r");
  timeout = 10000;
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsWIFIApCtrlResponsed)
    {
      IsWIFIApCtrlResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
}

/**
 * @brief check for other network
 * @param seq sequence number
 */
void WifiScan(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[12];
  buf[0] = WIFI_SCAN_REQ;
  buf[1] = seq;
  memset(&buf[2], 0, 10);
  buf[3] = 2; // bss type = any
  Serial_Transmit(CMD_ID_WIFI, buf, 12, ACK_NOT_REQUIRED);
  uprintf(huart,"-WifiScan\n\r");
}

/**
 * @brief Join the WIFI network
 * @param seq sequence number
 */
void WifiJoin(UART_HandleTypeDef* huart,uint8_t seq)
{

  uint8_t buf[128];
  int8_t tempstr[2] = {0};
  uint8_t *p = buf;

  *p++ = WIFI_JOIN_REQ;
  *p++ = seq;

  uprintf(huart,"Enter SSID: \n\r");
  uget_line(huart,SSID,sizeof(SSID));
//  uint8_t data,i;
//  i = 0;
//  while(1)
//  {
//    if(HAL_UART_Receive(&huart6,&data,1,50000) == HAL_OK)
//    {
//      if(data == In_CR)
//      {
//        break;
//      }
//      SSID[i] = data;
//      i++;
//      HAL_UART_Transmit(&huart6,&data,1,1000);
//    }
//  }

//  scanf("%s", SSID);
  uprintf(huart,"\n\r");
  while(!strlen(SSID))
  {
    uprintf(huart,"SSID can't be empty. Enter SSID: \n\r");
    uget_line(huart,SSID,sizeof(SSID));
//      scanf("%s", SSID);
    uprintf(huart,"\n\r");
  }
  memcpy(p, SSID, strlen(SSID));
  p += strlen(SSID);
  *p++ = 0x00;
  uprintf(huart,"Enter Security Mode (e.g., 0 for open, 2 for WPA TKIP, 4 for WPA2 AES, 6 for WPA2 MIXED):\n\r");
  uget_line(huart,tempstr,sizeof(tempstr));
//  scanf("%s", tempstr);
  uprintf(huart,"\n\r");
  secMode = atoi(tempstr);
  if (secMode)
  {
    uprintf(huart,"Enter Security Key:\n\r");
    uget_line(huart,secKey,sizeof(secKey));
//    scanf("%s", secKey);
    uprintf(huart,"\n\r");
    Keylen = (uint8_t)strlen(secKey);
    if (Keylen <= 0)
    {
      uprintf(huart,"Invalid Key\n\r");
      return;
    }
  }
  *p++ = secMode;
  *p++ = Keylen;
  if (Keylen)
  {
    memcpy(p, secKey, Keylen);
    p += Keylen;
  }
  Serial_Transmit(CMD_ID_WIFI, buf, (int32_t)(p - buf), ACK_NOT_REQUIRED);
  uprintf(huart,"-WifiJoin\n\r");
  timeout = 10000;
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsWIFIJoinResponsed)
    {
      IsWIFIJoinResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
  joinok = 0;
}

/**
 * @brief Disconnect the WIFI network
 * @param seq sequence number
 */
void WifiDisconn(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[2];
  buf[0] = WIFI_DISCONNECT_REQ;
  buf[1] = seq;
  Serial_Transmit(CMD_ID_WIFI, buf, 2, ACK_NOT_REQUIRED);
  uprintf(huart,"-WifiDisconn\n\r");
}

/**
 * @brief Initialize SNIC mode
 * @param seq sequence number
 */
void SnicInit(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[4];
  int32_t tmp;
  tmp = 0x00;     //The Default receive buffer size
  buf[0] = SNIC_INIT_REQ;
  buf[1] = seq;
  memcpy(buf+2, (uint8_t*)&tmp, 2);
  Serial_Transmit(CMD_ID_SNIC, buf, 4, ACK_NOT_REQUIRED);
  uprintf(huart,"-SnicInit\n\r");
}

/**
 * @brief SNIC cleanup function
 * @param seq sequence number
 */
void SnicCleanup(UART_HandleTypeDef* huart,uint8_t seq)
{
    uint8_t buf[2];
    buf[0] = SNIC_CLEANUP_REQ;
    buf[1] = seq;
    Serial_Transmit(CMD_ID_SNIC, buf, 2, ACK_NOT_REQUIRED);
    uprintf(huart,"-SnicCleanup\n\r");
}

/**
 * @brief SNIC IP configuration function
 * @param seq sequence number for command
 */
void SnicIPConfig(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[16];
  buf[0] = SNIC_IP_CONFIG_REQ;
  buf[1] = seq;
  buf[2] = 0; //STA
  buf[3] = 1; //DHCP

  Serial_Transmit(CMD_ID_SNIC, buf, 4, ACK_NOT_REQUIRED);
  uprintf(huart,"-SnicIPConfig\n\r");
  timeout = 10000;
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsSNICIPConfigResponsed)
    {
      IsSNICIPConfigResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
}

/**
 * @brief Get the DHCP value
 * @param seq sequence number
 */
void SnicGetDhcp(UART_HandleTypeDef* huart,uint8_t seq)
{
  uint8_t buf[3];
  int8_t tempstr[2] = {0};

  buf[0] = SNIC_GET_DHCP_INFO_REQ;
  buf[1] = seq;
  uprintf(huart,"\n\rInterface Type? (0: STA  1: AP) \n\r");
  uget_line(huart,tempstr,sizeof(tempstr));
//  scanf("%s", tempstr);
  uprintf(huart,"\n\r");
  buf[2] = atoi(tempstr);
  //buf[2] = 0; // STA  1; // AP
  Serial_Transmit(CMD_ID_SNIC, buf, 3, ACK_NOT_REQUIRED);
  uprintf(huart,"-SnicGetDhcp\n\r");
  timeout = 10000;
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsSNICGetDHCPInfoResponsed)
    {
      IsSNICGetDHCPInfoResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
}

/**
 * @brief Creat socket for TCP
 * @param bindOption to present Local IP address and Local port
 * @param localIp IP address
 * @param port local port
 * @param seq sequence number
 * @param ssl SNIC command
 * @return value
 */
uint8_t TCPCreateSocket(UART_HandleTypeDef* huart,uint8_t bindOption, uint32_t localIp, uint16_t port, uint8_t seq, uint8_t ssl)
{
  uint8_t buf[9];
  buf[0] = ssl;
  buf[1] = seq;
  buf[2] = bindOption;

  if (bindOption)
  {
    memcpy(buf+3, (uint8_t*)&localIp, 4);
    memcpy(buf+7, (uint8_t*)&port, 2);
    Serial_Transmit(CMD_ID_SNIC, buf, 9, ACK_NOT_REQUIRED);
  }
  else
  {
    Serial_Transmit(CMD_ID_SNIC, buf, 3, ACK_NOT_REQUIRED);
  }
  uprintf(huart,"-tcpCreateSocket\n\r");
  timeout = 10000;
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsCreateSocketResponsed)
    {
      IsCreateSocketResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
  return 0;
}

/**
 * @brief close a socket
 * @param shortSocket socket number in both TCP and UDP
 * @param seq sequence number
 * @return value
 */
uint8_t CloseSocket(UART_HandleTypeDef* huart,uint8_t shortSocket, uint8_t seq)
{
  uint8_t buf[3];
  buf[0] = SNIC_CLOSE_SOCKET_REQ;
  buf[1] = seq;
  buf[2] = shortSocket;
  Serial_Transmit(CMD_ID_SNIC, buf, 3, ACK_NOT_REQUIRED);
  uprintf(huart,"-closeSocket\n\r");
  uprintf(huart,"Socket %d02 closed\n\r", shortSocket);
  return 0;
}

/**
 * @brief connect to a remote TCP server
 * @param shortSock socket number in both TCP
 * @param ip  server IP address
 * @param port server port
 * @param bufsize maximum packet size the application wants to receive per transmission
 * @param timeout Time for connect attempt
 * @param seq sequence number
 * @return value
 */
uint8_t TCPConnectToServer(UART_HandleTypeDef* huart,uint8_t shortSock, uint32_t ip, uint16_t port, uint16_t bufsize, uint8_t timeout, uint8_t seq)
{
  uint8_t buf[12];
  if (bufsize == 0 || bufsize > MAX_BUFFER_SIZE)
  {
    bufsize = MAX_BUFFER_SIZE;
  }
  buf[0] = SNIC_TCP_CONNECT_TO_SERVER_REQ;
  buf[1] = seq;
  buf[2] = shortSock;

  memcpy(buf+3, (uint8_t*)&ip, 4);
  memcpy(buf+7, (uint8_t*)&port, 2);
  bufsize = swap16(bufsize);
  memcpy(buf+9, (uint8_t*)&bufsize, 2);
  buf[11] = timeout;
  Serial_Transmit(CMD_ID_SNIC, buf, 12, ACK_NOT_REQUIRED);
  uprintf(huart,"-tcpConnectToServer\n\r");
  HAL_Delay(1000);        //Wait module return value
  while (1)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsSNICTCPConnectToServerResponsed)
    {
      IsSNICTCPConnectToServerResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
  return 0;
}

/**
 * @brief Create connection to a remote TCP server
 * @param shortSock socket number in both TCP
 * @param size maximum packet size the application wants to receive per transmission
 * @param maxClient maximum number of client connections this connection server can serve simultaneously
 * @param seq sequence number
 * @return value
 */
uint8_t TCPCreateConnection(UART_HandleTypeDef* huart,uint8_t shortSock, uint16_t size, uint8_t maxClient, uint8_t seq)
{
  uint8_t buf[6];
  if (size == 0 || size > MAX_BUFFER_SIZE)
  {
    size = MAX_BUFFER_SIZE;
  }
  if (maxClient == 0 || maxClient > MAX_CONNECTION_PER_SOCK)
  {
    maxClient = MAX_CONNECTION_PER_SOCK;
  }
  buf[0] = SNIC_TCP_CREATE_CONNECTION_REQ;
  buf[1] = seq;
  buf[2] = shortSock;
  size = swap16(size);
  memcpy(buf+3, (uint8_t*)&size, 2);
  buf[5] = maxClient;
  Serial_Transmit(CMD_ID_SNIC, buf, 6, ACK_NOT_REQUIRED);
  uprintf(huart,"-tcpCreateConnection\n\r");
  return 0;
}

/**
 * @brief Send a packet to the remote peer via a connected socket
 * @param shortSocket socket number
 * @param sendBuf data to be sent
 * @param len data length
 * @param timeout time for send data attempt
 * @param seq sequence number
 * @return value
 */
uint8_t SendFromSock(UART_HandleTypeDef* huart,uint8_t shortSocket, uint8_t * sendBuf, uint16_t len, uint8_t timeout, uint8_t seq)
{
  uint8_t buf[MAX_BUFFER_SIZE+6];
  uint16_t mybufsize;
  if (len == 0 || len > MAX_BUFFER_SIZE)
  {
    len = MAX_BUFFER_SIZE;
  }
  buf[0] = SNIC_SEND_FROM_SOCKET_REQ;
  buf[1] = seq;
  buf[2] = shortSocket;
  buf[3] = 0;
  mybufsize = swap16(len);
  memcpy(buf+4, (uint8_t*)&mybufsize, 2);
  memcpy(buf+6, sendBuf, len);
  Serial_Transmit(CMD_ID_SNIC, buf, 6+len, ACK_NOT_REQUIRED);
  uprintf(huart,"-sendFromSock\n\r");
  while (1)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsSNICSendFromSocketResponsed)
    {
      IsSNICSendFromSocketResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
  return 0;
}


/**
 * @brief Creat socket for UDP
 * @param bindOption to present Local IP address and Local port
 * @param ip IP address
 * @param port local port
 * @param seq sequence number
 * @return value
 */
uint8_t UDPCreateSocket(UART_HandleTypeDef* huart,uint8_t bindOption, uint32_t ip, uint16_t port, uint8_t seq)
{
  uint8_t buf[9];
  buf[0] = SNIC_UDP_CREATE_SOCKET_REQ;
  buf[1] = seq;
  buf[2] = bindOption;
  if (bindOption)
  {
    uint32_t myip = swap32(ip);
    uint16_t myport = swap16(port);
    memcpy(buf+3, (uint8_t*)&myip, 4);
    memcpy(buf+7, (uint8_t*)&myport, 2);
    Serial_Transmit(CMD_ID_SNIC, buf, 9, ACK_NOT_REQUIRED);
  }
  else
  {
    Serial_Transmit(CMD_ID_SNIC, buf, 3, ACK_NOT_REQUIRED);
  }
  uprintf(huart,"-udpCreateSocket\n\r");
  while (1)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(IsCreateSocketResponsed)
    {
      IsCreateSocketResponsed = 0;
      break;
    }
    HAL_Delay(1);
  }
  return 0;
}

/**
 * @brief Send a packet to the remote peer
 * @param ip Remote IP address
 * @param iPort Remote port
 * @param shortsock Socket number
 * @param conMode Connection mode
 * @param sendbuf data to be sent
 * @param len data length
 * @param seq sequence number
 * @return value
 */
uint8_t UDPSendFromSock(UART_HandleTypeDef* huart,uint32_t ip, uint16_t iPort, uint8_t shortsock, uint8_t conMode, uint8_t *sendbuf, uint16_t len, uint8_t seq)
{
  uint8_t buf[2048+12];
  uint16_t mybufsize;
  if (len == 0 || len > MAX_BUFFER_SIZE)
  {
    len = MAX_BUFFER_SIZE;
  }
  buf[0] = SNIC_UDP_SEND_FROM_SOCKET_REQ;
  buf[1] = seq;
  memcpy(buf+2, (uint8_t*)&ip, 4);
  memcpy(buf+6, (uint8_t*)&iPort, 2);
  buf[8] = shortsock;
  buf[9] = conMode;
  mybufsize = swap16(len);
  memcpy(buf+10, (uint8_t*)&mybufsize, 2);
  memcpy(buf+12, sendbuf, len);
  Serial_Transmit(CMD_ID_SNIC, buf, 12+len, ACK_NOT_REQUIRED);
  uprintf(huart,"-udpSendFromSock\n\r");
  timeout = 10000;
  while (timeout--)
  {
    if(SN8200_API_HasInput())
    {
      ProcessSN8200Input();
    }
    if(sendUDPDone)
    {
      sendUDPDone = 0;
      break;
    }
    HAL_Delay(1);
  }
  return 0;
}

/**
 * @brief Start receiving incoming data packets
 * @param sock Socket
 * @param bufsize data size
 * @param seq sequence number
 * @return
 */
uint8_t UDPStartRecv(UART_HandleTypeDef* huart,uint32_t sock, uint16_t bufsize, uint8_t seq)
{
  uint8_t buf[5];
  int32_t tmp;
  if (bufsize == 0 || bufsize > MAX_BUFFER_SIZE)
  {
    bufsize = MAX_BUFFER_SIZE;
  }
  tmp = swap16(bufsize);
  buf[0] = SNIC_UDP_START_RECV_REQ;
  buf[1] = seq;
  buf[2] = sock;
  memcpy(buf+3, (uint8_t*)&tmp, 2);
  Serial_Transmit(CMD_ID_SNIC, buf, 5, ACK_NOT_REQUIRED);
  uprintf(huart,"-udpStartRecv\n\r");
  return 0;
}

/**
 * @brief process the received data
 */
void ProcessSN8200Input(void)
{
  RX_Thread_Proc();
}

/**
 * @brief Send a packet to the remote peer
 * @param buf data
 * @param size data size
 */
void SendSNIC(uint8_t *buf, int32_t size)
{
    const uint32_t IP = udpDestIP;
    uint16_t port = udpDestPort;
    sendUDPDone = 0;
    UDPSendFromSock(&huart4,IP, port, mysock, 0, buf, size, seqNo++);
}

/**
 * @brief Send HTTP request
 * @param seq sequence number
 * @param domain domain name
 * @param uri to identify name of resource
 * @param method
 * @param contentType type of data
 * @param otherHeader
 * @param contentLen data length
 * @param content data
 * @param timeout time to send HTTP request
 * @param moreData
 * @param isHttps is https? 1 or 0
 * @return value
 */
uint8_t FillNSendHttpReq(uint8_t seq, int8_t* domain, int8_t* uri, int8_t method, int8_t* contentType, int8_t* otherHeader,
    int32_t contentLen, int8_t* content, uint8_t timeout, int8_t moreData, int8_t isHttps)
{
  int8_t *ptr = NULL;
  uint8_t buf[1024];
  uint16_t encodedLen = moreData?contentLen|0x8000:contentLen;
  memset(buf, 0, sizeof(buf));
  buf[0] = SNIC_HTTP_REQ;
  buf[1] = seq;
  *((uint16_t *)&buf[2]) = 0x5000; //swapped
  buf[4] = method;
  buf[5] = timeout;
  if (isHttps)
  {
    buf[0] = SNIC_HTTPS_REQ;
    *((uint16_t*)&buf[2]) = 0xbb01; // 443 swapped
  }
  ptr = (int8_t *)buf+6;
  ptr += sprintf(ptr, "%s", domain)+1;
  ptr += sprintf(ptr, "%s", uri)+1;
  ptr += sprintf(ptr, "%s", contentType)+1;
  ptr += sprintf(ptr, "%s", otherHeader)+1;
  *((uint16_t*)ptr) = swap16(encodedLen);
  ptr += 2;
  if (contentLen)
  {
    memcpy(ptr, content, contentLen);
  }
  Serial_Transmit(CMD_ID_SNIC, buf, ptr - (int8_t *)buf+contentLen, ACK_NOT_REQUIRED);
  return 0;
}

/**
 * @brief If moredata in the FillNSendHttpReq function is 1 then this function is
 *        used to send the request with the FillNSendHttpReq function
 * @param seq sequence number
 * @param contentLen data length
 * @param content data
 * @param moreData
 * @return value
 */
int8_t FillNSendHttpMoreReq(uint8_t seq, int32_t contentLen, int8_t* content, int8_t moreData)
{
  uint8_t buf[1024];
  uint16_t len = moreData?contentLen|0x8000:contentLen;
  buf[0] = SNIC_HTTP_MORE_REQ;
  buf[1] = seq;
  *((uint16_t*)&buf[2]) = swap16(len);
  if (contentLen+4 > sizeof(buf))
  {
    return -1;
  }
  memcpy(&buf[4], content, contentLen);
  Serial_Transmit(CMD_ID_SNIC, buf, contentLen+4, ACK_NOT_REQUIRED);
  return 0;
}

/**
 * @brief Print the menu
 */
void ShowMenu(UART_HandleTypeDef* huart)
{
  uprintf(huart,"----------------------\n\r");
  uprintf(huart,"0 Get WiFi status\n\r");
  uprintf(huart,"1 Wifi Scan\n\r");
  uprintf(huart,"2 Wifi Join\n\r");
  uprintf(huart,"3 Get IP\n\r");
  uprintf(huart,"4 TCP client\n\r");
  uprintf(huart,"5 TCP sever\n\r");
  uprintf(huart,"6 Send from sock\n\r");
  uprintf(huart,"7 WiFi Leave\n\r");
  uprintf(huart,"8 AP On/Off\n\r");
  uprintf(huart,"9 UDP client\n\r");
  uprintf(huart,"a UDP server\n\r");
  uprintf(huart,"b Wifi Off\n\r");
  uprintf(huart,"c Wifi On\n\r");
//  uprintf("d HTTP get req\n\r");
//  uprintf("e HTTP post req\n\r");
//  uprintf("f HTTP post Json req\n\r");
//  uprintf("g HTTP chunked post req\n\r");
//  uprintf("h HTTPS get req\n\r");
//  uprintf("i TLS client\n\r");
//  uprintf("j TLS server (HTTPS server)\n\r");
  uprintf(huart,"m: Show Menu\n\r");
  uprintf(huart,"q: press q to Quit \n\r");
  uprintf(huart,"-----------------------\n\r");
}

/**
 * @brief Send http request function
 * @param domain domain name
 * @param isHttps https or not
 * @return value
 */
int8_t SendHttpReqTest(UART_HandleTypeDef* huart,int8_t *domain, int8_t isHttps)
{
  int8_t tmp[100] = {0};
  int8_t method = 0; //GET
  int8_t contentType[] = "text/html";
  int8_t otherHeader[] = "";
  int8_t content[] = "";
  uint8_t timeout = 10;
  uprintf(huart,"Enter URI after the server name: ([CR] to accept %s)\n\r", uri);
//  uget_line(tmp, sizeof(tmp));
  uint8_t data,i;
  i = 0;
  while(1)
  {
    if(HAL_UART_Receive(huart,&data,1,50000) == HAL_OK)
    {
      if(data == In_CR)
      {
        break;
      }
      tmp[i] = data;
      i++;
      HAL_UART_Transmit(huart,&data,1,1000);
    }
  }
//  scanf("%s",tmp);
  uprintf(huart,"\n\r");

  if (strlen(tmp))
  {
    strcpy(uri, tmp);
  }
  return FillNSendHttpReq(seqNo++, domain, uri, method, contentType, otherHeader, strlen(content), content, timeout, 0, isHttps);
}

/**
 * @brief Send http post demo function
 * @param domain domain name
 * @return value
 */
int32_t SendHttpPostDemo(UART_HandleTypeDef* huart,int8_t *domain)
{
  int8_t content[256]={0};
  int8_t tmp[100];
  int8_t method = 1; //POST
  int8_t contentType[] = "text/html";
  int8_t otherHeader[] = "Accept-Language: en-US\r\n";
  uint8_t timeout = 10;

  uprintf(huart,"Enter URI after the server name: ([CR] to accept %s)\n\r", uri);
  uget_line(huart,tmp, sizeof(tmp));
//  scanf("%s",tmp);
  uprintf(huart,"\n\r");
  if (strlen(tmp))
  {
    strcpy(uri, tmp);
  }
  uprintf(huart,"Enter content to POST: \n\r");
//  uget_line(content, sizeof(content));
  uint8_t data,i;
  i = 0;
  while(1)
  {
    if(HAL_UART_Receive(&huart6,&data,1,50000) == HAL_OK)
    {
      if(data == In_CR)
      {
        break;
      }
      content[i] = data;
      i++;
      HAL_UART_Transmit(&huart6,&data,1,1000);
    }
  }
//  scanf("%s",content);
  uprintf(huart,"\n\r");
  if (strlen(uri)==0)
  {
    strcpy(uri, "/add.php");
  }
  return FillNSendHttpReq(seqNo++, domain, uri, method, contentType, otherHeader, strlen(content), content, timeout, 0,0);
}

/**
 * @brief Send http JSON post demo function
 * @param domain domain name
 * @return value
 */
int32_t SendHttpJsonPostDemo(UART_HandleTypeDef* huart,int8_t *domain)
{
  int8_t content[]="Params=%7B%22if%22%3A%22sta%22%7D&callback=jsonp1363359950547";
  int8_t tmp[100];

  int8_t method = 1; //POST
  int8_t contentType[] = "application/x-www-form-urlencoded";
  int8_t otherHeader[] = "Accept-Language: en-US\r\n";
  uint8_t timeout = 10;

  strcpy(uri, "/sws/wifi/stat");
  uprintf(huart,"Enter URI after the server name: ([CR] to accept %s)\n\r", uri);
  uget_line(huart,tmp, sizeof(tmp));
//  scanf("%s",tmp);
  uprintf(huart,"\n\r");
  if (strlen(tmp))
  {
    strcpy(uri, tmp);
  }
  uprintf(huart,"Enter content to POST: ([CR] to accept %s)\n\r", content);
  uget_line(huart,tmp, sizeof(tmp));
//  scanf("%s",tmp);
  uprintf(huart,"\n\r");
  if (strlen(tmp))
  {
    strcpy(content, tmp);
  }
  return FillNSendHttpReq(seqNo++, domain, uri, method, contentType, otherHeader, strlen(content), content, timeout, 0,0);
}

/**
 * @brief Send http chunk request test function
 * @param domain domain name
 * @return value
 */
int32_t SendHttpChunkReqTest(int8_t *domain)
{
  int8_t uri[] = "/rest/thermostatGetTime";
  int8_t method = 1; //POST
  int8_t contentType[] = "application/x-www-form-urlencoded";
  int8_t otherHeader[] = "Accept: text/html,application/xml\r\nAccept-Language: en-US\r\n";
  int8_t content[] = "mcu_serial_number_hex=00112233445566778899AA";
  int8_t content1[] = "&username=MyUsername&password=MyPassword";
  uint8_t timeout = 20;
  int16_t len = strlen(content); // more data

  // first chunk
  FillNSendHttpReq(seqNo++, domain, uri, method, contentType, otherHeader, len, content, timeout, 1, 0);
  HAL_Delay(1000);
  //second chunk
  FillNSendHttpMoreReq(seqNo++, strlen(content1), content1, 0);
  return 0;
}

/**
 * @brief Process user input data function
 */
void ProcessUserInput(UART_HandleTypeDef* huart)
{
  int8_t tmp[100];
  DBGU_InputReady = 0;

  uprintf(huart,"\n\r");

  if (key == 'q')
      quit_flag = 1;

  switch(key)
  {
    case '0':
      GetStatus(huart,seqNo++);
      break;

    case '1':
      WifiScan(huart,seqNo++);
      break;

    case '2':
      WifiDisconn(huart,seqNo++);
      WifiJoin(huart,seqNo++);
      SnicInit(huart,seqNo++);
      SnicIPConfig(huart,seqNo++);
      break;

    case '3':
      SnicInit(huart,seqNo++);
      SnicGetDhcp(huart,seqNo++);
      break;

    case '4':
      mysock = -1;
      TCPCreateSocket(huart,0, 0xFF, 0xFF, seqNo++, SNIC_TCP_CREATE_SOCKET_REQ);
      if (mysock != -1)
      {
        if (GetTCPInfo(huart) == CMD_ERROR)
        {
          uprintf(huart,"Invalid Server\n\r");
          break;
        }
        // This connection can receive data upto 0x0400=1K bytes at a time.
        TCPConnectToServer(huart,mysock, destIP, (uint16_t)destPort, 0x0400, 0x5, seqNo++);
      }
      break;

    case '5':
      if (SetTCPInfo(huart) == CMD_ERROR)
      {
        uprintf(huart,"Invalid Server to create\n\r");
        break;
      }
      mysock = -1;
      TCPCreateSocket(huart,1, srcIP, (uint16_t)srcPort, seqNo++, SNIC_TCP_CREATE_SOCKET_REQ);
      if (mysock != -1)
      {
        // This connection can receive data upto TEST_BUFFERSIZE at a time.
        TCPCreateConnection(huart,mysock, TEST_BUFFERSIZE, 0x5, seqNo++);
      }
      break;

    case '6':
    {
      int8_t tempstr[3];
      uint8_t datamode;
      int8_t sockstr[8] = {0};
      uint32_t sock;
      int8_t teststr[128];
      int32_t len;

      uprintf(huart,"Enter socket number to send from: \n\r");
      uget_line(huart,sockstr,sizeof(sockstr));
      uprintf(huart,"\n\r");
//      scanf("%s", sockstr);
      sock = strtol(sockstr, NULL, 0);
      uprintf(huart,"Content Option? (0: Default 128bytes will be sent) \n\r");
      uget_line(huart,tempstr,sizeof(tempstr));
      uprintf(huart,"\n\r");
//      scanf("%s", tempstr);
//      datamode = strtol(tempstr);
//      if (datamode)
//      {
//        uprintf("Enter payload to send (up to 128 bytes): \n\r");
//        uget_line(tempstr,sizeof(tempstr));
//        uprintf("\n\r");
//        scanf("%s", teststr);
//        len = (int32_t)strlen(teststr);
//        SendFromSock(sock, (uint8_t*)teststr, len, 2, seqNo++);
//      }
//      else

     SendFromSock(huart,sock, TxBuf, TEST_BUFFERSIZE, 2, seqNo++);
     pktcnt = 0;

      break;
    }
    case '7':
      SnicCleanup(huart,seqNo++);
      WifiDisconn(huart,seqNo++);
      break;

    case '8':
      ApOnOff(huart,seqNo++);
      break;

    case '9': //udp send
    {
      int32_t i;
      UDPCreateSocket(huart,0, 0, 0, seqNo++);
      if (mysock != -1)
      {
        if (GetUDPInfo(huart) == CMD_ERROR)
        {
          uprintf(huart,"Invalid Server\n\r");
          break;
        }
        uprintf(huart,"Send %d02\n\r", UDP_NUM_PKT);
        for (i=0; i<UDP_NUM_PKT; i++)
        {
          int32_t si = i % TEST_BUFFERSIZE + 1;
          SendSNIC(TxBuf, si);
          uprintf(huart,"%d02 %d02\n\r", i, si);
        }
        CloseSocket(huart,mysock,seqNo++);
      }
      break;
    }
    case 'a': //udp recv
    {
      uint16_t port = 43211;
      uint32_t ip = 0xAC1F0001; // 172.31.0.1
      UDPCreateSocket(huart,1, ip, port, seqNo++);
      UDPStartRecv(huart,mysock, 2048, seqNo++);
      break;
    }
    case 'b':
      SnicCleanup(huart,seqNo++);
      WifiOff(huart,seqNo++);
      break;

    case 'c':
      WifiOn(huart,seqNo++);
      break;

//    case 'd':
//      uprintf("Enter server name:  %s\n\r", domain);
//      uget_line(tmp, sizeof(tmp));
////      scanf("%s", tmp);
//      uprintf("\n\r");
//      if (strlen(tmp))
//      {
//        strcpy(domain, tmp);
//      }
//      SendHttpReqTest(domain, 0);
//      break;
//
//    case'e':
//      uprintf("Enter server name: ([CR] to accept %s)\n\r", domain);
//      uget_line(tmp, sizeof(tmp));
////      scanf("%s", tmp);
//      uprintf("\n\r");
//      if (strlen(tmp))
//      {
//        strcpy(domain, tmp);
//      }
//      SendHttpPostDemo(domain);
//      break;
//
//    case 'f':
//      uprintf("Make sure STA is connected to SN8200 soft AP.\n\r");
//      strcpy(domain, "sn8200.com");
//      uprintf("Enter server name: ([CR] to accept %s)\n\r", domain);
//      uget_line(tmp, sizeof(tmp));
////      scanf("%s", tmp);
//      uprintf("\n\r");
//      if (strlen(tmp))
//      {
//        strcpy(domain, tmp);
//      }
//      SendHttpJsonPostDemo(domain);
//      break;
//#if 1
//    case 'g':
//      strcpy(domain, "192.168.10.100");
//      uprintf("Enter server name (or the peer testclient IP, peer testclient should start TCP server on port 80): ([CR] to accept %s)\n\r", domain);
//      uget_line(tmp, sizeof(tmp));
////      scanf("%s", tmp);
//      uprintf("\n\r");
//      if (strlen(tmp))
//      {
//        strcpy(domain, tmp);
//      }
//      SendHttpChunkReqTest(domain);
//      break;
//#endif
//
//    case 'h':
//      uprintf("Enter server name: ([CR] to accept %s)\n\r", domain);
//      uget_line(tmp, sizeof(tmp));
////      scanf("%s", tmp);
//      uprintf("\n\r");
//      if (strlen(tmp))
//      {
//        strcpy(domain, tmp);
//      }
//      SendHttpReqTest(domain, 1);
//      break;
//
//    case 'i':
//      timeout1 = 5;
//      mysock = -1;
//      TCPCreateSocket(0, 0xFF, 0xFF, seqNo++, SNIC_TCP_CREATE_SIMPLE_TLS_SOCKET_REQ);  // use less memory in SN8200
//      HAL_Delay(500);
//      if (mysock != -1)
//      {
//        strcpy(Portstr, "443");
//        if (GetTCPInfo() == CMD_ERROR)
//        {
//          uprintf("Invalid Server\n\r");
//          break;
//        }
//        TCPConnectToServer(mysock, destIP, (unsigned short)destPort,0x0000,timeout1,seqNo++);
//        while ((sockConnected == -1) && timeout1)
//        {
//          HAL_Delay(500);
//          timeout1--;
//          if (sockClosed == mysock)
//          {
//            sockClosed = -1;
//            break;
//          }
//        }
//        if (sockConnected == mysock)
//        {
//          SendFromSock(mysock, (uint8_t*)GET_REQUEST, sizeof(GET_REQUEST)-1, 2, seqNo++);
//          sockConnected = -1;
//        }
//        else
//          {
//            uprintf("Connect failed.\n\r");
//          }
//      }
//      break;
//
//    case 'j': //ssl server
//      strcpy(Portstr, "443");
//      if (SetTCPInfo() == CMD_ERROR)
//      {
//        uprintf("Invalid Server to create\n\r");
//        break;
//      }
//      mysock = -1;
//      TCPCreateSocket(1, srcIP, (uint16_t)srcPort, seqNo++, SNIC_TCP_CREATE_ADV_TLS_SOCKET_REQ);
//      if (mysock != -1)
//      {
//        // This connection can receive data upto TEST_BUFFERSIZE at a time.
//        TCPCreateConnection(mysock, TEST_BUFFERSIZE, 0x5, seqNo++);
//      }
//      break;

    case 'm':
      ShowMenu(huart);
      break;

    default:
      break;
  }
}
/**
 * @} STM32F4_DISCOVERY_WIFI_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_WIFI End
 */

 /* --------------------------------- End Of File ------------------------------ */
