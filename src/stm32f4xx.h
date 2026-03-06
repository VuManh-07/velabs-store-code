#ifndef STM32F4XX_H
#define STM32F4XX_H

#include <stdint.h>

/* ── RCC ─────────────────────────────────────────────────────────────────── */
#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44))
#define RCC_AHB1ENR_GPIOAEN  (1U << 0)
#define RCC_AHB1ENR_GPIODEN  (1U << 3)
#define RCC_APB1ENR_USART2EN (1U << 17)
#define RCC_APB1ENR_TIM2EN   (1U << 0)
#define RCC_APB2ENR_USART1EN (1U << 4)

/* ── GPIO ────────────────────────────────────────────────────────────────── */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA   ((GPIO_TypeDef *) 0x40020000UL)
#define GPIOD   ((GPIO_TypeDef *) 0x40020C00UL)

#define GPIO_MODER_INPUT    0x0U
#define GPIO_MODER_OUTPUT   0x1U
#define GPIO_MODER_AF       0x2U

/* ── USART ───────────────────────────────────────────────────────────────── */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define USART1  ((USART_TypeDef *) 0x40011000UL)
#define USART2  ((USART_TypeDef *) 0x40004400UL)

#define USART_SR_RXNE   (1U << 5)
#define USART_SR_TXE    (1U << 7)
#define USART_SR_TC     (1U << 6)
#define USART_CR1_UE    (1U << 13)
#define USART_CR1_RE    (1U << 2)
#define USART_CR1_TE    (1U << 3)

/* ── TIM ─────────────────────────────────────────────────────────────────── */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} TIM_TypeDef;

#define TIM2    ((TIM_TypeDef *) 0x40000000UL)

#define TIM_CR1_CEN     (1U << 0)
#define TIM_DIER_UIE    (1U << 0)
#define TIM_SR_UIF      (1U << 0)

/* ── NVIC ────────────────────────────────────────────────────────────────── */
#define NVIC_ISER0  (*(volatile uint32_t *) 0xE000E100UL)
#define NVIC_ISER1  (*(volatile uint32_t *) 0xE000E104UL)

/* ── SysTick ─────────────────────────────────────────────────────────────── */
#define SYSTICK_BASE    0xE000E010UL
#define SYSTICK_CTRL    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL     (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

/* ── Helpers ─────────────────────────────────────────────────────────────── */
#define SET_BIT(REG, BIT)   ((REG) |=  (BIT))
#define CLR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)  ((REG) &   (BIT))

static inline void delay_ms(volatile uint32_t ms) {
    while (ms--) {
        volatile uint32_t i = 4000;
        while (i--);
    }
}

#endif /* STM32F4XX_H */
