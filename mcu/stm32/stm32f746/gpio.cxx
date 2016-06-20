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
 * PA13, PA14 - SWD(AF0)
 * PA15 - I2S1_WS (AF5)
 */
#define GPIOA_MODER   0xa8000000 // AF Pin 15,14,13
#define GPIOA_OTYPER  0x00000000 // Push-Pull
#define GPIOA_OSPEEDR 0x00000000 // High speed: Pin12,11
#define GPIOA_PUPDR   0x64155555 // Pin14 pull-down Pin13,12 floating
#define GPIOA_ODR     0x00000000
#define GPIOA_AFR0    0x00000000
#define GPIOA_AFR1    0x50000000 // AF5 Pin15 AF0 Pin14,13

/*
 * Port B setup.
 * PB0  - LED green (LED 1:ON 0:OFF)
 * PB3  - I2S1_CK (AF5)
 * PB5  - I2S1_SD (AF5)
 * PB7  - LED blue
 * PB8  - CAN1_RX (AF9)
 * PB9  - CAN1_TX (AF9)
 * PB12 - CAN2_RX (AF9)
 * PB13 - CAN2_TX (AF9)
 * PB14 - LED red
 */
#define GPIOB_MODER   0x1a0a4881 // AF9 Pin13,12,9,8 AF5 Pin5,3 
#define GPIOB_OTYPER  0x00000000 // Push-Pull
#define GPIOB_OSPEEDR 0x00000cc0 // High speed: Pin5,3
#define GPIOB_PUPDR   0x55555555 // Pull-up
#define GPIOB_ODR     0x00000000
#define GPIOB_AFR0    0x00505000 // AF5 Pin5,3
#define GPIOB_AFR1    0x00990099 // AF9 Pin13,12,9,8

/*
 * Port C setup.
 * PC13 - USER Button
 */
#define GPIOC_MODER   0x00000000 // Input Pin13
#define GPIOC_OTYPER  0x00000000 // Push-Pull
#define GPIOC_OSPEEDR 0x00000000
#define GPIOC_PUPDR   0x01555555 // Input Pin15-13 floating
#define GPIOC_ODR     0x00000000
#define GPIOC_AFR0    0x00000000
#define GPIOC_AFR1    0x00000000

/*
 * Port D setup.
 * PD8  - USART3_TX (AF7)
 * PD9  - USART3_RX (AF7)
 * PD12 - I2C3_SCL(AF4)
 * PD13 - I2C3_SDA(AF4)
 */
#define GPIOD_MODER   0x0a0a0000 // AF Pin9,8
#define GPIOD_OTYPER  0x00003000 // Pin13,12 Open drain otherwise Push-Pull
#define GPIOD_OSPEEDR 0x0f000000 // High speed: Pin13,12
#define GPIOD_PUPDR   0x55505555 // Pin9,8 floating
#define GPIOD_ODR     0x00000000
#define GPIOD_AFR0    0x00000000
#define GPIOD_AFR1    0x00440077 // AF4 Pin13,12 AF7 Pin9,8

#define GPIO_LED GPIOB
#define GPIO_LED_SET_TO_EMIT    0

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

#if UAVCAN_STM32_NUM_IFACES > 1
  RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
  RCC->APB1RSTR = RCC_APB1RSTR_CAN2RST;
  RCC->APB1RSTR = 0;
#endif

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

  GPIOD->AFR[0]  = GPIOD_AFR0;
  GPIOD->AFR[1]  = GPIOD_AFR1;
  GPIOD->OSPEEDR = GPIOD_OSPEEDR;
  GPIOD->OTYPER  = GPIOD_OTYPER;
  GPIOD->ODR     = GPIOA_ODR;
  GPIOD->MODER   = GPIOD_MODER;
  GPIOD->PUPDR   = GPIOD_PUPDR;
}

void
ef::board::set_led (bool on)
{
  if (on)
    GPIO_LED->BSRR = (1 << GPIO_LED_SET_TO_EMIT);
  else
    GPIO_LED->BSRR = (1 << GPIO_LED_SET_TO_EMIT) << 16;
}
