#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "uart.h"

#define USER_USART_BAUD 115200
#define USER_USART_WORD_LEN 8
#define USER_USART_PARITY USART_Parity_No

static void uart_hw_init(USART_TypeDef *uart, uint32_t baud, uint16_t wordlen, uint16_t parity) {
	uint16_t wordlen_tmp;
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef USART_InitStructure;

	if (wordlen == 8)
		wordlen_tmp = USART_WordLength_8b;
	if (wordlen == 9)
		wordlen_tmp = USART_WordLength_9b;

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = wordlen_tmp;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = parity;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(uart);

	if (uart == USART1) {
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

		gpio.GPIO_Pin = GPIO_Pin_10;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		gpio.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &gpio);

		gpio.GPIO_Pin = GPIO_Pin_9;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		gpio.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &gpio);

		RCC->APB2ENR |= RCC_APB2Periph_USART1;
		NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0x05, 0));
		NVIC_EnableIRQ(USART1_IRQn);
		USART_Init(USART1, &USART_InitStructure);
		USART_Cmd(USART1, ENABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	}

	if (uart == USART2) {
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

		gpio.GPIO_Pin = GPIO_Pin_3;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		gpio.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &gpio);

		gpio.GPIO_Pin = GPIO_Pin_2;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		gpio.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &gpio);

		NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0x05, 0));
		NVIC_EnableIRQ(USART2_IRQn);
		USART_Init(USART2, &USART_InitStructure);
		USART_Cmd(USART2, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);


	}

    	if (uart == USART3) {
        	RCC->APB1ENR |= RCC_APB1Periph_USART3;
        	NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0x05, 0));
        	NVIC_EnableIRQ(USART3_IRQn);
        	USART_Init(USART3, &USART_InitStructure);
        	USART_Cmd(USART3, ENABLE);
    	}
}

void uart_init(void) {
	uart_hw_init(USER_USART, USER_USART_BAUD, USER_USART_WORD_LEN, USER_USART_PARITY);
}


void putchar(uint8_t c) {
	while (!(USER_USART->SR & USART_FLAG_TXE) && 
		!(USER_USART->SR & USART_FLAG_TC));
	USER_USART->DR = (uint16_t)c;
}

void putc(void* p, char c) {
	(void)p;
	putchar(c);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
    }
}

void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
        USART_ClearITPendingBit(USART2, USART_IT_TXE);
    }
}

void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_TXE);
    }
}
	
