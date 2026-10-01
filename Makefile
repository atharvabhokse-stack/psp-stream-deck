TARGET = psp_stream_deck
OBJS = main.c

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = PSP Stream Deck

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak