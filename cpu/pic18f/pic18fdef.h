// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef PIC18FDEF_H
#define PIC18FDEF_H

#include <stdint.h>

/* Compiler configurations */
#define CCIF
#define CLIF
#define CC_CONF_CONST_FUNCTION_BUG

#define CC_CONF_ASSIGN_AGGREGATE(dest, src) \
    memcpy(dest, src, sizeof(*dest))

#define uip_ipaddr_copy(dest, src) \
    memcpy(dest, src, sizeof(*dest))

/* ensure that memcpy is available */
#include <string.h>

void pic18f_cpu_init(void); /* Rename to cpu_init() later! */
#define cpu_init() pic18f_cpu_init()

typedef unsigned char spl_t;
#define splhigh() splhigh_()
#define splx(sr) splx_()

#define READ_AND_DISCARD(r) do { __asm movf _##r __endasm; } while (0)
#define di() do { INTCON &= ~(_GIEH | _GIEL); } while (0)
#define ei() do { INTCON |= (_GIEH | _GIEL ); } while (0)

#endif /* PIC18FDEF_H */
