/******************************************************************//**
* @file   stm32f4_discovery_wifi.h
* @brief  This file contains declaration for stm32f4_discovery_wifi.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_WIFI_H
#define __STM32F4_DISCOVERY_WIFI_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_WIFI
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_WIFI_Public_Types
 * @{
 */

#define ACK_REQUIRED 1
#define ACK_NOT_REQUIRED 0

// Maximum payload len
#define MAX_PAYLOAD_LEN 8000

// RX TX Buffer size
#define RX_BUFFER_SIZE 4096
#define SEND_BUF_SIZE 4096

#define SERIAL_TX(x) Dummy_TX(x)

// Special character defines
#define SOM_CHAR 0x02
#define EOM_CHAR 0x04
#define ESC_CHAR 0x10
#define ACK_CMD 0x7F
#define NAK_CMD 0x00

#define DBGU_RX_BUFFER_SIZE 256
#define TEST_BUFFERSIZE 128
#define UDP_NUM_PKT 10

#define swap16(in) htons(in)
#define swap32(in) htonl(in)

#define NUM_RX_BUF 10

#define PORT_NONE 0
#define CMD_ERROR -1
#define SUB_CMD_RESP_MASK 0x80 // Bit 7: 0 for original command, 1 for response
#define MAX_CONNECTION_PER_SOCK 4   // max connection per listening socket
#define MAX_BUFFER_SIZE 0x800

/* BSD Internet Addresses */
#define INADDR_ANY     0x00000000 /* All IP addresses accepted               */
#define INADDR_NONE    0xffffffff /* No IP address accepted                  */

#define __REV   __rev
/**
 * @brief  Reverse byte order (32 bit)
 *                 This function reverses the byte order in integer value.
 *  @param [in]    value  Value to reverse
 *  @return        Reversed value
 */
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t __REV(uint32_t value)
{
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
  return __builtin_bswap32(value);
#else
  uint32_t result;

  __ASM volatile ("rev %0, %1" : __CMSIS_GCC_OUT_REG (result) : __CMSIS_GCC_USE_REG (value) );
  return(result);
#endif
}

#define U32_BE(v)      (uint32_t)(__rev(v))
#define U16_BE(v)      (uint16_t)(__rev(v) >> 16)
#define ntohs(v)        U16_BE(v)
#define ntohl(v)        U32_BE(v)
#define htons(v)        ntohs(v)
#define htonl(v)        ntohl(v)

typedef struct in_addr
{          /* << Generic IPv4 Address structure >>    */
  union
  {
    struct
    {
      uint8_t s_b1,s_b2,s_b3,s_b4;     /* IP address, byte access                 */
    };
    struct
    {
      uint16_t s_w1,s_w2;              /* IP address, short int access            */
    };
    uint32_t s_addr;                   /* IP address in network byte order        */
  };
} IN_ADDR;

typedef enum {
    WIFI_ON_REQ = 0,
    WIFI_OFF_REQ,
    WIFI_JOIN_REQ,
    WIFI_DISCONNECT_REQ,
    WIFI_GET_STATUS_REQ,
    WIFI_SCAN_REQ,
    WIFI_GET_STA_RSSI_REQ,
    WIFI_AP_CTRL_REQ,

    WIFI_NETWORK_STATUS_IND = 0x10,
    WIFI_SCAN_RESULT_IND,
    WIFI_RSSI_IND,

    WIFI_ON_RSP = 0x80,
    WIFI_OFF_RSP,
    WIFI_JOIN_RSP,
    WIFI_DISCONNECT_RSP,
    WIFI_GET_STATUS_RSP,
    WIFI_SCAN_RSP,
    WIFI_GET_STA_RSSI_RSP,
    WIFI_AP_CTRL_RSP,
} WIFI_SubCmd_Id_Typedef;

enum {
    WIFI_SUCCESS,
    WIFI_FAIL,
    WIFI_NETWORK_UP = 0x10,
    WIFI_NETWORK_DOWN,
};

typedef enum {
    CMD_ID_NACK = 0,  // reservered
    CMD_ID_WIFI = 0x50,
    CMD_ID_SNIC = 0x70,
    CMD_ID_ACK = 0x7F,
} Cmd_Id_Typedef;


typedef struct {
    uint8_t available;        //bool type
    uint8_t payload_len;
    uint8_t ack_reqd;
    uint8_t cmd_id;
    uint8_t rx_payload[MAX_PAYLOAD_LEN];
    uint8_t chksum;
    uint8_t ackOk;
} RX_Info_Typedef;

