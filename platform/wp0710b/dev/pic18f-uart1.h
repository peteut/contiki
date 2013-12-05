// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef PIC18F_UART1_H
#define PIC18F_UART1_H

/* FIXME: implement
#define	UART_RX				RXBUF1
#define	UART_TX				TXBUF1
#define UART_RESET_RX()		do { U1IFG &= ~URXIFG1; } while(0)
#define	UART_RESET_RXTX()	do { U1IFG &= ~(URXIFG1 | UTXIFG1); } while(0)
#define	UART_WAIT_RX()		while((U1IFG & URXIFG1) == 0) { _NOP(); }
#define	UART_WAIT_TX()		while((U1IFG & UTXIFG1) == 0) { _NOP(); }
#define UART_WAIT_TXDONE()	while((UTCTL1 & TXEPT) == 0) { _NOP(); }
*/

typedef int(*uart_handler_t)(unsigned char);

void uart_init(void);
void uart_set_speed(unsigned, unsigned, unsigned, unsigned);
void uart_set_handler(unsigned, uart_handler_t);
int uart_lock(unsigned);
int uart_lock_wait(unsigned);
int uart_unlock(unsigned);
void uart_set_mode(unsigned);
int uart_get_mode(void);

#endif /* PIC18F_UART1_H */
