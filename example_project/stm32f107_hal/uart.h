#ifndef UART_H
#define UART_H

#define USER_USART USART2

void uart_init(void);
void putc(void *p, char c);

 #endif /* UART_H */
