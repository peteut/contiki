// -*-coding: utf-8 -*- vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief PIC18F-specific rtimer code.
 */
#include "sys/energest.h"
#include "sys/rtimer.h"
#include "io.h"

void rtimer_arch_init(void)
{
  /* To run during sleep TMR1ON, TMR1E, PEIE, T1SYNC, TMR1CS, T1OSCEN must
   * be set!
   */
  di();
  TMR3H = TMR3L = 0;
  /* set the compare match value */
  CCPR1H = 0;
  CCPR1L = 40;
  /* enable the external oscillator (32768Hz), set prescaler to 8 */
  T1CON |= _T1OSCEN | _T1CKPS1 | _T1CKPS0 | _NOT_T1SYNC | _TMR1CS;
  /* clock source is external */
  T3CON |= _T3CON_TMR3CS | _T3CON_TMR3ON;
  /* Compare mode, trigger special event */
  CCP1CON = _CCP1M3 | _CCP1M1 | _CCP1M0;
  PIR1 &= ~(_CCP1IF);
  PIE1 |= _CCP1IE;
  INTCON |= _PEIE;
  ei();
}

void rtimer_arch_schedule(rtimer_clock_t t)
{
  CCPR1H = t >> 8;
  CCPR1L = 0xFF & t;
}
