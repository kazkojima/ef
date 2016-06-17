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

void
ef::mcu::gpio_init (void)
{
  // Enable GPIO, UART, IOCO and CAN clock
  SYSCON->SYSAHBCLKCTRL |= (1 << 6)|(1 << 12)|(1 << 16)|(1 << 17);

  // Reset SSP0, I2C, SSP1 and CAN
  SYSCON->PRESETCTRL &= ~0xf;
  SYSCON->PRESETCTRL |= 0xf;

  // Set Pins
  // UART RXD(PIO1_6): FUNC[2:0]=1, MODE[4:3]=0, HYS[5]=0, [9:6]=3, OD[10]=0
  IOCON->PIO1[6] = (IOCON->PIO1[6] & ~0x43f)|(1 << 0);
  // UART TXD(PIO1_7): FUNC[2:0]=1, MODE[4:3]=0, HYS[5]=0, [9:6]=3, OD[10]=0
  IOCON->PIO1[7] = (IOCON->PIO1[7] & ~0x43f)|(1 << 0);
  
  // GPIO LED(PIO1_10): FUNC[2:0]=0, MODE[4:3]=0, HYS[5]=0, [9:6]=3, OD[10]=1
  IOCON->PIO1[10] = (IOCON->PIO1[10] & ~0x43f)|(1 << 10);
  // GPIO LED(PIO1_11): FUNC[2:0]=0, MODE[4:3]=0, HYS[5]=0, [9:6]=3, OD[10]=1
  IOCON->PIO1[11] = (IOCON->PIO1[11] & ~0x43f)|(1 << 10);

  GPIO[1].DATA[(1 << 10)|(1 << 11)] = (1 << 10)|(1 << 11);  
  // GPIO Set PIO1_10 and _11 as output
  GPIO[1].DIR = (1 << 10)|(1 << 11);
}

void
ef::board::set_led (bool on)
{
  // LED GPIO pins are open drain, i.e. negative logic
  if (on)
    GPIO[1].DATA[(1 << 11)] = (0 << 11); 
  else
    GPIO[1].DATA[(1 << 11)] = (1 << 11); 
}
