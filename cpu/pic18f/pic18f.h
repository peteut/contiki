// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief PIC18F definitions.
 */
#ifndef __PIC18_H__
#define __PIC18_H__

#include "contiki-conf.h"

#ifdef F_CPU
#define PIC18F_CPU_SPEED F_CPU
#else
#error 'F_CPU' not defined!
#endif

#endif /* __PIC18_H__ */
