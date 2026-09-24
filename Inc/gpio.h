#ifndef GPIO_H
#define GPIO_H

#include "base.h"
#include <stdint.h>

/*                             See STM32103xx Reference Manual                                    */

#define GPIOA ((gpio_x *)(PORT_A_PERIPHERAL))
#define GPIOB ((gpio_x *)(PORT_B_PERIPHERAL))
#define GPIOC ((gpio_x *)(PORT_C_PERIPHERAL))
#define GPIOD ((gpio_x *)(PORT_D_PERIPHERAL))



typedef enum {
    LOW, HIGH
} gpio_level;


typedef struct {
    volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
} gpio_x;

typedef enum {
    INPUT, OUTPUT_10MHZ, OUTPUT_2MHZ, OUTPUT_50MHZ
} gpio_mode;

typedef enum {
    ANALOG, FLOATING_INPUT, INPUT_PUSH_PULL,
    GENERAL_PURPOSE_PUSH_PULL = 0, GENERAL_PURPOSE_OPEN_DRAIN,
    ALTERNATE_FUNCTION_PUSH_PULL, ALTERNATE_FUNCTION_OPEN_DRAIN 
} gpio_cnf;

// Set RCC
void gpio_init(gpio_x *gpio, uint8_t pinNumber, uint8_t mode, uint8_t config);

// 
void gpio_set(gpio_x *gpio, uint8_t pinNumber, gpio_level level);

// Reads IDR
gpio_level gpio_read(gpio_x *gpio, uint8_t pinNumber);







#endif