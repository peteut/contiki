// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_MSG_TYPE 1

struct button_msg {
  uint8_t type;
  uint8_t value;
};

void button_init(struct process* proc);

#endif /* BUTTON_H */
