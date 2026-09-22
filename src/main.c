#include <stdint.h>

/* Base Addresses */
#define PERIPH_BASE     0x40000000U // + for address, | for mask
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000U)
#define GPIOC_BASE      (APB2PERIPH_BASE + 0x1000U)

/* RCC Register Addresses */
#define RCC             (PERIPH_BASE + 0x21000U)
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC + 0x18U)) // treat number as the address of a variable

/* GPIOC Peripheral Register Struct */
typedef struct
{
    volatile uint32_t CRL;  // 0x00 pins 0-7
    volatile uint32_t CRH;  // 0x04 pins 8-15
    volatile uint32_t IDR;  // 0x08
    volatile uint32_t ODR;  // 0x0C
    volatile uint32_t BSRR; // 0x10
    volatile uint32_t BRR;  // 0x14
} GPIO_TypeDef;

/* GPIOC */
#define GPIOC ((GPIO_TypeDef *)(GPIOC_BASE))
#define GPIOC_CLOCK (1U << 4)

/* LED Pin */
#define LED_OUTPUT  (0x2U << 20)
#define LED_PIN     (1U << 13)

void ledSetup(void)
{
    RCC_APB2ENR |= GPIOC_CLOCK;     // enables GPIOC clock

    GPIOC->CRH &= ~(0xFU << 20);    // clear CRH bits [23:20]
    GPIOC->CRH |= LED_OUTPUT;       // set PIN C13 to output
    GPIOC->ODR |= LED_PIN;          // sets PIN C13 to HIGH (LED off — active low)
}

int main(void)
{
    ledSetup();

    volatile uint32_t counter;

    while (1)
    {
        for (counter = 0; counter < 100000; counter ++); // delay
        GPIOC->ODR ^= LED_PIN;
    }

    return 0;
}