typedef enum {
    IDLE,
    SOM_RECD,
    LEN_RECD,
    ACK_SEQ_RECD,
    CMD_RECD,
    PAYLAD_RX,
    PAYLAD_RX_ESC,
    CHKSUM_RECD,
    EOM_RECD,
    WAIT_FOR_ACK_NAK,
} Serial_RX_State_Typedef;

typedef enum {
    MODE_WIFI_OFF,
    MODE_NO_NETWORK,
    MODE_STA_JOINED,
    MODE_AP_STARTED,
    MODE_SNIC_INIT_NOT_DONE,
    MODE_SNIC_INIT_DONE,

    /* Non-mode special values */
    MODE_LIST_END,
    MODE_ANY,
} Serial_Wifi_Mode_Typedef;

typedef enum {
    SNIC_INIT_REQ = 0,
    SNIC_CLEANUP_REQ,
    SNIC_SEND_FROM_SOCKET_REQ,
    SNIC_CLOSE_SOCKET_REQ,
    SNIC_SOCKET_PARTIAL_CLOSE_REQ,
    SNIC_GETSOCKOPT_REQ,
    SNIC_SETSOCKOPT_REQ,
    SNIC_SOCKET_GETNAME_REQ,
    SNIC_SEND_ARP_REQ,
    SNIC_GET_DHCP_INFO_REQ,
    SNIC_RESOLVE_NAME_REQ,
    SNIC_IP_CONFIG_REQ,

    SNIC_TCP_CREATE_SOCKET_REQ = 0x10,
    SNIC_TCP_CREATE_CONNECTION_REQ,
    SNIC_TCP_CONNECT_TO_SERVER_REQ,

    SNIC_UDP_CREATE_SOCKET_REQ,
    SNIC_UDP_START_RECV_REQ,
    SNIC_UDP_SIMPLE_SEND_REQ,
    SNIC_UDP_SEND_FROM_SOCKET_REQ,

    SNIC_HTTP_REQ,
    SNIC_HTTP_MORE_REQ,
    SNIC_HTTPS_REQ,

    SNIC_TCP_CREATE_ADV_TLS_SOCKET_REQ,
    SNIC_TCP_CREATE_SIMPLE_TLS_SOCKET_REQ,

    SNIC_TCP_CONNECTION_STATUS_IND = 0x20,
    SNIC_TCP_CLIENT_SOCKET_IND,
    SNIC_CONNECTION_RECV_IND,
    SNIC_UDP_RECV_IND,
    SNIC_ARP_REPLY_IND,
    SNIC_HTTP_RSP_IND,

    SNIC_SEND_RSP = 0x82,
    SNIC_CLOSE_SOCKET_RSP,
    SNIC_GET_DHCP_INFO_RSP = 0x89,
    SNIC_IP_CONFIG_RSP = 0x8B,
    SNIC_TCP_CREATE_SOCKET_RSP = 0x90,
    SNIC_TCP_CREATE_CONNECTION_RSP,
    SNIC_TCP_CONNECT_TO_SERVER_RSP,

    SNIC_UDP_CREATE_SOCKET_RSP = 0x93,
    SNIC_UDP_SEND_FROM_SOCKET_RSP = 0x96,

    SNIC_TCP_CREATE_ADV_TLS_SOCKET_RSP = 0x9A,
    SNIC_TCP_CREATE_SIMPLE_TLS_SOCKET_RSP,
} SNIC_SubCmd_Id_Typedef;

typedef enum {
    SNIC_SUCCESS=0,
    SNIC_FAIL,
    SNIC_INIT_FAIL,
    SNIC_CLEANUP_FAIL,
    SNIC_GETADDRINFO_FAIL,
    SNIC_CREATE_SOCKET_FAIL,
    SNIC_BIND_SOCKET_FAIL,
    SNIC_LISTEN_SOCKET_FAIL,
    SNIC_ACCEPT_SOCKET_FAIL,
    SNIC_PARTIAL_CLOSE_FAIL,
    SNIC_CONNECTION_PARTIALLY_CLOSED = 0x0A,
    SNIC_CONNECTION_CLOSED,
    SNIC_CLOSE_SOCKET_FAIL,
    SNIC_PACKET_TOO_LARGE,
    SNIC_SEND_FAIL,
    SNIC_CONNECT_TO_SERVER_FAIL,
    SNIC_NOT_ENOUGH_MEMORY = 0x10,
    SNIC_TIMEOUT,
    SNIC_CONNECTION_UP,
    SNIC_GETSOCKOPT_FAIL,
    SNIC_SETSOCKOPT_FAIL,
    SNIC_INVALID_ARGUMENT,
    SNIC_SEND_ARP_FAIL,
    SNIC_INVALID_SOCKET,
    SNIC_CONNECT_TO_SERVER_PENDING,
    SNIC_SOCKET_NOT_BOUND,
    SNIC_SOCKET_NOT_CONNECTED,
} SNIC_Return_Code_Typedef;

