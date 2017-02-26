TARGETS = partitions test stm32_proj

CROSS_GCC = arm-none-eabi-gcc
GCC=gcc

CSRC += partitions.c

.PHONY: all $(TARGETS)

all: $(TARGETS)

partitions: partitions.o
partitions.o: core_cm3.c flash.c
	$(GCC) $(CFLAGS) -o $@ -c partitions.c

stm32_proj:
	@make -C example_project

distclean:
	rm -f *.o
