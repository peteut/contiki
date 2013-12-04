#ifndef JTAG
#  JTAG := $(CONTIKI)/platform/$(TARGET)/buildscripts/jtag/pyjtag/jtag.py
#endif

ifndef PK2CMD
    PK2CMD := pk2cmd
endif

prog: %.hex
	$(PK2CMD) -MPIC -P -YPIC -F$<

r:
	$(PK2CMD) -P -R

erase:
	$(PK2CMD) -P -E

