// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
/**
 * @file
 * @brief Platform dependent m
 */
#ifndef __MTARCH_H__
#define __MTARCH_H__

#include "pic18fdef.h"

enum { MTARCH_STACKSIZE = 128 };

struct mtarch_thread {
  uint8_t stack[MTARCH_STACKSIZE];
  uint8_t* sp;
};

struct mt_thread;
int mtarch_stack_usage(struct mt_thread *t);

#endif /* __MTARCH_H__ */

