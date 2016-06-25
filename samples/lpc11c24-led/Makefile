# Makefile

PROJECT = sample

### This is a LED example for LPC11C24

EFROOT=../..

LDSCRIPT= $(EFROOT)/mcu/lpc11xx/lpc11c24/ef.ld
CXXSRC = \
	$(EFROOT)/sys.cxx \
	$(EFROOT)/rcc.cxx \
	$(EFROOT)/gpio.cxx \
	$(EFROOT)/systimer.cxx \
	$(EFROOT)/usart.cxx \
	$(EFROOT)/irq.cxx \
	$(EFROOT)/heap.cxx \
	$(EFROOT)/context.cxx \
	$(EFROOT)/ef.cxx \
	efmain.cxx \

###################################
CROSS = arm-none-eabi-
CC   = $(CROSS)gcc
CXX  = $(CROSS)g++
LD   = $(CROSS)g++
OBJCOPY   = $(CROSS)objcopy

MCU   = cortex-m0
CWARN = -Wall -Wextra
DEFS  = -DFREE_STANDING -DMHZ=48 -DMCU_LPC11C24
OPT   = -O3 -Os -g
LIBS  =

# -lgcc-m0 was a workaround for temporal libgcc problem of old toolchain
# EXTRA_LDFLAGS=-lgcc-m0

include $(EFROOT)/rules.mk

distclean: clean
