// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#include "dev/button-sensor.h"
#include "dev/hwconf.h"
#include <signal.h>

static struct timer debouncetimer;

HWCONF_PIN(button, B, 0)
HWCONF_IRQ(button, B, 0)

SIGHANDLER(button_interrupt)
{
  button_clear_flag();
  if (timer_expired(&debouncetimer)) {
    timer_set(&debouncetimer, CLOCK_SECOND / 4);
    sensors_changed(&button_sensor);
  }
}

static int value(int type)
{
  return button_read() || !timer_expired(&debouncetimer);
}

static int configure(int type, int value)
{
  switch (type) {
    case SENSORS_HW_INIT:
      button_irq_edge_selectd();
      button_select_io();
      button_make_input();
      return 1;
    case SENSORS_ACTIVE:
      if(value) {
        if(!button_irq_enabled()) {
          timer_set(&debouncetimer, 0);
          button_enable_irq();
        }
      } else {
        button_disable_irq();
      }
      return 1;
  }
  return 0;
}

static int status(int type)
{
  switch (type) {
    case SENSORS_ACTIVE:
      break;
    case SENSORS_READY:
      return button_irq_enabled();
  }
  return 0;
}

SENSORS_SENSOR(button_sensor, BUTTON_SENSOR,
    value, configure, status);
