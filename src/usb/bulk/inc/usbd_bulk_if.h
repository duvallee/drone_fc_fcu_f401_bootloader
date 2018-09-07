/*
 *  File: usbd_bulk_if.h
 *
 * Written by duvallee.lee in 2018
 *
 */

#ifndef __USBD_BULK_IF_H__
#define __USBD_BULK_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

void BULK_Custom_Init();
int get_usb_rx_event_id();

uint8_t* get_usb_rx_data(int* rx_len);

#define BULK_TRANSMIT_SUCCESS                            0
#define BULK_TRANSMIT_ERROR_NULL_POINTER                 1
#define BULK_TRANSMIT_ERROR                              2
#define BULK_TRANSMIT_ERROR_TIME_OUT                     3
#define BULK_TRANSMIT_ERROR_UNPLUG_CABLE                 4
uint8_t BULK_Transmit_HS(uint8_t* Buf, uint16_t Len);


#ifdef __cplusplus
}
#endif

#endif   // __USBD_BULK_IF_H__




