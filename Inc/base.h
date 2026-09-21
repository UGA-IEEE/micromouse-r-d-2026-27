#ifndef BASE_H
#define BASE_H

/*                             See STM32F103x8 Block Diagram & Memory Mapping                     */

// Peripheral Address & APB Bus Offsets
#define PERIPHERAL_BASE   (0x40000000)
#define APB1_OFFSET       (0x0000)
#define APB2_OFFSET       (0x00010000)

// APB Buses
#define APB1_BUS          (PERIPHERAL_BASE + APB1_OFFSET) // 0x4000 0000
#define APB2_BUS          (PERIPHERAL_BASE + APB2_OFFSET) // 0x4001 0000


// RCC Register Addresses & Offset
#define RCC_OFFSET        (0x00011000)
#define RCC               (APB2_BUS + RCC_OFFSET)

#define R_REGISTER_OFFSET (0x04)
#define RCC_CR            (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 0)
#define RCC_CFGR          (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 1)
#define RCC_CIR           (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 2)
#define RCC_APB2RSTR      (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 3)
#define RCC_APB1RSTR      (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 4)
#define RCC_APB2ENR       (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 5)
#define RCC_APB1ENR       (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 6)
#define RCC_BDCR          (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 7)
#define RCC_CSR           (volatile uint32_t *)(RCC + R_REGISTER_OFFSET * 8)


// ADC Addresses & Offsets
#define ADC1_OFFSET       (0x00002400)
#define ADC1_PERIPHERAL   (APB2_BUS + ADC1_OFFSET)

#define ADC2_OFFSET       (0x00002800)
#define ADC2_PERIPHERAL   (APB2_BUS + ADC2_OFFSET)

// USART Addresses & Offsets 
#define USART1_OFFSET     (0x00003800)
#define USART1_PERIPHERAL (APB2_BUS + USART1_OFFSET)

#define USART2_OFFSET     (0x00004400)
#define USART2_PERIPHERAL (APB1_BUS + USART2_OFFSET)

#define USART3_OFFSET     (0x00004800)
#define USART3_PERIPHERAL (APB1_BUS + USART3_OFFSET)

// GPIO Port Addresses
#define PORT_OFFSET       (0x400)
#define PORT_A_PERIPHERAL (APB2_BUS + (PORT_OFFSET * 2))

#define PORT_B_PERIPHERAL (APB2_BUS + (PORT_OFFSET * 3))

#define PORT_C_PERIPHERAL (APB2_BUS + (PORT_OFFSET * 4))

#define PORT_D_PERIPHERAL (APB2_BUS + (PORT_OFFSET * 5))


// DMA Address & Offset
#define DMA_OFFSET        (0x00010000)
#define DMA_PERIPHERAL    (APB2_BUS + DMA_OFFSET)



/*                                    R&D TASKS                                                   */
// TIM Address & Offset

// I2C Address & Offset

// EXTI Address & Offset

// AFIO Address & Offset

// RTC Address & Offset

#endif