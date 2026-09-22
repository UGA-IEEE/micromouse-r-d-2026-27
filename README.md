# Blue Pill Bare Metal Blink Led

## What the program does

`src/main.c` blinks the onboard LED on **PC13**.

1. Turn on the clock for **GPIOC** (without a clock, the GPIO block is dead).
2. Configure **PC13** as a push-pull output.
3. Loop forever: wait a bit, then toggle PC13 with XOR.

On most Blue Pills the LED is **active-low** (writing `1` turns it off, `0` turns it on). That is why setup sets the pin high first, then toggles.

```text
  Reset
    │
    ▼
  ledSetup()
    ├─ RCC_APB2ENR |= IOPCEN     enable GPIOC clock
    ├─ GPIOC->CRH  configure PC13 as output
    └─ GPIOC->ODR  |= LED_PIN    LED off (active low)
    │
    ▼
  while (1)
    ├─ busy-wait delay
    └─ GPIOC->ODR ^= LED_PIN     toggle LED
```

## Docs you need

ST publishes two main documents. Use both:

| Document | What it is for |
|---|---|
| **Datasheet** (DS5319 for STM32F103x8) | Pinout, package, memory size, which pins have which AF |
| **Reference Manual** (RM0008) | Register maps, bit fields, how RCC / GPIO / UART / … work |

For this blink, almost everything comes from **RM0008**.

In the PDF, search for the register names like `APB2ENR` and `GPIOx_CRH`.

## How to find peripherals in the reference manual

Every peripheral is a block of memory-mapped registers. The CPU talks to hardware by reading/writing those addresses.

### 1. Find the memory map

In **RM0008**, open the **memory map** chapter (often titled *Memory and bus architecture*).

You should find something like:

| Region | Base address |
|---|---|
| Peripheral bus | `0x4000_0000` |
| APB2 peripherals | `0x4001_0000` |
| GPIOC | `0x4001_1000` |
| RCC | `0x4002_1000` |

That is where these `#define`s come from:

```c
#define PERIPH_BASE     0x40000000U
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000U)  // 0x40010000
#define GPIOC_BASE      (APB2PERIPH_BASE + 0x1000U) // 0x40011000
#define RCC             (PERIPH_BASE + 0x21000U)    // 0x40021000
```

### 2. Find the clock-enable bit (RCC)

GPIO does nothing until its clock is enabled in **RCC**.

1. Go to the **RCC** chapter.
2. Find register **`RCC_APB2ENR`** (APB2 peripheral clock enable).
3. Note:
   - Offset from RCC base (for F103 this is `0x18`)
   - Bit that enables GPIOC — **`IOPCEN`**, bit **4**

So in code:

```c
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC + 0x18U))
#define GPIOC_CLOCK (1U << 4)

RCC_APB2ENR |= GPIOC_CLOCK;  // set bit 4 → clock on
```

`|=` sets the bit without clearing other enable bits.

### 3. Find the GPIO registers

1. Go to the **GPIO** chapter in RM0008.
2. Note the register layout for F1 (this family is **not** the same as L4 / F4):

| Offset | Register | Role |
|---|---|---|
| `0x00` | `CRL` | config for pins 0–7 |
| `0x04` | `CRH` | config for pins 8–15 |
| `0x08` | `IDR` | input data |
| `0x0C` | `ODR` | output data |
| `0x10` | `BSRR` | atomic set/reset |
| `0x14` | `BRR` | atomic reset |

That maps to the struct in `main.c`:

```c
typedef struct
{
    volatile uint32_t CRL;  // 0x00
    volatile uint32_t CRH;  // 0x04
    volatile uint32_t IDR;  // 0x08
    volatile uint32_t ODR;  // 0x0C
    volatile uint32_t BSRR; // 0x10
    volatile uint32_t BRR;  // 0x14
} GPIO_TypeDef;
```

`volatile` tells the compiler: this memory can change / has side effects - do not optimize the accesses away.

### 4. Configure PC13 as an output (`CRH`)

PC13 is pin **13**, so it lives in **`CRH`** (pins 8–15).

In the reference manual, each pin uses **4 bits** in `CRL`/`CRH`:

| Field | Bits (within the pin’s nibble) | Meaning |
|---|---|---|
| `MODE` | [1:0] | `00` = input, `01`/`10`/`11` = output speed |
| `CNF` | [3:2] | for output: `00` = push-pull, `01` = open-drain |

Pin 13’s nibble starts at bit **20** → bits **[23:20]**.

For a 2 MHz push-pull output: `MODE = 10`, `CNF = 00` → nibble value `0x2`.

```c
#define LED_OUTPUT (0x2U << 20)
#define LED_PIN    (1U << 13)

GPIOC->CRH &= ~(0xFU << 20); // clear bits [23:20]
GPIOC->CRH |= LED_OUTPUT;    // write MODE/CNF for PC13
GPIOC->ODR |= LED_PIN;       // drive pin high (LED off)
```

### 5. Toggle the LED (`ODR`)

`ODR` bit 13 is the output level of PC13.

```c
GPIOC->ODR ^= LED_PIN;  // flip bit 13 each loop
```

## Project layout

| File | Role |
|---|---|
| `src/main.c` | Application — RCC + GPIO blink |
| `startup_stm32f103xb.s` | Reset handler, vector table, `.data` / `.bss` init |
| `stm32f103c8.ld` | Linker script: 64K flash, 20K RAM |
| `Makefile` | Build + OpenOCD flash |

At reset, the CPU loads the stack pointer and `Reset_Handler` from flash, copies initialized data, zeros BSS, then calls `main`.

## Build and flash

**Tools:** `arm-none-eabi-gcc`, OpenOCD, ST-Link.

```bash
make clean && make   # build build/firmware.elf
make flash           # program via ST-Link
```

In VS Code (with **VSCode Task Buttons**):

- **Clean and Make** → `make clean && make`
- **Build and Upload** → `make && make flash`