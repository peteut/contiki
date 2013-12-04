// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief WP0710B has 2 red LEDs, OP_MODE is mapped to LEDS_RED,
 * PWR_GOOD to LEDS_GREEN.
 */
#include "contiki-conf.h"
#include "dev/leds.h"
#include "io.h"
#include "dev/hwconf.h"

HWCONF_PIN_NO_MX(op_mode, B, 5)
HWCONF_PIN_NO_MX(pwr_good, C, 7)

void leds_arch_init(void)
{
  op_mode_clear();
  pwr_good_clear();
  op_mode_make_output();
  pwr_good_make_output();
}

unsigned char leds_arch_get(void)
{
  unsigned char l = 0;

  if(op_mode_read()) {
    l |= LEDS_RED;
  }
  if(pwr_good_read()) {
    l |= LEDS_GREEN;
  }
  return l;
}

void leds_arch_set(unsigned char leds)
{
  if(leds & LEDS_RED) {
    op_mode_set();
  } else {
    op_mode_clear();
  }

  if(leds & LEDS_GREEN) {
    pwr_good_set();
  } else {
    pwr_good_clear();
  }
}
