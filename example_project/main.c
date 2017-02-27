#include <stdint.h>
#include <stdbool.h>

#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include "stm32f10x_gpio.h"

#include "uart.h"
#include "printf.h"

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

	printf("Hello\r\n");

	for(;;);
	return 0;
}

