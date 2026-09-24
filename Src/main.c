

#include <stdint.h>
#include "gpio.h"

int main(void)
{
    /* Loop forever */
    // Turns On onboard LED
    gpio_init(GPIOC, 13, OUTPUT_2MHZ, GENERAL_PURPOSE_PUSH_PULL);
    gpio_set(GPIOC, 13, LOW);
	for(;;);
}
