// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief Machine dependent PIC18F UART1 code.
 */
#include <usart.h>
#include <signal.h>
#include <stdio.h>

#include "dev/pic18f-uart1.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "io.h"

static int (*uart1_input_handler)(unsigned char c);

void uart1_set_input(int (*input)(unsigned char c))
{
  uart1_input_handler = input;
}

/**
 * @brief Initalize the RS232 port.
 */
void uart1_init(unsigned short ubr)
{
  usart_open(
    USART_TX_INT_OFF &
    USART_RX_INT_ON &
    USART_BRGH_HIGH &
    USART_CONT_RX &
    USART_ASYNCH_MODE &
    USART_EIGHT_BIT,
    ubr);
  stdout = STREAM_USART;
}

SIGHANDLER(uart1_rx)
{
  if (usart_drdy())
    uart1_input_handler(usart_getc());
}
