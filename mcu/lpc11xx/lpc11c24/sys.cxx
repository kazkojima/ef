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

#include "ldsym.h"
#include "systimer.h"
#include "context.h"

typedef void (*initfp)(void);

static void
loop (void *arg __attribute__ ((unused)))
{
  while (1)
    ;
}

void __attribute__ ((naked, section(".startup.entry")))
ef::sys::entry (void)
{
  asm volatile ("cpsid	i" : : : "memory");
  mcu::rcc_init ();
  memset (&_bss_start, 0, &_bss_end - &_bss_start);
  memcpy (&_data, &_textdata, &_edata - &_data);
  for (initfp *fp = (initfp *)&__init_array_start;
       fp < (initfp *)&__init_array_end; fp++)
    (*fp) ();

#ifdef USE_VECTOR_ON_RAM
  memcpy (sys::vector, sys::_vector, 1024);
#endif

  prio_init ();
  systimer::init ();
  mcu::gpio_init ();

  // Create idle thread
  thread::create (MAX_ID-1, loop, NULL, NULL, 128, 0);
  thread::runnable.add (MAX_ID-1);
  // Create main thread
  thread *tp = thread::create (MAX_ID-2, ef::main, NULL, loop, 128, 0);
  thread::runnable.add (MAX_ID-2);
  thread::running = tp;
  uint32_t psp = ((context *) tp)->sp;

  psp += sizeof (context_on_stack);

  // Use PSP for threads 
  asm ("msr	PSP, %0\n\t"
       "msr	CONTROL, %1\n\t"
       "isb"
       : : "r" (psp), "r" (2) : "memory");

  // Enable interrupts
  asm volatile ("cpsie	i" : : : "memory");

  ef::main (NULL);

  for (;;);
}

static void nmi (void)
{
  for (;;);
}

static void hard_fault (void)
{
  for (;;);
}

static void none (void)
{
}

typedef void (*handler)(void);

handler ef::sys::_vector[48] __attribute__ ((section(".startup.vectors"))) = {
  (handler)(&__initial_sp__),
  ef::sys::entry,
  nmi,			// -14:nmi
  hard_fault,		// -13:hard fault
  none,			// -12: reserved
  none,			// -11: reserved
  none,			// -10: reserved
  none,			//  -9: reserved
  none,			//  -8: reserved
  none,			//  -7: reserved
  none,			//  -6: reserved
  none,			//  -5: SVCall
  none,			//  -4: Debug
  none,			//  -3: reserved
  ef::exc::preempt,	//  -2: PendSV
  ef::exc::systick,	//  -1: SysTick
  ef::exc::irq_handler,	//   0: PIO0_0
  ef::exc::irq_handler, //   1: PIO0_1
  ef::exc::irq_handler, //   2: PIO0_2
  ef::exc::irq_handler, //   3: PIO0_3
  ef::exc::irq_handler, //   4: PIO0_4
  ef::exc::irq_handler, //   5: IRQ 5
  ef::exc::irq_handler, //   6: IRQ 6
  ef::exc::irq_handler, //   7: IRQ 7
  ef::exc::irq_handler, //   8: IRQ 8
  ef::exc::irq_handler, //   9: IRQ 9
  ef::exc::irq_handler, //  10: IRQ 10
  ef::exc::irq_handler, //  11: IRQ 11
  ef::exc::irq_handler, //  12: PIO1_0
  ef::exc::irq_handler, //  13: CAN
  ef::exc::irq_handler, //  14: SSP1
  ef::exc::irq_handler, //  15: I1C0
  ef::exc::irq_handler, //  16: TIMER_16_0
  ef::exc::irq_handler, //  17: TIMER_16_1
  ef::exc::irq_handler, //  18: TIMER_32_0
  ef::exc::irq_handler, //  19: TIMER_32_1
  ef::exc::irq_handler, //  20: SSP0
  ef::exc::irq_handler, //  21: UART0
  ef::exc::irq_handler, //  22: Reserved
  ef::exc::irq_handler, //  23: Reserved
  ef::exc::irq_handler, //  24: ADC
  ef::exc::irq_handler, //  25: WDT
  ef::exc::irq_handler, //  26: BOD
  ef::exc::irq_handler, //  27: Reserved
  ef::exc::irq_handler, //  28: EINT3
  ef::exc::irq_handler, //  29: EINT2
  ef::exc::irq_handler, //  30: EINT1
  ef::exc::irq_handler, //  31: EINT0
};

#ifdef USE_VECTOR_ON_RAM
handler ef::sys::vector[48] __attribute__ ((section(".vectors")));
#endif
