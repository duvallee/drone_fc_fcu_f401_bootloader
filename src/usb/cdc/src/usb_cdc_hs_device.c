/*
 *  File: usb_cdc_hs_device.c
 *
 * COPYRIGHT(c) 2018 MICROVISION Co., Ltd.
 *
*/

#include "main.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_cdc_desc.h"
#include "usb_cdc_hs_device.h"
#include "usbd_cdc_if.h"
#include "soft_timer.h"
#include "charger.h"

// ===============================================================================
USBD_HandleTypeDef gUSBD_Device;

static void wait_event_usb_cable_detect(uint8_t status);
extern USBD_CDC_ItfTypeDef USBD_CDC_fops;

/* --------------------------------------------------------------------------
 * Name : usb_otg_hs_init()
 *
 *
 * -------------------------------------------------------------------------- */
uint8_t usb_cdc_hs_device_init()
{
   int usb_vbus_event_id                                 = -1;
   uint8_t usb_cable_status; 

   debug_output_info(" \r\n");

   // Init Device Library
   USBD_Init(&gUSBD_Device, &VCP_Desc, 0);

   // Add Supported Class
   USBD_RegisterClass(&gUSBD_Device, USBD_CDC_CLASS);

   // Add CDC Interface Class
   USBD_CDC_RegisterInterface(&gUSBD_Device, &USBD_CDC_fops);

   // add event for usb rx
   CDC_Itf_Event_Init();

   // Start Device Process
   USBD_Start(&gUSBD_Device);

   HAL_PWREx_EnableUSBVoltageDetector();

   //
   usb_vbus_event_id                                     = get_usb_vbus_event_id();
   if (usb_vbus_event_id < 0)
   {
      debug_output_error("can't get event id for usb cable detect \r\n");
      return -1;
   }
   add_wait_event_fn(usb_vbus_event_id, wait_event_usb_cable_detect);
   get_event_status(usb_vbus_event_id, &usb_cable_status);

   debug_output_info("usb cable status : %d \r\n", usb_cable_status);

   // ...
   usb_vbus_power_control(0);

   return 0;
}





/* --------------------------------------------------------------------------
 * Name : wait_event_usb_cable_detect()
 *
 *
 * -------------------------------------------------------------------------- */
static void wait_event_usb_cable_detect(uint8_t status)
{
   debug_output_info("VBUS Detect event !!! : %d \r\n", status);

#if 0
   if (status)
   {
      // Start Device Process
      USBD_Start(&g_usbd_hid_device);
   }
   else
   {
      USBD_Stop(&g_usbd_hid_device);
   }
#endif
}





