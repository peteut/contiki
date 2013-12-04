// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#include <signal.h>

#include "io.h"
#include "pic18fdef.h"
#include "dev/watchdog.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"

/**
 * @brief Initialise ports.
 */
static void init_ports(void)
{
#define init_port(tris_reg, port_reg, tris_val, port_val) do { \
  port_reg = port_val; \
  tris_reg = tris_val; \
} while (0)

#ifdef TRISA_CONF
#ifdef PORTA_INIT
	init_port(TRISA, PORTA, TRISA_CONF, PORTA_INIT);
#else
#error TRISA_CONF/ PORTA_INIT not defined!
#endif
#endif

#ifdef TRISB_CONF
#ifdef PORTB_INIT
	 init_port(TRISB, PORTB, TRISB_CONF, PORTB_INIT);
#else
#error TRISB_CONF / PORTB_INIT not defined!
#endif
#endif

#ifdef TRISC_CONF
#ifdef PORTC_INIT
	 init_port(TRISC, PORTC, TRISC_CONF, PORTC_INIT);
#else
#error TRISC_CONF / PORTC_INIT not defined!
#endif
#endif

#ifdef TRISD_DONF
#ifdef PORTD_INIT
	 init_port(TRISD, PORTD, TRISD_CONF, PORTD_INIT);
#else
#error TRISD_CONF / PORTD_INIT not defined!
#endif
#endif

#ifdef TRISE_DONF
#ifdef PORTE_INIT
	 init_port(TRISE, PORTE, TRISE_CONF, PORTE_INIT);
#else
#error TRISE_CONF / PORTE_INIT not defined!
#endif
#endif


#ifndef ANSEL_CONF
#error ANSEL_CONF not defined!
#else
  ANSEL = ANSEL_CONF;
#endif

#ifndef ANSELH_CONF
#error ANSELH_CONF not defined!
#else
  ANSELH = ANSELH_CONF;
#endif
#undef init_port

#ifndef WPUB_CONF
#error WPUB_CONF not defined!
#else
  WPUB = WPUB_CONF;
  /* Weak pull-ups enabled */
#endif

  INTCON2 &= ~(_NOT_RBPU);
}
/**
 * @brief Initialise SPI.
 */
static void init_spi(void)
{
  if (SSPCON1 & _SSPEN) {
    return;
  }
#ifndef SSPSTAT_CONF
#error SSPSTAT_CONF not defined!
#endif
#ifndef SSPCON_CONF
#error SSPCON_CONF not defined!
#endif
  SSPSTAT = SSPSTAT_CONF;
  SSPCON1 = SSPCON_CONF;
}
/**
 * @brief Initialise CPU.
 */
void pic18f_cpu_init(void)
{
  di();
  INTCON &= ~(_TMR0IP); /* low priority for system tick  */
  RCON |= _IPEN; /* enable interrupt priority */
  watchdog_init();
//  init_ports();
#if defined SPI
  #warning "*** spi support enabled ***"
  init_spi();
#else
  #warning "*** SPI not defined, init_spi() not called ***"
#endif

#if defined RTIMER
  #warning "*** rtimer support enabled ***"
  rtimer_arch_init(); // FIXME maybe not available, start in platform code
#else
  #warning "*** RTIMER not defined, rtimer_arch_init() not called ***"
#endif
  ei();
}

/**
 * @brief Mask all interrupts that can be masked.
 */
spl_t splhigh_(void)
{
  /* Clear the GIE (General Interrupt Enable) flag. */
  spl_t sr;
  sr = INTCON;
  return sr & _GIE;		/* Ignore other sr bits. */
}

/**
 * @brief Restore previous interrupt mask.
 */
void splx_(spl_t sr)
{
  /* If GIE was set, restore it. */
  INTCON |= sr;
}


/* Declare handler to be the handler function for the given signal.
 * sig should be one of SIG_xxx from above,
 * sig2 should also be a signal (probably SIG_xxxIE from below) and
 * handler should be a function defined using SIGHANDLER(handler)
 * or SIGHANDLERNAKED(handler).
 * To be used together with DEF_INTHIGH and DEF_INTLOW.
 *
 * Added extern definition, since the handler may be defined in another
 * module.
 */
#undef DEF_HANDLER2
#define DEF_HANDLER2(sig1,sig2,handler) \
  __asm \
    btfss sig1; \
    bra   $+8; \
    btfsc sig2; \
    extern _ ## handler; \
    goto  _ ## handler; \
  __endasm;

DEF_INTHIGH(high_handler)
  DEF_HANDLER2(SIG_RC, SIG_RCIE, uart1_rx)
/*  DEF_HANDLER2(SIG_TMR0, SIG_TMR0IE, tick_it) */
#if defined(TMR2_HANDLER)
  DEF_HANDLER2(SIG_TMR2, SIG_TMR2IE, tmr2_tick)
#endif
#if BUTTON_PIN == 0
#warning "*** INT0 IRQ handler: button_interrupt ***"
  DEF_HANDLER2(SIG_INT0, SIG_INT0IE, button_interrupt)
#elif BUTTON_PIN == 1
#warning "*** INT0 IRQ handler: button_interrupt ***"
  DEF_HANDLER2(SIG_INT1, SIG_INT1IE, button_interrupt)
#elif BUTTON_PIN == 2
#warning "*** INT0 IRQ handler: button_interrupt ***"
  DEF_HANDLER2(SIG_INT2, SIG_INT2IE, button_interrupt)
#endif
END_DEF

DEF_INTLOW(low_handler)
  DEF_HANDLER2(SIG_TMR0, SIG_TMR0IE, tick_it)
END_DEF