typedef struct {
    int32_t ch;
    int32_t rssi;
    int32_t sectype;
    int8_t SSIDname[33];
} Scanlist_Typedef ;

extern UART_HandleTypeDef huart1;
extern uint8_t RxBuffer[RX_BUFFER_SIZE];
extern uint32_t RxBufferTail;
extern uint32_t RxBufferHead;
extern int8_t domain[100];
extern int8_t uri[100];
extern uint8_t key;
extern uint8_t seqNo;
extern uint8_t DBGU_InputReady;
extern uint8_t quit_flag;
/**
 * @} STM32F4_DISCOVERY_WIFI_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_WIFI_Public_Functions
 * @{
 */
int8_t FillNSendHttpMoreReq(uint8_t seq, int32_t contentLen, int8_t* content, int8_t moreData);
uint8_t FillNSendHttpReq(uint8_t seq, int8_t* domain, int8_t* uri, int8_t method, int8_t* contentType, int8_t* otherHeader,
    int32_t contentLen, int8_t* content, uint8_t timeout, int8_t moreData, int8_t isHttps);

void SendSNIC(uint8_t *buf, int32_t size);
void ProcessSN8200Input(void);
uint8_t UDPStartRecv(uint32_t sock, uint16_t bufsize, uint8_t seq);
uint8_t UDPSendFromSock(uint32_t ip, uint16_t iPort, uint8_t shortsock, uint8_t conMode, uint8_t *sendbuf, uint16_t len, uint8_t seq);
uint8_t UDPCreateSocket(uint8_t bindOption, uint32_t ip, uint16_t port, uint8_t seq);
uint8_t SendFromSock(uint8_t shortSocket, uint8_t * sendBuf, uint16_t len, uint8_t timeout, uint8_t seq);
uint8_t TCPCreateConnection(uint8_t shortSock, uint16_t size, uint8_t maxClient, uint8_t seq);
uint8_t TCPConnectToServer(uint8_t shortSock, uint32_t ip, uint16_t port, uint16_t bufsize, uint8_t timeout, uint8_t seq);
uint8_t CloseSocket(uint8_t shortSocket, uint8_t seq);
uint8_t TCPCreateSocket(uint8_t bindOption, uint32_t localIp, uint16_t port, uint8_t seq, uint8_t ssl);
void SnicGetDhcp(uint8_t seq);
void SnicIPConfig(uint8_t seq);
void SnicCleanup(uint8_t seq);
void SnicInit(uint8_t seq);
void WifiDisconn(uint8_t seq);
void WifiJoin(uint8_t seq);
void WifiScan(uint8_t seq);
void ApOnOff(uint8_t seq);
void WifiOff(uint8_t seq);
void WifiOn(uint8_t seq);
void GetStatus(uint8_t seq);
int8_t SN8200_HasInput(void);
int8_t SetUDPInfo(void);
int8_t GetUDPInfo(void);
int8_t SetTCPInfo(void);
int8_t GetTCPInfo(void);
void SN8200_HAL_Init(uint32_t baudrate);
uint16_t UART_ReceiveData(UART_HandleTypeDef* huart);
void ShowMenu(void);
int8_t SendHttpReqTest(int8_t *domain, int8_t isHttps);
int32_t SendHttpPostDemo(int8_t *domain);
int32_t SendHttpJsonPostDemo(int8_t *domain);
int32_t SendHttpChunkReqTest(int8_t *domain);
void ProcessUserInput(void);

/**
 * @} STM32F4_DISCOVERY_WIFI_Public_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_WIFI_H */

/**
 * @} STM32F4_DISCOVERY_WIFI End
 */

/* --------------------------------- End Of File ------------------------------ */
