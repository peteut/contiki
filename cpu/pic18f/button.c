// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#include <signal.h>

#include "contiki.h"
#include "io.h"
#include "dev/button.h"

/* FIXME this config stuff doesn't belong here */
#define BUTTON_PORT B
#define BUTTON_PIN 0

#ifndef BUTTON_PORT
  #error "*** BUTTON_PORT not defined! Possible value(s): 'B' ***"
#endif

#ifndef BUTTON_PIN
  #error "*** BUTTON_PIN not defined! Possible range: '0'..'2' ***"
#endif

static struct button_msg button_msg;
static struct process* selecting_proc;

void button_init(struct process* proc)
{
  button_msg.type = BUTTON_MSG_TYPE;

  CC_CONCAT(TRIS, BUTTON_PORT) |= BV(BUTTON_PIN);
#if BUTTON_PIN == 0
  INTCON |= BV(4);
  INTCON &= ~BV(1);
#else
  INTCON3 |= BV(3 + (BUTTON_PIN - 1));
  INTCON3 &= ~BV(BUTTON_PIN - 1);
#endif

  selecting_proc = proc;
  if(proc != NULL) {
#if BUTTON_PIN == 0
    INTCON |= BV(4);
#else
    INTCON3 |= BV(3 + (BUTTON_PIN - 1));
#endif
  } else {
#if BUTTON_PIN == 0
    INTCON &= ~BV(4);
#else
    INTCON3 &= ~BV(3 + (BUTTON_PIN - 1));
#endif
  }
}

SIGHANDLER(button_interrupt)
{
  static struct timer debouncetimer;

#if BUTTON_PIN == 0
  INTCON &= ~BV(2);
#else
  INTCON3 &= ~BV(BUTTON_PIN - 1);
#endif
  if (timer_expired(&debouncetimer)) {
    button_msg.value = CC_CONCAT(PORT, BUTTON_PORT) & BV(BUTTON_PIN);
    timer_set(&debouncetimer, CLOCK_SECOND / 4);
    if (selecting_proc != NULL) {
      process_post(selecting_proc, PROCESS_EVENT_MSG, &button_msg);
    }
  }
}
