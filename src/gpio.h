#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init(void);
void led_set(uint8_t pin, uint8_t state);
void led_toggle(uint8_t pin);

#endif /* GPIO_H */
