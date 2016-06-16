# Makefile

PROJECT = sample

### This is a CAN example for Nucleo STM32F446

LDSCRIPT= mcu/stm32/stm32f446/ef.ld
CXXSRC = \
	sys.cxx \
	rcc.cxx \
	gpio.cxx \
	systimer.cxx \
	usart.cxx \
	irq.cxx \
	heap.cxx \
	context.cxx \
	ef.cxx \
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

INCDIR += .
BUILDDIR = build

CXXOBJS = $(addprefix $(BUILDDIR)/, $(notdir $(CXXSRC:.cxx=.o)))
CPPOBJS = $(addprefix $(BUILDDIR)/, $(notdir $(CPPSRC:.cpp=.o)))
COBJS = $(addprefix $(BUILDDIR)/, $(notdir $(CSRC:.c=.o)))
OBJS = $(COBJS) $(CXXOBJS) $(CPPOBJS)

OUTFILES = $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).bin
ifneq ($(ENABLE_OUTPUT_HEX),)
OUTFILES += $(BUILDDIR)/$(PROJECT).hex
endif

OPT += -ffunction-sections -fdata-sections -fno-common

IINCDIR   = $(patsubst %,-I%,$(INCDIR))
LLIBDIR   = $(patsubst %,-L%,$(LIBDIR))

vpath %.cxx $(dir $(CXXSRC))
vpath %.cpp $(dir $(CPPSRC))
vpath %.c $(dir $(CSRC))

MCFLAGS   = -mcpu=$(MCU)

CFLAGS    = $(MCFLAGS) $(OPT) $(CWARN) -Wa,-alms=$(BUILDDIR)/$(notdir $(<:.c=.lst)) $(DEFS)

LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch $(LLIBDIR)

CFLAGS   += -mthumb -mno-thumb-interwork -DTHUMB
LDFLAGS  += -mthumb -mno-thumb-interwork

CFLAGS   += -MD -MP -MF .dep/$(@F).d
CXXFLAGS += $(CFLAGS) -std=c++11

all: $(OUTFILES)

$(OBJS): | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)


$(CXXOBJS) : $(BUILDDIR)/%.o : %.cxx Makefile
	@echo
	$(CXX) -c $(CXXFLAGS) -I. $(IINCDIR) $< -o $@

$(CPPOBJS) : $(BUILDDIR)/%.o : %.cpp Makefile
	@echo
	$(CXX) -c $(CXXFLAGS) -I. $(IINCDIR) $< -o $@

$(COBJS) : $(BUILDDIR)/%.o : %.c Makefile
	@echo
	$(CC) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@

%.elf: $(OBJS) $(LDSCRIPT)
	@echo
	$(LD) $(COBJS) $(CXXOBJS) $(CPPOBJS) $(LDFLAGS) $(LIBS) -o $@

%.bin: %.elf $(LDSCRIPT)
	$(OBJCOPY) -O binary $< $@

%.hex: %.elf $(LDSCRIPT)
	$(OBJCOPY) -O ihex $< $@

clean:
	-rm -f -r .dep $(BUILDDIR)

# Include dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

distclean: clean
