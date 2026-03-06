#include "uart.h"
#include "stm32f4xx.h"

/* USART2: PA2=TX, PA3=RX — 115200 baud @ 16 MHz (HSI) */
#define APB1_CLOCK_HZ   16000000UL
#define BAUD_RATE       115200UL
#define BRR_VALUE       ((APB1_CLOCK_HZ + BAUD_RATE/2) / BAUD_RATE)

void uart_init(void) {
    /* Enable clocks */
    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    SET_BIT(RCC_APB1ENR, RCC_APB1ENR_USART2EN);

    /* PA2: AF7 (USART2 TX) */
    GPIOA->MODER  &= ~(0x3U << (2*2));
    GPIOA->MODER  |=  (GPIO_MODER_AF << (2*2));
    GPIOA->AFR[0] &= ~(0xFU << (4*2));
    GPIOA->AFR[0] |=  (7U   << (4*2));   /* AF7 = USART2 */

    /* Configure USART2 */
    USART2->BRR = BRR_VALUE;
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart_putc(char c) {
    while (!READ_BIT(USART2->SR, USART_SR_TXE)) {}
    USART2->DR = (uint8_t)c;
}

void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}

void uart_print_int(int n) {
    if (n < 0) { uart_putc('-'); n = -n; }
    if (n >= 10) uart_print_int(n / 10);
    uart_putc('0' + (n % 10));
}
