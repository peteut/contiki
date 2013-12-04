// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#include "pic18fdef.h"

#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS                1
#define CC_CONF_INLINE

#define SERIAL_LINE_CONF_BUFSIZE 32

#define CCIF
#define CLIF

typedef unsigned short clock_time_t;

/**
 * The statistics data type.
 *
 * This datatype determines how high the statistics counters are able
 * to count.
 */
typedef unsigned short uip_stats_t;

#define PROCESS_CONF_NUMEVENTS 8
#define PROCESS_CONF_STATS 8

/* CPU target speed in Hz */
#define F_CPU 64000000ul

/* Our clock resolution, this is the same as Unix HZ. Must be a power
   of two (see clock.c for details). */
#define CLOCK_CONF_SECOND 64

/* TMR1CS set clk input */
#define TMR1CS_CONF 0

#define rs232_rx() uart_rx()

/* SPI configuration */

/* SPI input/output registers. */
#define SPI_TXBUF SSPBUF
#define SPI_RXBUF SSPBUF

#define SPI_WAITFOREOTx() while (BF != 1)
#define SPI_WAITFOREORx() while (BF != 1)

/* platform specific defines */

/* SPI */
#define SSPSTAT_CONF 0x00 /* CKE = 1 */
#define SSPCON_CONF 0x00 /* SSPEN = 1 */

/* 1 = in, 0 = out */

/* TRISA5 downto TRISA0 */
#define TRISA_CONF 0xff
/* RA5 downto RA0 */
#define PORTA_INIT 0x00

#define WPUA_CONF 0x00

/* TRISB7 downto TRISB0 */
#define TRISB_CONF 0x01
/* RB7 downto RB4 */
#define PORTB_INIT 0x00
/* WPUB7 downto WPUB0 */
#define WPUB_CONF 0x10
/* TODO: Global RABPU bit of the OPTION register must be enabled for individual pull-ups to be enabled. */

/* TRISC7 downto TRISC0 */
#define TRISC_CONF 0x03
/* RC7 downto RC0 */
#define PORTC_INIT 0x00

/* ANS7 downto ANS0 (1 = analog) */
#define ANSEL_CONF 0xde

/* ANS12 downto ANS8 */
#define ANSELH_CONF 0x00

#define BUTTON_PIN 0

#define eeprom_wr(addr, value) \
do { \
  while (WR) continue; EEADR = (addr); EEDAT = (value); \
  C = 0; if (GIE) C = 1; GIE = 0; \
  WREN = 1; EECON2 = 0x55; EECON2 = 0xAA; WR = 1; WREN = 0; \
  if (C) GIE = 1; \
} while (0)

#define eeprom_rd(addr) ((EEADR = (addr)), (RD = 1), EEDAT)

#endif /* __CONTIKI_CONF_H__ */
