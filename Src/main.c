#include <stdint.h>
#include "gpio.h"

int main(void)
{
    /*
     * Enable both GPIO clocks.
     * Bit 2 enables GPIOA, used by the sensor.
     * Bit 4 enables GPIOC, used by the LED.
     */
    *RCC_APB2ENR |= (1U << 2) | (1U << 4);
    (void)*RCC_APB2ENR;

    /*
     * Configure PA0 as an input with a pull-up.
     *
     * For an input with pull-up/pull-down:
     * ODR bit = 1 selects pull-up.
     * CNF = 10, MODE = 00, giving configuration 1000.
     *
     * PA0 uses bits 3:0 of GPIOA_CRL.
     */
    GPIOA->BSRR = (1U << 0);
    GPIOA->CRL =
        (GPIOA->CRL & ~(0xFU << 0)) | (0x8U << 0);

    /* Preload PC13 HIGH so the active-low LED starts off. */
    GPIOC->BSRR = (1U << 13);

    /* Configure PC13 as a push-pull output, 2 MHz setting. */
    GPIOC->CRH =
        (GPIOC->CRH & ~(0xFU << 20)) | (0x2U << 20);

    for (;;)
    {
        /*
         * Read PA0.
         * This sensor is expected to output LOW on detection.
         */
        if ((GPIOA->IDR & (1U << 0)) == 0U)
        {
            /* Object detected: PC13 LOW turns LED on. */
            GPIOC->BSRR = (1U << (13U + 16U));
        }
        else
        {
            /* No object detected: PC13 HIGH turns LED off. */
            GPIOC->BSRR = (1U << 13);
        }
    }
}