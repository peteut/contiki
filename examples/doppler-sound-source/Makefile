CONTIKI_PROJECT = doppler-sound-source
CONTIKI = ../..
ifndef TARGET
TARGET = wp0710b
endif

## some debug macros (comment them out if not used)
DEBUG_MACROS = ## DEBUG_TOGGLE_TICK
DEBUG_MACROS += ## DEBUG_IDLE_MODE

CFLAGS = -DTMR2_HANDLER=tmr2_tick $(addsuffix =1, $(addprefix -D, $(DEBUG_MACROS)))

all: $(CONTIKI_PROJECT)
include $(CONTIKI)/Makefile.include
