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
 * RCC init
 */

void
ef::mcu::rcc_init (void)
{
  // Use system ossillator
  SYSCON->PDRUNCFG &= ~(1 << 5);
  for (volatile int i = 0; i < 1000; i++)
    ;

  SYSCON->SYSPLLCLKSEL = 1;

  // Power down system PLL
  SYSCON->PDRUNCFG |= (1 << 7);

  // 48MHz: main 48MHz, P=(1<<1), M=(3+1), 48*2*P=192MHz 192/(2*P)/M=12
  SYSCON->SYSPLLCTRL = (1 << 5)|(3 << 0);
  SYSCON->SYSAHBCLKDIV = 1;

  SYSCON->PDRUNCFG &= ~(1 << 7);
  while (!(SYSCON->SYSPLLSTAT & 0x01))
    ;

  // 3 system clocks flash access time for 48MHz clock
  FLASHCTRL->FLASHTIM = (FLASHCTRL->FLASHTIM & ~0x03)|0x2;

  SYSCON->MAINCLKSEL = 0x03;
  SYSCON->MAINCLKUEN = 0x01;
  while (!(SYSCON->MAINCLKUEN & 0x01))
    ;

  // Enable GPIO, UART, IOCO and CAN clock
  SYSCON->SYSAHBCLKCTRL |= (1 << 6)|(1 << 12)|(1 << 16)|(1 << 17);

  // Reset SSP0, I2C, SSP1 and CAN
  SYSCON->PRESETCTRL &= ~0xf;
  SYSCON->PRESETCTRL |= 0xf;

  // Use vector on RAM?
}
