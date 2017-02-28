#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "uart.h"
#include "printf.h"
#include "partitions.h"

#include "flash.h"

static void init_rcc(void) {
	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | \
			RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | \
			RCC_APB2ENR_AFIOEN;

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB1ENR |=  RCC_APB1ENR_USART2EN;
}

int main(void) {
	SystemInit();
	init_rcc();

	uart_init();
	init_printf(0, putc);

	prtn_callbacks_t cb = {
		flash_write_block,
		flash_erase_page,
		flash_unlock,
		flash_lock,
		__disable_irq,
		__enable_irq
	};

	prtn_table_t prtns = {
		{2048, 0x08000000, 1024 * 256},
		{
			{"bootloader", 0x08000000, 62 * 1024},
			{"firmware",   0x0800f800, 40 * 1024},
			{"settings", 0xdeadbeef, 62 * 1024},
			{NULL, NULL, NULL}
		}
	};
	
	partitions_register_callbacks(&cb);
	partition_table_init(&prtns);
	
	int i = 0;
	while (prtns.partitions[i].origin_addr != NULL) {
		printf("----\n\r%s\n\r", prtns.partitions[i].name);
		printf("origin: %02X\r\n", prtns.partitions[i].origin_addr);
		printf("size: %d\r\n", prtns.partitions[i].size);
		i++;
	}

	for(;;);
	return 0;
}

