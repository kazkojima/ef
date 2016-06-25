# Makefile

PROJECT = sample

### This is a LED example for Nucleo STM32F746

EFROOT=../..

LDSCRIPT= $(EFROOT)/mcu/stm32/stm32f746/ef.ld
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
	i2s.cxx \
	efmain.cxx

CSRC = \
	fftsg.c

###################################
CROSS = arm-none-eabi-
CC   = $(CROSS)gcc
CXX  = $(CROSS)g++
LD   = $(CROSS)g++
OBJCOPY   = $(CROSS)objcopy

MCU   = cortex-m7
CWARN = -Wall -Wextra
DEFS  = -DFREE_STANDING -DMHZ=216 -DMCU_STM32F746 -DUSE_FPU_AUTO_SAVE
OPT   = -O3 -Os -g -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffast-math
LIBS  =

include $(EFROOT)/rules.mk

distclean: clean
