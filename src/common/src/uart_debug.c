/*
 * File: uart_debug.c
 *
 *  Written by duvallee.lee in 2018
 *
*/
#include "main.h"

// ---------------------------------------------------------------------------
#if defined(UART_DEBUG_OUTPUT)

UART_HandleTypeDef g_dubug_uart1                         =
{
   .Instance                                             = NULL,
};

#define MILLI_SECOND                                     1000

volatile unsigned int g_System_Start_Second              = 0;
volatile unsigned int g_System_Start_Milli_Second        = 0;

// ***************************************************************************
// Fuction      : _putc()
// Description  : 
// 
//
// ***************************************************************************
int _putc(unsigned char ch)
{
#if defined(UART_DEBUG_OUTPUT)
   return uart_debug_put(ch);
#else
   return(ch);
#endif
}


/* --------------------------------------------------------------------------
 * Name : debug_tick_timer_handler()
 *
 *
 * -------------------------------------------------------------------------- */
int uart_debug_put(unsigned char ch)
{
   if (g_dubug_uart1.Instance == NULL)
   {
      return 0;
   }
   if (HAL_UART_Transmit(&g_dubug_uart1, (uint8_t *) &ch, sizeof(ch), 0xFFFF) == HAL_OK)
   {
      return 0;
   }
   return ch;
}


/* --------------------------------------------------------------------------
 * Name : debug_tick_timer_handler()
 *
 *
 * -------------------------------------------------------------------------- */
void debug_tick_timer_handler()
{
   g_System_Start_Milli_Second++;

   if (g_System_Start_Milli_Second >= MILLI_SECOND)
   {
      g_System_Start_Second++;
      g_System_Start_Milli_Second                        = 0;
   }
}

/* --------------------------------------------------------------------------
 * Name : USER_Debug_UART_MspInit()
 *
 *
 * -------------------------------------------------------------------------- */
void USER_Debug_UART_MspInit(UART_HandleTypeDef *huart)
{
   GPIO_InitTypeDef GPIO_InitStruct;

   if (huart->Instance == USART1)
   {
      // Peripheral clock enable
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_USART1_CLK_ENABLE();

      GPIO_InitStruct.Pin                                = UART_DEBUG_RX_PIN | UART_DEBUG_TX_PIN;
      GPIO_InitStruct.Mode                               = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull                               = GPIO_NOPULL;
      GPIO_InitStruct.Speed                              = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate                          = GPIO_AF7_USART1;
      HAL_GPIO_Init(UART_DEBUG_RX_GPIO_PORT, &GPIO_InitStruct);
   }
}

/* --------------------------------------------------------------------------
 * Name : MX_USART1_UART_Init()
 *
 *
 * -------------------------------------------------------------------------- */
static void MX_USART1_UART_Init(void)
{
   g_dubug_uart1.Instance                                = USART1;
   g_dubug_uart1.Init.BaudRate                           = UART_DEBUG_BAUDRATE;
   g_dubug_uart1.Init.WordLength                         = UART_WORDLENGTH_8B;
   g_dubug_uart1.Init.StopBits                           = UART_STOPBITS_1;
   g_dubug_uart1.Init.Parity                             = UART_PARITY_NONE;
   g_dubug_uart1.Init.Mode                               = UART_MODE_TX_RX;
   g_dubug_uart1.Init.HwFlowCtl                          = UART_HWCONTROL_NONE;
   g_dubug_uart1.Init.OverSampling                       = UART_OVERSAMPLING_16;
#if !defined(STM32F401xC)
   g_dubug_uart1.Init.OneBitSampling                     = UART_ONE_BIT_SAMPLE_DISABLE;
   g_dubug_uart1.AdvancedInit.AdvFeatureInit             = UART_ADVFEATURE_NO_INIT;
#endif
   if (HAL_UART_Init(&g_dubug_uart1) != HAL_OK)
   {
      g_dubug_uart1.Instance                             = NULL;
      _Error_Handler(__FILE__, __LINE__);
   }
}


/* --------------------------------------------------------------------------
 * Name : uart_debug_init()
 *
 *
 * -------------------------------------------------------------------------- */
void uart_debug_init()
{
   MX_USART1_UART_Init();
}

#endif      // UART_DEBUG_OUTPUT


