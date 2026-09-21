#include "gpio.h"
#include "base.h"
#include <stdint.h>

/* -- Useful Equation -- */
// bit extraction where P = starting bit position & N = # of bits to extract
// ((*gpio) >> 0) & ((1U << 16) - 1)


// Set RCC for GPIO peripheral & Modify CRL/CRH
void gpio_init(gpio_x *gpio, uint8_t pinNumber, uint8_t mode, uint8_t config) {
    if (pinNumber > 15) return;
    /* -- Initialize clock for GPIO port X -- */
    uint32_t bitToSet = (((uintptr_t)gpio) - APB2_BUS) / PORT_OFFSET;
    *RCC_APB2ENR |= (1U << bitToSet);

    /* -- Set MODE & Config for GPIO port X -- */
    uint8_t totalConfig = (config << 2U) | mode;

    /* -- Clear MODE & Config Bits then Set -- */
    if (pinNumber > 7) {
        gpio->CRH &= ~(((1U << 4) - 1) << ((pinNumber % 8U) * 4U));
        gpio->CRH |= (totalConfig << ((pinNumber % 8U) * 4U));
    } else {
        gpio->CRL &= ~(((1U << 4) - 1) << (pinNumber * 4U));
        gpio->CRL |= (totalConfig << (pinNumber * 4U));
    }
}

// Modifies BSRR
void gpio_set(gpio_x *gpio, uint8_t pinNumber, gpio_level level) {
    if (pinNumber > 15) return;

    switch (level) {
        case HIGH:
            gpio->BSRR = (uint32_t)(1U << pinNumber);
            break;

        case LOW:
            gpio->BSRR = (uint32_t)(1U << (pinNumber + 16));
            break;

        default:
            break;
    }
}

// Reads IDR
void gpio_read(gpio_x *gpio, uint8_t pinNumber) {

}


