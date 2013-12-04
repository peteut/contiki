// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef __RTIMER_ARCH_H__
#define __RTIMER_ARCH_H__

#include "contiki-conf.h"
#include "sys/rtimer.h"
#include "io.h"

#define RTIMER_ARCH_SECOND (4096U)

#define rtimer_arch_now() (TMR3L | (TMR3H << 8))

#endif /* __RTIMER_ARCH_H__ */
