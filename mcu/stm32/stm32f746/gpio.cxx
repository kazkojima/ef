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
 * PA1  - RMII_REF_CLK(AF11)
 * PA2  - RMII_MDIO(AF11)
 * PA7  - RMII_RX_DV(AF11)
 * PA13, PA14 - SWD(AF0)
 * PA15 - I2S3_WS (AF6)
 */
#define GPIOA_MODER   0xa8008028 // AF Pin15,14,13,2,1
#define GPIOA_OTYPER  0x00000000 // Push-Pull
#define GPIOA_OSPEEDR 0x00000000 // High speed: Pin12,11
#define GPIOA_PUPDR   0x64155551 // Pin14 pull-down Pin13,12,1 floating
#define GPIOA_ODR     0x00000000
#define GPIOA_AFR0    0xb0000bb0 // AF11 2,1
#define GPIOA_AFR1    0x60000000 // AF6 Pin15 AF0 Pin14,13

/*
 * Port B setup.
 * PB0  - LED green (LED 1:ON 0:OFF)
 * PB3  - I2S3_CK (AF6)
 * PB5  - I2S3_SD (AF6)
 * PB7  - LED blue
 * PB13 - RMII_TXD1(AF11)
 * PB14 - LED red
 */
#define GPIOB_MODER   0x18004881 // AF11 Pin13 AF6 Pin5,3 
#define GPIOB_OTYPER  0x00000000 // Push-Pull
#define GPIOB_OSPEEDR 0x00000cc0 // High speed: Pin5,3
#define GPIOB_PUPDR   0x55555555 // Pull-up
#define GPIOB_ODR     0x00000000
#define GPIOB_AFR0    0x00606000 // AF6 Pin5,3
#define GPIOB_AFR1    0x00b00000 // AF11 Pin13

/*
 * Port C setup.
 * PC1  - RMII_MDC (AF11)
 * PC4  - RMII_RXD0 (AF11)
 * PC5  - RMII_RXD1 (AF11)
 * PC7  - I2S3_MCK (AF6)
 * PC13 - USER Button
 */
#define GPIOC_MODER   0x00008a08 // Input Pin13 AF6 Pin7 AF11 Pin5,4,1
#define GPIOC_OTYPER  0x00000000 // Push-Pull
#define GPIOC_OSPEEDR 0x0000c000 // High speed: Pin7
#define GPIOC_PUPDR   0x01555051 // Input Pin15-13,5,4,1 floating
#define GPIOC_ODR     0x00000000
#define GPIOC_AFR0    0x60bb00b0 // AF6 Pin7 AF11 Pin5,4,1
#define GPIOC_AFR1    0x00000000

/*
 * Port D setup.
 * PD0  - CAN1_RX (AF9)
 * PD1  - CAN1_TX (AF9)
 * PD8  - USART3_TX (AF7)
 * PD9  - USART3_RX (AF7)
 */
#define GPIOD_MODER   0x000a000a // AF Pin9,8,1,0
#define GPIOD_OTYPER  0x00000000 // Push-Pull
#define GPIOD_OSPEEDR 0x00000000 // High speed: Pin13,12
#define GPIOD_PUPDR   0x55505555 // Pin9,8 floating
#define GPIOD_ODR     0x00000000
#define GPIOD_AFR0    0x00000099 // AF9 Pin1,0
#define GPIOD_AFR1    0x00000077 // AF7 Pin9,8

/*
 * Port E setup.
 */
#define GPIOE_MODER   0x00000000
#define GPIOE_OTYPER  0x00000000 // Push-Pull
#define GPIOE_OSPEEDR 0x00000000
#define GPIOE_PUPDR   0x55555555
#define GPIOE_ODR     0x00000000
#define GPIOE_AFR0    0x00000000
#define GPIOE_AFR1    0x00000000

/*
 * Port F setup.
 * PF0  - I2C2_SDA(AF4)
 * PF1  - I2C2_SCL(AF4)
 */
#define GPIOF_MODER   0x0000000a // AF Pin1,0
#define GPIOF_OTYPER  0x00000003 // Pin1,0 Open drain otherwise Push-Pull
#define GPIOF_OSPEEDR 0x0000000f // High speed: Pin1,0
#define GPIOF_PUPDR   0x55555555
#define GPIOF_ODR     0x00000000
#define GPIOF_AFR0    0x00000044 // AF4 Pin1,0 
#define GPIOF_AFR1    0x00000000

