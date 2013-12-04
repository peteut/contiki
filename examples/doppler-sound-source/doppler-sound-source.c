// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief A class D sound process, using a LUT.
 */
#include <signal.h>
#include "io.h"
#include "dev/hwconf.h"
#include "contiki-pic18f.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"

PROCESS(sound_process, "Sound process");
AUTOSTART_PROCESSES(&sound_process);

#pragma udata acessram idx
static volatile uint8_t idx = 0;
extern volatile uint8_t update_pwm;

#ifndef _CONCAT
#undef  _CONCAT2
#define _CONCAT(a,b)  _CONCAT2(a,b)
#define _CONCAT2(a,b) a ## b
#endif

#ifndef PWM_FREQ
#define PWM_FREQ 126000l
#endif

#ifndef ULTRASONIC_FREQ
#define ULTRASONIC_FREQ 40000l
#endif

#if PWM_FREQ >= (PIC18F_CPU_SPEED / (4 * 0xff))
#define SETUP_T2_PWM() \
	do { \
		/* prescaler defaults to 1 */ \
		PR2 = ((PIC18F_CPU_SPEED / 4) / PWM_FREQ) - 1; \
		TMR2 = 0; \
	} while(0)
#elif PWM_FREQ >= (PIC18F_CPU_SPEED / (16 * 0xff))
#define SETUP_T2_PWM() \
	do { \
		/* set prescaler to 4 */ \
		T2CON =  0x01; \
		PR2 = ((PIC18F_CPU_SPEED / 16) / PWM_FREQ) - 1; \
		TMR2 = 0; \
	} while(0)
#elif PWM_FREQ >= (PIC18F_CPU_SPEED / (64 * 0xff))
#define SETUP_T2_PWM() \
	do { \
		/* set prescaler to 16 */ \
		T2CON = 0x02; \
		PR2 = ((PIC18F_CPU_SPEED / 64) / PWM_FREQ) - 1; \
		TMR2 = 0; \
	} while(0)
#else
#error Unable to set PWM frequency!
#endif

#if ULTRASONIC_FREQ >= (PIC18F_CPU_SPEED / (4 * 0xff))
#define SETUP_T2_ULTRASONIC() \
	do { \
		/* prescaler defaults to 1 */ \
		PR2 = ((PIC18F_CPU_SPEED / 4) / ULTRASONIC_FREQ) - 1; \
		TMR2 = 0; \
	} while(0)
#elif ULTRASONIC_FREQ >= (PIC18F_CPU_SPEED / (16 * 0xff))
#define SETUP_T2_ULTRASONIC() \
	do { \
		/* set prescaler to 4 */ \
		T2CON =  0x01; \
		PR2 = ((PIC18F_CPU_SPEED / 16) / ULTRASONIC_FREQ) - 1; \
		TMR2 = 0; \
	} while(0)
#elif ULTRASONIC_FREQ >= (PIC18F_CPU_SPEED / (64 * 0xff))
#define SETUP_T2_ULTRASONIC() \
	do { \
		/* set prescaler to 16 */ \
		T2CON = 0x02; \
		PR2 = ((PIC18F_CPU_SPEED / 64) / ULTRASONC_FREQ) - 1; \
		TMR2 = 0; \
	} while(0)
#else
#error Unable to set PWM frequency!
#endif


typedef enum {PWM_INIT, PWM_PRESET, PWM_DISABLE, PWM_ENABLE} pwm_conf_t;

HWCONF_PIN_NO_MX(ccp1out, C, 2)
HWCONF_PIN(ccp2out, B, 3)
HWCONF_PIN_NO_MX(clkout, A, 6)
HWCONF_PIN(boost_en, A, 1)
HWCONF_PIN(pid, B, 4)
HWCONF_PIN(id0, A, 3)
HWCONF_PIN_NO_MX(id1, A, 4)
HWCONF_PIN(_12v_en, A, 2)

void process_start_dummy(void)
{
	process_start(&sound_process, NULL);
}

