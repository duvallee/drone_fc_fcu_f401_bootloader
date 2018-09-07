/*
 *  File: usb_bulk_device.c
 *
 *  Written by duvallee.lee in 2018
 *
 */
#include "main.h"
#include "usb_bulk_device.h"
#include "usb_bulk.h"
#include "usbd_bulk_desc.h"


// ===============================================================================
USBD_HandleTypeDef gUSBD_Device;
extern USBD_BULK_ItfTypeDef USBD_Interface_fops_HS;


// ***************************************************************************
// Fuction      : usb_bulk_device_init()
// Description  : 
// 
//
// ***************************************************************************
int usb_bulk_device_init()
{
   debug_output_info("=============================================== \r\n");

   // Init Device Library
   USBD_Init(&gUSBD_Device, &BULK_Desc, 0);

   // Add Supported Class
   USBD_RegisterClass(&gUSBD_Device, &USBD_BULK);

   // Add CDC Interface Class
   USBD_BULK_RegisterInterface(&gUSBD_Device, &USBD_Interface_fops_HS);

   // add event for usb rx
   BULK_Custom_Init();

   // Start Device Process
   USBD_Start(&gUSBD_Device);

   return 0;
}