/*
 * Port G setup.
 * PG11 - RMII_TX_EN (AF11)
 * PG13 - RMII_TXD0 (AF11)
 */
#define GPIOG_MODER   0x08800000 // AF Pin13,11
#define GPIOG_OTYPER  0x00000000 // Push-Pull
#define GPIOG_OSPEEDR 0x00000000
#define GPIOG_PUPDR   0x51155555 // Pin13,11 floating
#define GPIOG_ODR     0x00000000
#define GPIOG_AFR0    0x00000000
#define GPIOG_AFR1    0x00b0b000 // AF11 Pin13,11

/*
 * Port H setup.
 */
#define GPIOH_MODER   0x00000000
#define GPIOH_OTYPER  0x00000000 // Push-Pull
#define GPIOH_OSPEEDR 0x00000000
#define GPIOH_PUPDR   0x55555555
#define GPIOH_ODR     0x00000000
#define GPIOH_AFR0    0x00000000
#define GPIOH_AFR1    0x00000000

/*
 * Port I setup.
 */
#define GPIOI_MODER   0x00000000
#define GPIOI_OTYPER  0x00000000 // Push-Pull
#define GPIOI_OSPEEDR 0x00000000
#define GPIOI_PUPDR   0x55555555
#define GPIOI_ODR     0x00000000
#define GPIOI_AFR0    0x00000000
#define GPIOI_AFR1    0x00000000


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

#if 0
  RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
  RCC->APB1RSTR = RCC_APB1RSTR_CAN2RST;
  RCC->APB1RSTR = 0;
#endif

  // Then enable GPIO clock.
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN
		   | RCC_AHB1ENR_GPIOBEN
		   | RCC_AHB1ENR_GPIOCEN
		   | RCC_AHB1ENR_GPIODEN
		   | RCC_AHB1ENR_GPIOEEN
		   | RCC_AHB1ENR_GPIOFEN
		   | RCC_AHB1ENR_GPIOGEN);
  RCC->AHB1RSTR = (RCC_AHB1RSTR_GPIOARST
		   | RCC_AHB1RSTR_GPIOBRST
		   | RCC_AHB1RSTR_GPIOCRST
		   | RCC_AHB1RSTR_GPIODRST
		   | RCC_AHB1RSTR_GPIOERST
		   | RCC_AHB1RSTR_GPIOFRST
		   | RCC_AHB1RSTR_GPIOGRST);
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

  GPIOE->AFR[0]  = GPIOE_AFR0;
  GPIOE->AFR[1]  = GPIOE_AFR1;
  GPIOE->OSPEEDR = GPIOE_OSPEEDR;
  GPIOE->OTYPER  = GPIOE_OTYPER;
  GPIOE->ODR     = GPIOA_ODR;
  GPIOE->MODER   = GPIOE_MODER;
  GPIOE->PUPDR   = GPIOE_PUPDR;

  GPIOF->AFR[0]  = GPIOF_AFR0;
  GPIOF->AFR[1]  = GPIOF_AFR1;
  GPIOF->OSPEEDR = GPIOF_OSPEEDR;
  GPIOF->OTYPER  = GPIOF_OTYPER;
  GPIOF->ODR     = GPIOA_ODR;
  GPIOF->MODER   = GPIOF_MODER;
  GPIOF->PUPDR   = GPIOF_PUPDR;

  GPIOG->AFR[0]  = GPIOG_AFR0;
  GPIOG->AFR[1]  = GPIOG_AFR1;
  GPIOG->OSPEEDR = GPIOG_OSPEEDR;
  GPIOG->OTYPER  = GPIOG_OTYPER;
  GPIOG->ODR     = GPIOA_ODR;
  GPIOG->MODER   = GPIOG_MODER;
  GPIOG->PUPDR   = GPIOG_PUPDR;
}

void
ef::board::set_led (bool on)
{
  if (on)
    GPIO_LED->BSRR = (1 << GPIO_LED_SET_TO_EMIT);
  else
    GPIO_LED->BSRR = (1 << GPIO_LED_SET_TO_EMIT) << 16;
}
