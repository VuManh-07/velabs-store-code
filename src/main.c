#include <stdint.h>
#include "../../common/stm32f4xx.h"
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

/* ── TODO: Sinh viên hoàn thành các hàm dưới đây ────────────────────────── */

void gpio_init(void) {
    /* TODO: Bật clock GPIOD (RCC_AHB1ENR bit 3) */

    /* TODO: Cấu hình PD12, PD13, PD14, PD15 là output (MODER = 01) */

    /* TODO: Bật clock GPIOA (RCC_AHB1ENR bit 0) */

    /* TODO: Cấu hình PA2 là AF mode (MODER = 10) cho USART2 */
    /* TODO: Set PA2 AFR = AF7 (USART2) */
}

void led_set(uint8_t pin, uint8_t state) {
    /* TODO: Nếu state != 0, set bit 'pin' trong GPIOD->ODR
             Nếu state == 0, clear bit 'pin' trong GPIOD->ODR */
    (void)pin; (void)state;
}

void led_toggle(uint8_t pin) {
    /* TODO: XOR bit 'pin' trong GPIOD->ODR */
    (void)pin;
}

/* ── Test cases ──────────────────────────────────────────────────────────── */

static void tc1_led_set_on(void) {
    /* Bật LED PD12, kiểm tra bit 12 trong ODR */
    led_set(12, 1);
    ASSERT("TC1", READ_BIT(GPIOD->ODR, (1U << 12)) != 0);
}

static void tc2_led_toggle(void) {
    /* Toggle PD13 2 lần → phải về trạng thái ban đầu (off) */
    GPIOD->ODR &= ~(1U << 13);   /* đảm bảo ban đầu là off */
    led_toggle(13);               /* on */
    led_toggle(13);               /* off */
    ASSERT("TC2", READ_BIT(GPIOD->ODR, (1U << 13)) == 0);
}

static void tc3_all_leds_on(void) {
    /* Bật tất cả 4 LED (PD12–PD15) */
    led_set(12, 1); led_set(13, 1);
    led_set(14, 1); led_set(15, 1);
    uint32_t mask = (1U<<12)|(1U<<13)|(1U<<14)|(1U<<15);
    ASSERT("TC3", (GPIOD->ODR & mask) == mask);
}

static void tc4_all_leds_off(void) {
    /* Tắt tất cả 4 LED */
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
