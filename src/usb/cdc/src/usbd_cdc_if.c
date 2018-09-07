/*
 *  File: usbd_custom_hid_if.c
 *
 * COPYRIGHT(c) 2018 MICROVISION Co., Ltd.
 *
*/

#include "main.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "soft_timer.h"

// ==========================================================================
#define APP_RX_DATA_SIZE                                 2048
#define APP_TX_DATA_SIZE                                 2048

static uint8_t UserRxBuffer[APP_RX_DATA_SIZE];
static uint8_t UserTxBuffer[APP_TX_DATA_SIZE];

// ==========================================================================
static int g_usb_rx_event_id                             = -1;
static int8_t g_usb_rx_event_count                       = 0;

static int g_usb_last_rx_received_len                    = 0;

/* --------------------------------------------------------------------------
 * Name : get_usb_rx_event_id()
 *
 *
 * -------------------------------------------------------------------------- */
int get_usb_rx_event_id()
{
   return g_usb_rx_event_id;
}

/* --------------------------------------------------------------------------
 * Name : get_usb_rx_data()
 *
 *
 * -------------------------------------------------------------------------- */
uint8_t* get_usb_rx_data(int* rx_len)
{
   *rx_len                                               = g_usb_last_rx_received_len;
   if (g_usb_rx_event_count > 0)
   {
      g_usb_rx_event_count--;
   }
   g_usb_last_rx_received_len                            = 0;
   return (UserRxBuffer);
}


// ==========================================================================
USBD_CDC_LineCodingTypeDef LineCoding                    =
{
   115200,                                               // baud rate
   0x00,                                                 // stop bits-1
   0x00,                                                 // parity - none
   0x08                                                  // nb. of bits 8
};

// ==========================================================================
// Prescaler declaration
uint32_t uwPrescalerValue                                = 0;


// ==========================================================================
uint32_t UserTxBufPtrIn                                  = 0;
uint32_t UserTxBufPtrOut                                 = 0;

// ==========================================================================
extern USBD_HandleTypeDef gUSBD_Device;

// ==========================================================================
static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t * pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t * pbuf, uint32_t * Len);

USBD_CDC_ItfTypeDef USBD_CDC_fops                        =
{
   CDC_Itf_Init,
   CDC_Itf_DeInit,
   CDC_Itf_Control,
   CDC_Itf_Receive
};

/* --------------------------------------------------------------------------
 * Name : CDC_Itf_Event_Init()
 *
 *
 *
 * -------------------------------------------------------------------------- */
int8_t CDC_Itf_Event_Init()
{
   // add event
   g_usb_rx_event_id                                     = add_event(g_usb_rx_event_count);
   if (g_usb_rx_event_id < 0)
   {
      debug_output_error("add_event() failed \r\n");
      _Error_Handler(__FILE__, __LINE__);
      return -1;
   }

   return 0;
}


/* --------------------------------------------------------------------------
 * Name : CDC_Itf_Init()
 * @brief  Initializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 * -------------------------------------------------------------------------- */
static int8_t CDC_Itf_Init(void)
{
   USBD_CDC_SetTxBuffer(&gUSBD_Device, UserTxBuffer, 0);
   USBD_CDC_SetRxBuffer(&gUSBD_Device, UserRxBuffer);

   return (USBD_OK);
}

/* --------------------------------------------------------------------------
 * Name : CDC_Itf_DeInit()
 * @brief  CDC_Itf_DeInit
 *         DeInitializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 * -------------------------------------------------------------------------- */
static int8_t CDC_Itf_DeInit(void)
{
   return (USBD_OK);
}

/* --------------------------------------------------------------------------
 * Name : CDC_Itf_Control()
 * @brief  CDC_Itf_Control
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 * -------------------------------------------------------------------------- */
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t * pbuf, uint16_t length)
{
   switch (cmd)
   {
      case CDC_SEND_ENCAPSULATED_COMMAND :
         // Add your code here
         break;

      case CDC_GET_ENCAPSULATED_RESPONSE :
         // Add your code here
         break;

      case CDC_SET_COMM_FEATURE :
         // Add your code here
         break;

      case CDC_GET_COMM_FEATURE :
         // Add your code here
         break;

      case CDC_CLEAR_COMM_FEATURE :
         // Add your code here
         break;

      case CDC_SET_LINE_CODING :
         LineCoding.bitrate                              = (uint32_t) (pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
         LineCoding.format                               = pbuf[4];
         LineCoding.paritytype                           = pbuf[5];
         LineCoding.datatype                             = pbuf[6];
         break;

      case CDC_GET_LINE_CODING :
         pbuf[0]                                         = (uint8_t) (LineCoding.bitrate);
         pbuf[1]                                         = (uint8_t) (LineCoding.bitrate >> 8);
         pbuf[2]                                         = (uint8_t) (LineCoding.bitrate >> 16);
         pbuf[3]                                         = (uint8_t) (LineCoding.bitrate >> 24);
         pbuf[4]                                         = LineCoding.format;
         pbuf[5]                                         = LineCoding.paritytype;
         pbuf[6]                                         = LineCoding.datatype;
         break;

      case CDC_SET_CONTROL_LINE_STATE :
         // Add your code here
         break;

      case CDC_SEND_BREAK :
         // Add your code here
         break;

      default:
         break;
   }

   return (USBD_OK);
}

/* --------------------------------------------------------------------------
 * Name : CDC_Itf_Receive()
 *
 *
 *
 * -------------------------------------------------------------------------- */
static int8_t CDC_Itf_Receive(uint8_t* Buf, uint32_t* Len)
{
//   SCB_CleanDCache_by_Addr((uint32_t *) Buf, *Len);
   USBD_CDC_SetRxBuffer(&gUSBD_Device, UserRxBuffer);
   if (USBD_CDC_ReceivePacket(&gUSBD_Device) != USBD_OK)
   {
      return -1;
   }
   memcpy(UserRxBuffer, Buf, *Len);
   g_usb_last_rx_received_len                            = *Len;

   g_usb_rx_event_count++;
   set_event(g_usb_rx_event_id, g_usb_rx_event_count);
   return 0;
}

/* --------------------------------------------------------------------------
 * Name : CDC_Itf_Transmitter()
 *
 *
 *
 * -------------------------------------------------------------------------- */
int8_t CDC_Itf_Transmitter(uint8_t* pBuffer, uint16_t size)
{
   USBD_CDC_SetTxBuffer(&gUSBD_Device, (uint8_t *) pBuffer, size);
   if (USBD_CDC_TransmitPacket(&gUSBD_Device) != USBD_OK)
   {
      return -1;
   }
   return 0;
}








