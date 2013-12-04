// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief UART1 definitions.
 */
#ifndef __UART1_H__
#define __UART1_H__

#include "contiki-conf.h"

#define UART1_BAUD2UBR(baud) ((F_CPU) / (4 * (baud + 1)))

void uart1_set_input(int (*input)(unsigned char c));
void uart1_writeb(unsigned char c);
void uart1_init(unsigned short ubr);

#endif /* __UART1_H__ */
