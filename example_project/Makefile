BUILDDIR=build
DEPDIR = $(BUILDDIR)/.dep
TARGET=partitions
PWD = $(shell pwd)

V = $(if $(VERBOSE), , @)

CROSS = arm-none-eabi
GCC = $(CROSS)-gcc
AS = $(CROSS)-as
SIZE = $(CROSS)-size
OBJCOPY = $(CROSS)-objcopy
OBJDUMP = $(CROSS)-objdump
NM = $(CROSS)-nm

LDSCRIPT = stm32f107_hal/stm32_flash_0x08000000.ld
INCLUDES = -I. 
INCLUDES += -Istm32f107_hal 
INCLUDES += -Istm32f107_hal/stm32f10x_periph/inc

COMMONFLAGS = -mcpu=cortex-m3 -mthumb -ggdb3
CFLAGS += -Os
CFLAGS += $(COMMONFLAGS) $(INCLUDES) -I.
CFLAGS += -std=c99 -Wall -Wextra -static 
CFLAGS += -fdata-sections -ffunction-sections 
CFLAGS += -fno-hosted -fno-builtin
CFLAGS += -nostdlib -nodefaultlibs
CFLAGS += -mapcs-frame -msoft-float
CFLAGS += -Werror-implicit-function-declaration

CFLAGS += -MD -MP -MF $(DEPDIR)/$(@F).d
LDFLAGS = $(COMMONFLAGS) -static
LDFLAGS += -fno-exceptions 
LDFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -static -Xlinker --gc-sections
LDFLAGS += -Wl,-Map=$(BUILDDIR)/$(TARGET).map,--cref
ASFLAGS = $(COMMONFLAGS)

CFLAGS += -DSTM32F10X_CL
CFLAGS += -DUSE_STDPERIPH_DRIVER

CSRC = main.c
CSRC += stm32f107_hal/core_cm3.c
CSRC += stm32f107_hal/flash.c
CSRC += stm32f107_hal/startup_stm32f10x_cl.c
CSRC += stm32f107_hal/stm32f10x_it.c
CSRC += stm32f107_hal/system_stm32f10x.c
CSRC += stm32f107_hal/uart.c
CSRC += stm32f107_hal/stm32f10x_periph/src/stm32f10x_gpio.c
CSRC += stm32f107_hal/stm32f10x_periph/src/stm32f10x_usart.c
CSRC += printf/printf.c

COBJ = $(addprefix $(BUILDDIR)/, $(CSRC:.c=.c.o))
OBJ = $(COBJ)

all: $(BUILDDIR)/$(TARGET).bin
	@$(SIZE) $(BUILDDIR)/$(TARGET).elf
	@echo =======================================================================

$(BUILDDIR)/$(TARGET).bin: $(BUILDDIR)/$(TARGET).elf
	@$(OBJCOPY) -O binary $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).bin
	@$(OBJCOPY) -O ihex $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET).hex
	@$(OBJDUMP) -h -S -z $(BUILDDIR)/$(TARGET).elf > $(BUILDDIR)/$(TARGET).lss
	@$(NM) -n $(BUILDDIR)/$(TARGET).elf > $(BUILDDIR)/$(TARGET).sym

$(BUILDDIR)/$(TARGET).elf: $(OBJ)
	@echo Linking $@
	$(GCC) $(LDFLAGS) -T $(PWD)/$(LDSCRIPT) -o $@ $(OBJ)

$(COBJ): $(BUILDDIR)/%.c.o : %.c
	@echo Compiling $<
	@-mkdir -p $(@D)
	$(V)$(GCC) $(CFLAGS) -c $< -o $@

-include $(shell mkdir -p $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)

.PHONY: clean output

clean:
	rm -rf $(BUILDDIR)
