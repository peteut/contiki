// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/*
 * FIXME: to be implemented.
 */
#ifndef __RS232_H__
#define __RS232_H__

#include "dev/pic18f-uart1.h"

enum rs232_speed {
	RS232_9600  =	0,
	RS232_19200 =	1,
	RS232_38400 =	2,
	RS232_57600 =	3,
	RS232_115200 =	4
};

/**
 * @brief Initialize the RS232 module
 *
 * This function is called from the boot up code to
 * initalize the RS232 module.
 */
void rs232_init(void);

/**
 * @brief Set an input handler for incoming RS232 data.
 * @param f A pointer to a byte input handler
 *
 * This function sets the input handler for incoming RS232
 * data. The input handler function is called for every
 * incoming data byte. The function is called from the
 * RS232 interrupt handler, so care must be taken when
 * implementing the input handler to avoid race
 * conditions.
 *
 * The return value of the input handler affects the sleep
 * mode of the CPU: if the input handler returns non-zero
 * (true), the CPU is awakened to let other processing
 * take place. If the input handler returns zero, the CPU
 * is kept sleeping.
 */
void rs232_set_input(uart_handler_t f);

/**
 * @brief Configure the speed of the RS232 hardware.
 * @param speed The speed
 *
 * This function configures the speed of the RS232
 * hardware. The allowed parameters are RS232_9600,
 * RS232_19200, RS232_38400, RS232_57600, and RS232_115200.
 */
void rs232_set_speed(enum rs232_speed speed);

/**
 * @brief Print a text string on RS232.
 * @param text A pointer to the string that is to be printed
 *
 * This function prints a string to RS232. The string must
 * be terminated by a null byte. The RS232 module must be
 * correctly initalized and configured for this function
 * to work.
 */
void rs232_print(char *text);

/**
 * @brief Print a character on RS232.
 * @param c The character to be printed
 *
 * This function prints a character to RS232. The RS232
 * module must be correctly initalized and configured for
 * this function to work.
 */
void rs232_send(char c);

#endif /* __RS232_H__ */
