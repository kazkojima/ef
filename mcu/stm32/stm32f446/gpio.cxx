// Copyright (C) 2016 kaz Kojima
//
// This file is part of ef runtime library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the EF Runtime Library Exception.

// You should have received a copy of the GNU General Public License and
// a copy of the EF Runtime Library Exception along with this program;
// see the files COPYING and EXCEPTION respectively.

/*
 * GPIO init
 */

/*
 * Port A setup.
 * PA2  - USART2_TX (AF7)
 * PA3  - USART2_RX (AF7)
 * PA5  - LED (LED 1:ON 0:OFF)
 * PA13, PA14 - SWD(AF0)
 * PA11 - CAN1_RX (AF9)
 * PA12 - CAN1_TX (AF9)
 * PA15 - I2S1_WS (AF5)
 */
#define GPIOA_MODER   0xaa8004a0 // AF Pin 15,14,13,12,11,3,2 Output Pin5
#define GPIOA_OTYPER  0x00000000 // Push-Pull
#define GPIOA_OSPEEDR 0x03c00c00 // High speed: Pin12,11,5
#define GPIOA_PUPDR   0x64155105 // Pin14 pull-down Pin13,12,5,2,1 floating
#define GPIOA_ODR     0x00000000
#define GPIOA_AFR0    0x00007700 // AF7 Pin3,2
#define GPIOA_AFR1    0x50099000 // AF9 Pin12,11

/*
 * Port B setup.
 * PB3  - I2S1_CK (AF5)
 * PB5  - I2S1_SD (AF5)
 * PB8  - I2C1_SCL(AF4)
 * PB9  - I2C1_SDA(AF4)
 * PB12 - CAN2_RX (AF9)
 * PB13 - CAN2_TX (AF9)
 */
#define GPIOB_MODER   0x0a0a0880 // AF9 Pin13,12 AF4 Pin9,8 AF5 Pin5,3
#define GPIOB_OTYPER  0x00000300 // Open-drain Pin9,8 otherwise Push-Pull
#define GPIOB_OSPEEDR 0x000f0cc0 // High speed: Pin9,8,5,3
#define GPIOB_PUPDR   0x55555555 // Pull-up
#define GPIOB_ODR     0x00000000
#define GPIOB_AFR0    0x00505000 // AF5 Pin5,3
#define GPIOB_AFR1    0x00990044 // AF9 Pin13,12 AF4 Pin9,8

/*
 * Port C setup.
 * PC13  - USER Button
 */
#define GPIOC_MODER   0x000000ff // Input Pin13 Analog input Pin3,2,1,0
#define GPIOC_OTYPER  0x00000000 // Push-Pull
#define GPIOC_OSPEEDR 0x00000000
#define GPIOC_PUPDR   0x01555555 // Input Pin15-13 floating
#define GPIOC_ODR     0x00000000
#define GPIOC_AFR0    0x00000000
#define GPIOC_AFR1    0x00000000

#define GPIO_LED GPIOA
#define GPIO_LED_SET_TO_EMIT    5

void
ef::mcu::gpio_init (void)
{
  /*
   * Enabling the CAN controllers, then configuring GPIO functions for
   * CAN_TX.  Order matters, otherwise the CAN_TX pins will twitch,
   * disturbing the CAN bus.
   */
  RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
  RCC->APB1RSTR = RCC_APB1RSTR_CAN1RST;
  RCC->APB1RSTR = 0;

  RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
  RCC->APB1RSTR = RCC_APB1RSTR_CAN2RST;
  RCC->APB1RSTR = 0;

  // Then enable GPIO clock.
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN
		   | RCC_AHB1ENR_GPIOBEN
		   | RCC_AHB1ENR_GPIOCEN);
  RCC->AHB1RSTR = (RCC_AHB1RSTR_GPIOARST
		   | RCC_AHB1RSTR_GPIOBRST
		   | RCC_AHB1RSTR_GPIOCRST);
  RCC->AHB1RSTR = 0;

  GPIOA->AFR[0]  = GPIOA_AFR0;
  GPIOA->AFR[1]  = GPIOA_AFR1;
  GPIOA->OSPEEDR = GPIOA_OSPEEDR;
  GPIOA->OTYPER  = GPIOA_OTYPER;
  GPIOA->ODR     = GPIOA_ODR;
  GPIOA->MODER   = GPIOA_MODER;
  GPIOA->PUPDR   = GPIOA_PUPDR;

  GPIOB->AFR[0]  = GPIOB_AFR0;
  GPIOB->AFR[1]  = GPIOB_AFR1;
  GPIOB->OSPEEDR = GPIOB_OSPEEDR;
  GPIOB->OTYPER  = GPIOB_OTYPER;
  GPIOB->ODR     = GPIOA_ODR;
  GPIOB->MODER   = GPIOB_MODER;
  GPIOB->PUPDR   = GPIOB_PUPDR;

  GPIOC->AFR[0]  = GPIOC_AFR0;
  GPIOC->AFR[1]  = GPIOC_AFR1;
  GPIOC->OSPEEDR = GPIOC_OSPEEDR;
  GPIOC->OTYPER  = GPIOC_OTYPER;
  GPIOC->ODR     = GPIOA_ODR;
  GPIOC->MODER   = GPIOC_MODER;
  GPIOC->PUPDR   = GPIOC_PUPDR;
}

void
ef::board::set_led (bool on)
{
  if (on)
    GPIO_LED->BSRR = (1 << GPIO_LED_SET_TO_EMIT);
  else
    GPIO_LED->BSRR = (1 << GPIO_LED_SET_TO_EMIT) << 16;
}
