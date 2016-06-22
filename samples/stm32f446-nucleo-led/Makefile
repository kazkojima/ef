# Makefile

PROJECT = sample

### This is a LED example for Nucleo STM32F446

EFROOT=../..

LDSCRIPT= $(EFROOT)/mcu/stm32/stm32f446/ef.ld
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

MCU   = cortex-m4
CWARN = -Wall -Wextra
DEFS  = -DFREE_STANDING -DMHZ=180 -DMCU_STM32F446 -DUSE_FPU_AUTO_SAVE
OPT   = -O3 -Os -g -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffast-math
LIBS  =

include $(EFROOT)/rules.mk

distclean: clean
