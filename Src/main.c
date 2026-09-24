

#include <stdint.h>
#include "gpio.h"

int main(void)
{
    /* Loop forever */

    // Turns PC13 LED Off then On
    gpio_init(GPIOC, 13, OUTPUT_2MHZ, GENERAL_PURPOSE_PUSH_PULL);
    gpio_set(GPIOC, 13, GPIO_HIGH);

    gpio_level whatLevel = gpio_read(GPIOC, 13);
    if (whatLevel == GPIO_HIGH) {
        gpio_set(GPIOC, 13, GPIO_LOW);
    }
	for(;;);
}
