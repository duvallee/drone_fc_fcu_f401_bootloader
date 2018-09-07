/*
 *  File: led.h
 *
 *  Written by duvallee.lee in 2018
 *
*/

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif 

// ==========================================================================
enum LED_MODE
{
   LED_OFF_MODE                                          = 0,
   LED_ON_MODE                                           = 1,
   LED_BLINK_MODE                                        = 2,
};

// ==========================================================================
void led_init();

void set_led_1_blink_time(int ms);
void set_led_2_blink_time(int ms);
void set_led_1_mode(enum LED_MODE mode);
void set_led_2_mode(enum LED_MODE mode);

#ifdef __cplusplus
}
#endif


#endif   // __LED_H__

