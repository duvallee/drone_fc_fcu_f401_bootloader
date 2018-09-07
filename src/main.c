/*
 * File: main.c
 *
 * Written by duvallee.lee in 2018
 *
 */
#include "main.h"
#include "battery_gauge.h"
#include "led.h"
#if defined(DSTM32F401_USB_BULK_DEVICE)
#include "usb_bulk_device.h"
#endif


// ***************************************************************************
// Fuction      : SystemClock_Config()
// Description  : 
// 
//
// ***************************************************************************
void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct;
   RCC_ClkInitTypeDef RCC_ClkInitStruct;

   // Configure the main internal regulator output voltage 
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

   // Initializes the CPU, AHB and APB busses clocks 
   RCC_OscInitStruct.OscillatorType                      = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState                            = RCC_HSE_ON;
   RCC_OscInitStruct.PLL.PLLState                        = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource                       = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM                            = 16;
   RCC_OscInitStruct.PLL.PLLN                            = 336;
   RCC_OscInitStruct.PLL.PLLP                            = RCC_PLLP_DIV4;
   RCC_OscInitStruct.PLL.PLLQ                            = 7;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      _Error_Handler(__FILE__, __LINE__);
   }

   // Initializes the CPU, AHB and APB busses clocks 
   RCC_ClkInitStruct.ClockType                           = RCC_CLOCKTYPE_HCLK       |
                                                           RCC_CLOCKTYPE_SYSCLK     |
                                                           RCC_CLOCKTYPE_PCLK1      |
                                                           RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource                        = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider                       = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider                      = RCC_HCLK_DIV2;
   RCC_ClkInitStruct.APB2CLKDivider                      = RCC_HCLK_DIV1;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
   {
      _Error_Handler(__FILE__, __LINE__);
   }

   // Configure the Systick interrupt time 
   HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

   // Configure the Systick 
   HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

   // SysTick_IRQn interrupt configuration
   HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


// ***************************************************************************
// Fuction      : main()
// Description  : 
// 
//
// ***************************************************************************
int main(void)
{
   HAL_Init();
   SystemClock_Config();

   uart_debug_init();

   debug_output_info("=============================================== \r\n");
   debug_output_info("%s Ver%d.%d.%d \r\n", PROGRAM_NAME, VERSION_MAIN, VERSION_MINOR, VERSION_SUB);
   debug_output_info("Build Date : %s %s (%s) \r\n", __DATE__, __TIME__, __VERSION__);
   debug_output_info("=============================================== \r\n\r\n");

   scheduler_init();

   //
   led_init();
   set_led_1_mode(LED_ON_MODE);
   set_led_2_mode(LED_OFF_MODE);

   //
   Battery_Gauge_Init();

#if defined(DSTM32F401_USB_BULK_DEVICE)
   usb_bulk_device_init();
#endif

   while (1)
   {
      scheduler_run();
   }
}


// ***************************************************************************
// Fuction      : _Error_Handler()
// Description  : 
// 
//
// ***************************************************************************
void _Error_Handler(char * file, int line)
{
   while(1) 
   {
   }
}

#ifdef USE_FULL_ASSERT

// ***************************************************************************
// Fuction      : _Error_Handler()
// Description  : 
// 
//
// ***************************************************************************
void assert_failed(uint8_t* file, uint32_t line)
{
}
#endif

