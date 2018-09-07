/*
 *  File: main.h
 *
 * COPYRIGHT(c) 2018 MICROVISION Co., Ltd.
 *
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif


#define LED_RSSI_Pin                                     GPIO_PIN_13
#define LED_RSSI_GPIO_Port                               GPIOC

#define MAIN_12V_ADC_Pin                                 GPIO_PIN_0
#define MAIN_12V_ADC_GPIO_Port                           GPIOC

#define ADC_3V3_Pin                                      GPIO_PIN_1
#define ADC_3V3_GPIO_Port                                GPIOC

#define DI_3V3_Pin                                       GPIO_PIN_2
#define DI_3V3_GPIO_Port                                 GPIOC

#define DO_3V3_OUT_Pin                                   GPIO_PIN_3
#define DO_3V3_OUT_GPIO_Port                             GPIOC

#define GP_PA0_Pin                                       GPIO_PIN_1
#define GP_PA0_GPIO_Port                                 GPIOA

#define LED_PWR_Pin                                      GPIO_PIN_4
#define LED_PWR_GPIO_Port                                GPIOC

#define LED_LINK_Pin                                     GPIO_PIN_5
#define LED_LINK_GPIO_Port                               GPIOC

#define RESET_BUTTON_Pin                                 GPIO_PIN_0
#define RESET_BUTTON_GPIO_Port                           GPIOB

#define WISUN_IRQ_Pin                                    GPIO_PIN_1
#define WISUN_IRQ_GPIO_Port                              GPIOB

#define WATCHDOGTIMER_EN_Pin                             GPIO_PIN_2
#define WATCHDOGTIMER_EN_GPIO_Port                       GPIOB

#define USIM_EN_Pin                                      GPIO_PIN_10
#define USIM_EN_GPIO_Port                                GPIOB

#define USIM_RESET_Pin                                   GPIO_PIN_7
#define USIM_RESET_GPIO_Port                             GPIOC

#define LED_ORANGE_Pin                                   GPIO_PIN_9
#define LED_ORANGE_GPIO_Port                             GPIOC

#define RS485_MODE_SEL_Pin                               GPIO_PIN_15
#define RS485_MODE_SEL_GPIO_Port                         GPIOA

#define RS485_DE_Pin                                     GPIO_PIN_12
#define RS485_DE_GPIO_Port                               GPIOC
#define RS485_INDICATOR_Pin                              GPIO_PIN_3
#define RS485_INDICATOR_GPIO_Port                        GPIOB

#define SECUREMODULE_RESET_Pin                           GPIO_PIN_5
#define SECUREMODULE_RESET_GPIO_Port                     GPIOB

void _Error_Handler(char *, int);
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif   // __MAIN_H__

