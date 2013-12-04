// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brie fMain system file for the PIC18 port.
 */
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "contiki.h"
#include "contiki-pic18f.h"
#include "io.h"
#include "dev/hwconf.h"
#include "dev/serial-line.h"
#include "dev/watchdog.h"
#include "dev/leds.h"

/* FIXME PLL needs internal HF clock to be defined in CONFIG1H */
extern uint8_t __at(__CONFIG1H) config1h = _FOSC_INTIO67_1H;
extern uint8_t __at(__CONFIG2L) config2l = _BOREN_OFF_2L & _PWRT_OFF_2L;
extern uint8_t __at(__CONFIG2H) config2h = _WDTEN_OFF_2H & _WDTPS_32768_2H;
extern uint8_t __at(__CONFIG3H) config3h = _MCLRE_ON_3H & _CCP2MX_PORTBE_3H;
extern uint8_t __at(__CONFIG4L) config4l = _STVREN_ON_4L & _LVP_OFF_4L &
 	_XINST_OFF_4L & _DEBUG_ON_4L;
extern uint8_t __at(__CONFIG5L) config5l = _CP0_OFF_5L;
extern uint8_t __at(__CONFIG5H) config5h = _CPB_OFF_5H;
extern uint8_t __at(__CONFIG6L) config6l = _WRT0_OFF_6L;
extern uint8_t __at(__CONFIG6H) config6h = _WRTC_OFF_6H;
extern uint8_t __at(__CONFIG7L) config7l = _EBTR0_OFF_7L;
extern uint8_t __at(__CONFIG7H) config7h = _EBTRB_OFF_7H;

PROCINIT(&etimer_process, &sensors_process);
SENSORS(&button_sensor);

int main(void)
{
#if PIC18F_CPU_SPEED == 64000000UL
  OSCCON = (1 << 7) | (7 << 4);
  OSCTUNE |= _PLLEN; /* 16e6Hz * 4 */
#elif PIC18F_CPU_SPEED == 32000000UL
  OSCCON = (1 << 7) | (6 << 4);
  OSCTUNE |= _PLLEN; /* 8e6Hz * 4 */
#elif PIC18F_CPU_SPEED == 16000000UL
  OSCCON = (1 << 7) | (7 << 4);
#elif PIC18F_CPU_SPEED == 8000000UL
  OSCCON = (1 << 7) | (6 << 4);
#elif PIC18F_CPU_SPEED == 4000000UL
  OSCCON = (1 << 7) | (5 << 4);
#elif PIC18F_CPU_SPEED == 2000000UL
  OSCCON = (1 << 7) | (4 << 4);
#elif PIC18F_CPU_SPEED == 1000000UL
  OSCCON = (1 << 7) | (3 << 4);
#elif PIC18F_CPU_SPEED == 500000UL
  OSCCON = (1 << 7) | (2 << 4);
#elif PIC18F_CPU_SPEED == 250000UL
  OSCCON = (1 << 7) | (1 << 4);
#elif PIC18F_CPU_SPEED == 31000UL
  OSCCON = (1 << 7);
#else
#error 'PIC18F_CPU_SPEED' not defined!
#endif

  cpu_init();
  watchdog_stop();

  /* Platform-specific initialization. */
  clock_init();
  leds_init();
  process_init();

  /* System services */
  procinit_init();

  leds_off(LEDS_ALL);

  autostart_start(autostart_processes);
  /*
   * This is the scheduler loop.
   */
  while (1) {
    int r;

    do {
      /* Reset watchdog. */
      watchdog_periodic();
      r = process_run();
    } while(r > 0);

    /*
     * Idle processing.
     */
    if (process_nevents() != 0) {
      ;
    } else {
#ifdef DEBUG_IDLE_MODE
      leds_on(LEDS_RED);
#endif
      if(!CCP2CON)
        Sleep();
#ifdef DEBUG_IDLE_MODE
      leds_off(LEDS_RED);
#endif
      /*
       * We get the current processing time for interrupts that was
       * done during the LPM and store it for next time around.
       */
    }
  }
  return 0;
}
