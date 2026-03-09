#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "gpio.h"

/* ── Test framework ──────────────────────────────────────────────────────── */
static int test_pass = 0;
static int test_fail = 0;

#define ASSERT(name, cond) \
    do { \
        if (cond) { uart_puts(name ":PASS\r\n"); test_pass++; } \
        else       { uart_puts(name ":FAIL\r\n"); test_fail++; } \
    } while(0)

/* ── Implementation ──────────────────────────────────────────────────────── */

void gpio_init(void) {
    /* Bật clock GPIOD */
    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_GPIODEN);

    /* PD12–PD15: output mode (MODER = 01) */
    GPIOD->MODER &= ~((0x3U << (12*2)) | (0x3U << (13*2)) |
                      (0x3U << (14*2)) | (0x3U << (15*2)));
    GPIOD->MODER |=  ((GPIO_MODER_OUTPUT << (12*2)) |
                      (GPIO_MODER_OUTPUT << (13*2)) |
                      (GPIO_MODER_OUTPUT << (14*2)) |
                      (GPIO_MODER_OUTPUT << (15*2)));

    /* Bật clock GPIOA */
    SET_BIT(RCC_AHB1ENR, RCC_AHB1ENR_GPIOAEN);

    /* PA2: AF mode cho USART2 TX */
    GPIOA->MODER &= ~(0x3U << (2*2));
    GPIOA->MODER |=  (GPIO_MODER_AF << (2*2));
    GPIOA->AFR[0] &= ~(0xFU << (4*2));
    GPIOA->AFR[0] |=  (7U   << (4*2));  /* AF7 = USART2 */
}

void led_set(uint8_t pin, uint8_t state) {
    if (state) {
        GPIOD->ODR |=  (1U << pin);
    } else {
        GPIOD->ODR &= ~(1U << pin);
    }
}

void led_toggle(uint8_t pin) {
    GPIOD->ODR ^= (1U << pin);
}

/* ── Test cases ──────────────────────────────────────────────────────────── */

static void tc1_led_set_on(void) {
    led_set(12, 1);
    ASSERT("TC1", READ_BIT(GPIOD->ODR, (1U << 12)) != 0);
}

static void tc2_led_toggle(void) {
    GPIOD->ODR &= ~(1U << 13);
    led_toggle(13);
    led_toggle(13);
    ASSERT("TC2", READ_BIT(GPIOD->ODR, (1U << 13)) == 0);
}

static void tc3_all_leds_on(void) {
    led_set(12, 1); led_set(13, 1);
    led_set(14, 1); led_set(15, 1);
    uint32_t mask = (1U<<12)|(1U<<13)|(1U<<14)|(1U<<15);
    ASSERT("TC3", (GPIOD->ODR & mask) == mask);
}

static void tc4_all_leds_off(void) {
    led_set(12, 0); led_set(13, 0);
    led_set(14, 0); led_set(15, 0);
    uint32_t mask = (1U<<12)|(1U<<13)|(1U<<14)|(1U<<15);
    ASSERT("TC4", (GPIOD->ODR & mask) == 0);
}

/* ── main ────────────────────────────────────────────────────────────────── */
int main(void) {
    gpio_init();
    uart_init();

    uart_puts("=== LAB01 GPIO TEST ===\r\n");

    tc1_led_set_on();
    tc2_led_toggle();
    tc3_all_leds_on();
    tc4_all_leds_off();

    uart_puts("=== SUMMARY ===\r\n");
    uart_puts("PASS:");
    uart_print_int(test_pass);
    uart_puts(" FAIL:");
    uart_print_int(test_fail);
    uart_puts("\r\n");

    if (test_fail == 0) {
        uart_puts("TEST PASSED\r\n");
    } else {
        uart_puts("TEST FAILED\r\n");
    }

    while (1) {}
}
