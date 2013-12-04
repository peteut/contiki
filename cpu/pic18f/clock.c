// -*-coding: utf-8 -*- vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file clock.c
 * @brief Clock support for the PIC18 port.
 */
#include <signal.h>
#include "pic18f.h"
#include "io.h"

#include "sys/clock.h"
#include "sys/etimer.h"

#include "dev/leds.h"

#define INTERVAL (RTIMER_ARCH_SECOND / CLOCK_SECOND)
#undef INTERVAL
#define INTERVAL ((PIC18F_CPU_SPEED / (256 * 4)) / CLOCK_SECOND)

static volatile unsigned long seconds;
static volatile clock_time_t count = 0;

/**
 * @brief Get current time.
 */
clock_time_t clock_time(void)
{
  clock_time_t got_count;
  do {
    got_count = count;
  } while (got_count != count);

  return count;
}

/**
 * @brief Init timer subsystem.
 */
void clock_init(void)
{
  di();
  /* Setup Timer 1 as 32768Hz Xtal */
  T3CON |= _T3CON_RD16;
  /* async mode */
  T1CON |= _T1CKPS1 | _T1CKPS0 | _T1OSCEN | _NOT_T1SYNC | _TMR1CS;

  T0CON = _T0PS0 | _T0PS1 | _T0PS2; /* disable TMR0, 16bit, prescaler 1:256 */
  TMR0H = (uint8_t)((0xffff - INTERVAL) >> 8);
  TMR0L = (uint8_t)((0xffff - INTERVAL) & 0xff);
  T1CON |= _TMR0ON;
  INTCON |= _PEIE | _TMR0IE;
  /* Enable interrupts. */
  ei();
}

void clock_delay(unsigned int len)
{
  if (len) {
    while (--len) {
    }
  }
}

/**
 * @brief Seconds (since boot).
 */
unsigned long clock_seconds(void)
{
  static unsigned long t1;
  do {
    t1 = seconds;
  } while(t1 != seconds);
  return t1;
}

SIGHANDLER(tick_it)
{
  /* Preset */
  TMR0H = (uint8_t)((0xffff - INTERVAL) >> 8);
  TMR0L = (uint8_t)((0xffff - INTERVAL) & 0xff);
  /* clear flag */
  INTCON &= ~(_TMR0IF);

#ifdef DEBUG_TOGGLE_TICK
  leds_toggle(LEDS_GREEN);
#endif

  /* increase tick timer */
  ++count;

  /* Make sure the CLOCK_CONF_SECOND is a power of two, to ensure
     that the modulo operation below becomes a logical and and not
     an expensive divide. Algorithm from Wikipedia:
     http://en.wikipedia.org/wiki/Power_of_two */
#if (CLOCK_CONF_SECOND & (CLOCK_CONF_SECOND - 1)) != 0
#error CLOCK_CONF_SECOND must be a power of two (i.e., 1, 2, 4, 8, 16, ...).
#error Change CLOCK_CONF_SECOND in contiki-conf.h.
#endif

  if (count % CLOCK_CONF_SECOND == 0)
    ++seconds;

  if (etimer_pending())
    etimer_request_poll();
}
