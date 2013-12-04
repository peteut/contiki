// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief Platform dependent watchdog functions.
 */
#include "io.h"

static int stopped = 0;

/**
 * @brief Initialise the watchdog.
 */
void watchdog_init(void)
{
  /* The PIC18F watchdog is disbled at boot-up. */
  stopped = 0;
  watchdog_stop();
}

/**
 * @brief Start the watchdog.
 */
void watchdog_start(void)
{
  stopped--;
  if (!stopped)
    WDTCON |= _SWDTEN;
}

/**
 * @fn void watchdog_periodic(void)
 * @brief Kick the dog.
 */
void watchdog_periodic(void)
{
  if (!stopped)
    ClrWdt();
}

/**
 * @fn void watchdog_stop(void)
 * @brief Stop the watchdog.
 */
void watchdog_stop(void)
{
  WDTCON = 0;
  ++stopped;
}

/**
 * @fn void watchdog_reboot(void)
 * @brief Reboot.
 */
void watchdog_reboot(void)
{
  watchdog_start();
  while (1) {
  };
}
