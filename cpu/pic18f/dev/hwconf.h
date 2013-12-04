// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef __HWCONF_H__
#define __HWCONF_H__

#include "sys/cc.h"

#include <io.h>

#define ANSEL_REG(port, bit) ANS##port##bit##_REG
#define ANSEL_BIT(port, bit) ANS##port##bit##_POS

#define ANSB0_REG ANSELH
#define ANSB1_REG ANSELH
#define ANSB2_REG ANSELH
#define ANSB3_REG ANSELH
#define ANSB4_REG ANSELH

#define ANSA0_REG ANSEL
#define ANSA1_REG ANSEL
#define ANSA2_REG ANSEL
#define ANSA3_REG ANSEL
#define ANSA5_REG ANSEL
#define ANSA0_REG ANSEL
#define ANSE1_REG ANSEL
#define ANSE2_REG ANSEL

#define ANSB0_POS 4
#define ANSB1_POS 2
#define ANSB2_POS 0
#define ANSB3_POS 1
#define ANSB4_POS 3

#define ANSA0_POS 0
#define ANSA1_POS 1
#define ANSA2_POS 2
#define ANSA3_POS 3
#define ANSA5_POS 4
#define ANSE0_POS 5
#define ANSE1_POS 6
#define ANSE2_POS 7


#define HWCONF_PIN(name, port, bit) \
static inline void name##_select_io(void) {ANSEL_REG(port, bit) &= ~(1 << ANSEL_BIT(port, bit));} \
static inline void name##_set(void) {PORT##port |= 1 << bit;} \
static inline void name##_clear(void) {PORT##port &= ~(1 << bit);} \
static inline uint8_t name##_read(void) {return (PORT##port & (1 << bit));} \
static inline void name##_make_output(void) {TRIS##port &= ~(1 << bit);} \
static inline void name##_make_input(void) {TRIS##port |= 1 << bit;} \
static inline void name##_toggle(void) {__asm btg _##LAT##port, bit, 0 __endasm;}

#define HWCONF_PIN_NO_MX(name, port, bit) \
static inline void name##_set(void) {PORT##port |= 1 << bit;} \
static inline void name##_clear(void) {PORT##port &= ~(1 << bit);} \
static inline uint8_t name##_read(void) {return (PORT##port & (1 << bit));} \
static inline void name##_make_output(void) {TRIS##port &= ~(1 << bit);} \
static inline void name##_make_input(void) {TRIS##port |= 1 << bit;} \
static inline void name##_toggle(void) {__asm btg _##LAT##port, bit, 0 __endasm;}

#define INT_REG(port, bit) R##port##bit##I_REG
#define INT_IE(port, bit) R##port##bit##IE_POS
#define INT_IF(port, bit) R##port##bit##IF_POS
#define INT_ED_REG(port, bit) R##port##bit##ED_REG
#define INT_ED_POS(port, bit) R##port##bit##ED_POS

#define RB0I_REG INTCON
#define RB1I_REG INTCON3
#define RB2I_REG INTCON3
#define RB0IE_POS 4
#define RB0IF_POS 1
#define RB1IE_POS 3
#define RB1IF_POS 0
#define RB2IE_POS 4
#define RB2IF_POS 1

#define RB0ED_REG INTCON2
#define RB1ED_REG INTCON2
#define RB2ED_REG INTCON2
#define RB0ED_POS 6
#define RB1ED_POS 5
#define RB2ED_POS 4

#define HWCONF_IRQ(name, port, bit) \
static inline void name##_enable_irq(void) {INT_REG(port, bit) |= 1 << INT_IE(port, bit);} \
static inline void name##_disable_irq(void) {INT_REG(port, bit) &= ~(1 << INT_IE(port, bit));} \
static inline uint8_t name##_irq_enabled(void) {return INT_REG(port, bit) & (1 << INT_IE(port, bit));} \
static inline void name##_irq_edge_selectd(void) {INT_ED_REG(port, bit) &= ~(1 << INT_ED_POS(port, bit));} \
static inline void name##_irq_edge_selectu(void) {INT_ED_REG(port, bit) |= 1 << INT_ED_POS(port, bit);}\
static inline uint8_t name##_check_irq(void) {return INT_REG(port, bit) & (1 << INT_IF(port, bit));} \
static inline void name##_clear_flag(void) {INT_REG(port, bit) &= ~(1 << INT_IF(port, bit));} \
static inline uint8_t name##_irq_port(void) {return PORT##port;}

#endif /* __HWCONF_H__ */
