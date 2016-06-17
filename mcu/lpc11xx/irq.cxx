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

// LPC11xx IRQ

using namespace ef;

struct NVIC
{
  volatile uint32_t ISER[1];
  uint32_t unused1[31];
  volatile uint32_t ICER[1];
  uint32_t unused2[31];
  volatile uint32_t ISPR[1];
  uint32_t unused3[31];
  volatile uint32_t ICPR[1];
  uint32_t unused4[31];
  uint32_t unused5[64];
  volatile uint32_t IPR[8];
};

static struct NVIC *const NVIC = (struct NVIC *const)0xE000E100;
#define NVIC_ISER(n)	(NVIC->ISER[0])
#define NVIC_ICER(n)	(NVIC->ICER[0])
#define NVIC_ICPR(n)	(NVIC->ICPR[0])
#define NVIC_IPR(n)	(NVIC->IPR[n >> 2])

void
irq::enable (void)
{
  NVIC_ISER (irq_num) = 1 << (irq_num & 0x1f);
}

void
irq::clear (void)
{
  NVIC_ICPR (irq_num) = 1 << (irq_num & 0x1f);
}

void
irq::disable (void)
{
  NVIC_ICER (irq_num) = 1 << (irq_num & 0x1f);
}

void
irq::set_prio (void)
{
  unsigned int sh = (irq_num & 3) << 3;

  NVIC_IPR (irq_num) = ((NVIC_IPR(irq_num) & ~(0xFF << sh))
			| (ef::PRIO_IRQ << sh));
}