void pwm_configure(pwm_conf_t pwm_conf)
{
	switch(pwm_conf) {
		case PWM_INIT:
			CCP1CON = CCP2CON = 0x00; /* disable PWM */
			ccp2out_select_io();
			ccp1out_make_output();
			ccp2out_make_output();
			ccp1out_clear();
			ccp2out_clear();
			SLRCON = 0x01; /* PORTB, PORTC standard rate */
			pid_select_io();
			pid_make_output();
			pid_clear();
			id0_select_io();
			_12v_en_select_io();
			_12v_en_make_output();
			if (!(id1_read() | id0_read())) {
				SETUP_T2_PWM();
				PSTRCON = 0x09;
				_12v_en_clear();
			} else {
				SETUP_T2_ULTRASONIC();
				PSTRCON = 0x08;
				CCPR1L = (PR2 >> 1);
			}
			PIR1 &= ~(_TMR2IF);
			PIE1 |= _TMR2IE;
			INTCON |= _PEIE;
			T2CON |= _TMR2ON;
			break;
		case PWM_PRESET:
			ccp1out_set();
			ccp2out_set(); /* start condition */
			break;
		case PWM_DISABLE:
			CCP1CON = CCP2CON = 0x00;
			if (!(id1_read() | id0_read())) {
				ccp1out_set();
				ccp2out_set(); /* restore start condition */
			} else {
				pid_clear();
			}
			break;
		case PWM_ENABLE:
			idx = 31;
			if (!(id1_read() | id0_read())) {
				CCP1CON = CCP2CON = 0x0c; /* set PWM mode */
			} else {
				CCP1CON = 0x0c;
			}
			break;
	}
}

void exit_handler(void)
{
	pwm_configure(PWM_DISABLE);
	T2CON &= ~(_TMR2ON);
	boost_en_select_io();
	boost_en_clear();
	boost_en_make_output();
}

PROCESS_THREAD(sound_process, ev, data)
{
	static struct etimer etimer;
	static struct etimer flash;

	PROCESS_EXITHANDLER(exit_handler());
  PROCESS_BEGIN();
	/* do the setup here */
	pwm_configure(PWM_INIT);
	clkout_make_output();
	clkout_set();
	etimer_stop(&etimer);
	etimer_set(&etimer, CLOCK_SECOND / 2);
	etimer_set(&flash, CLOCK_SECOND * 2);
	button_sensor.configure(SENSORS_HW_INIT, 0);
	SENSORS_ACTIVATE(button_sensor);
	pwm_configure(PWM_PRESET);
	leds_on(LEDS_GREEN); /* turn the LED (User feedback) */
	/* wait to ensure proper start condition */
	PROCESS_WAIT_EVENT();
	leds_off(LEDS_GREEN);
	etimer_restart(&etimer);
	PROCESS_WAIT_EVENT();
	pwm_configure(PWM_ENABLE);
	leds_on(LEDS_RED);
	while (1) {
		PROCESS_WAIT_EVENT();
		if (ev == PROCESS_EVENT_TIMER && data == &flash) {
			etimer_reset(&flash);
			etimer_set(&etimer, CLOCK_SECOND / 16);
			leds_on(LEDS_GREEN);
		}
		if (ev == PROCESS_EVENT_TIMER && data == &etimer) {
			leds_off(LEDS_GREEN);
		}
		if (ev == sensors_event && data == &button_sensor) {
			process_post(PROCESS_CURRENT(), PROCESS_EVENT_EXIT, NULL);
		}
	}
	PROCESS_END();
}

SIGHANDLER(tmr2_tick)
{
	static const uint8_t sinus_lut[] = {
		0x45, 0x3a,
		0x51, 0x30,
		0x5b, 0x27,
		0x61, 0x20,
		0x65, 0x1b,
		0x65, 0x19,
		0x61, 0x1a,
		0x5b, 0x20,
		0x53, 0x28,
		0x49, 0x33,
		0x3f, 0x3f,
		0x35, 0x4b,
		0x2b, 0x56,
		0x23, 0x5e,
		0x1d, 0x64,
		0x19, 0x65,
		0x19, 0x63,
		0x1d, 0x5e,
		0x23, 0x57,
		0x2d, 0x4e,
		0x39, 0x44,
		0x3a, 0x45,
		0x30, 0x51,
		0x27, 0x5b,
		0x20, 0x61,
		0x1b, 0x65,
		0x19, 0x65,
		0x1a, 0x61,
		0x20, 0x5b,
		0x28, 0x53,
		0x33, 0x49,
		0x3f, 0x3f,
		0x4b, 0x35,
		0x56, 0x2b,
		0x5e, 0x23,
		0x64, 0x1d,
		0x65, 0x19,
		0x63, 0x19,
		0x5e, 0x1d,
		0x57, 0x23,
		0x4e, 0x2d,
		0x44, 0x39};

	PIR1 &= ~(_TMR2IF);

	if (CCP1CON) {
		CCPR1L = sinus_lut[idx++];
		CCPR2L = sinus_lut[idx++];
		if (!(idx < sizeof(sinus_lut))) {
			idx = 0;
    }
	}
	clkout_toggle();
}
