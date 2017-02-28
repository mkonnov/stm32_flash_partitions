#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
			{"bootloader", 0x08000000, 20 * 1024, NO_ERASE_ON_CREATION},
			{"firmware",   0x0800f800, 40 * 1024, ERASE_ON_CREATION},
			{"settings", 0x0803f800, 2 * 1024, NO_ERASE_ON_CREATION},
			{NULL, NULL, NULL, NULL}
		}
	};
	
	partitions_register_callbacks(&cb);
	partition_table_init(&prtns);

	prtn_desc_t *fw_pd = partition_get_by_name("firmware");
	prtn_desc_t *bl_pd = partition_get_by_name("bootloader");
	
	if (!partition_is_empty("firmware")) {
		printf("fw partition is not empty\n");
		partition_erase("firmware");
	}

	printf("copying bootloader to firmware...\n");
	partition_copy("firmware", "bootloader");

	printf("checking memory...\n");
	if (memcmp(fw_pd->origin_addr, bl_pd->origin_addr, bl_pd->size) == 0)
		printf("OK: memory match\n");
	else
		printf("ERR: memory mismatch\n");

	for(;;);
	return 0;
}

