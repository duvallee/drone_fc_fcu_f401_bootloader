/*
 *  File: usbd_bulk_if.c
 *
 * Written by duvallee.lee in 2018
 *
 */

#include "main.h"
#include "usbd_core.h"
#include "usb_bulk.h"
#include "usbd_bulk_if.h"
// #include "soft_timer.h"


// ==========================================================================
#define APP_RX_DATA_SIZE                                 512
static uint8_t UserRxBufferHS[APP_RX_DATA_SIZE];
static uint8_t UserTxBufferHS[APP_RX_DATA_SIZE];

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
   return (UserRxBufferHS);
}


/* --------------------------------------------------------------------------
 * Name : BULK_Custom_Init()
 *
 *
 *
 * -------------------------------------------------------------------------- */
void BULK_Custom_Init()
{
   // add event
   g_usb_rx_event_id                                     = add_event(g_usb_rx_event_count);
   if (g_usb_rx_event_id < 0)
   {
      debug_output_error("add_event() failed \r\n");
      _Error_Handler(__FILE__, __LINE__);
      return;
   }
}


// ==========================================================================
extern USBD_HandleTypeDef gUSBD_Device;

static int8_t BULK_Init_HS(void);
static int8_t BULK_DeInit_HS(void);
// static int8_t BULK_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t BULK_Receive_HS(uint8_t* Buf, uint32_t *Len);

// ==========================================================================
USBD_BULK_ItfTypeDef USBD_Interface_fops_HS              = 
{
   BULK_Init_HS,
   BULK_DeInit_HS,
   BULK_Receive_HS
};

/* --------------------------------------------------------------------------
 * Name : BULK_Init_HS()
 *
 *
 *
 * -------------------------------------------------------------------------- */
static int8_t BULK_Init_HS(void)
{
   USBD_BULK_SetTxBuffer(&gUSBD_Device, UserTxBufferHS, 0);
   USBD_BULK_SetRxBuffer(&gUSBD_Device, UserRxBufferHS);

   return (USBD_OK);
}

/* --------------------------------------------------------------------------
 * Name : BULK_DeInit_HS()
 *
 *
 *
 * -------------------------------------------------------------------------- */
static int8_t BULK_DeInit_HS(void)
{
  return (USBD_OK);
}

/* --------------------------------------------------------------------------
 * Name : BULK_Receive_HS()
 *
 *
 *
 * -------------------------------------------------------------------------- */
static int8_t BULK_Receive_HS(uint8_t* Buf, uint32_t *Len)
{
   USBD_BULK_SetRxBuffer(&gUSBD_Device, UserRxBufferHS);
   if (USBD_BULK_ReceivePacket(&gUSBD_Device) != USBD_OK)
   {
      return -1;
   }
//   memcpy(UserRxBuffer, Buf, *Len);
   g_usb_last_rx_received_len                            = *Len;

   g_usb_rx_event_count++;
   set_event(g_usb_rx_event_id, g_usb_rx_event_count);
   return 0;
}

/* --------------------------------------------------------------------------
 * Name : BULK_Receive_HS()
 *
 *
 *
 * -------------------------------------------------------------------------- */
uint8_t BULK_Transmit_HS(uint8_t* Buf, uint16_t Len)
{
   USBD_BULK_HandleTypeDef* hcdc                         = (USBD_BULK_HandleTypeDef*) (&gUSBD_Device)->pClassData;
   uint32_t cur_ms_time                                  = 0;

   if (hcdc == NULL)
   {
      return BULK_TRANSMIT_ERROR_NULL_POINTER;
   }

   USBD_BULK_SetTxBuffer(&gUSBD_Device, Buf, Len);
   if (USBD_BULK_TransmitPacket(&gUSBD_Device) != USBD_OK)
   {
      return BULK_TRANSMIT_ERROR;
   }

   // blocking transmit -- need to wait until transmit is complete
   // move this up to stlink level

   cur_ms_time                                           = get_cur_time_ms();
   while (((volatile uint32_t) hcdc->TxState))
   {
      us_delay(10);
      // wait 1000 ms ...
      if (get_spend_time_ms(cur_ms_time) > 1000)
      {
         debug_output_error("BULK Transmitt Time-Out !!! \r\n");
         return BULK_TRANSMIT_ERROR_TIME_OUT;
      }
   }
   return BULK_TRANSMIT_SUCCESS;
}


