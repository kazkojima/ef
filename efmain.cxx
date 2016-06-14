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

#include "ef.h"
#include "mcu.h"

using namespace ef;

// Example: usart echo thread

#define INTR_REQ_USART2		38

static void
echo (void *arg  __attribute__ ((unused)))
{
  board::usart_init ();

  while (1)
    {
      bitset flags;

      // USART2 RX interrupt handler with timeout function
      thread::poll_section ();
      flags.clear ();
      id_t to_id = eventflag::timeout_event (5000*1000);
      flags.add (to_id);
      id_t irq_id = eventflag::irq_event (INTR_REQ_USART2);
      flags.add (irq_id);
      id_t id = thread::poll (flags);
      if (id == irq_id)
	board::usart_send (USART2->DR);
      else
	board::usart_send ('x');
    }
}

// Yet another led brinker

void
ef::main (void *arg  __attribute__ ((unused)))
{
  thread *tp = thread::create (5, echo, NULL, NULL, 512, 0);
  tp->run ();

  while (1)
    {
      bitset flags;

      board::set_led (true);
      thread::poll_section ();
      flags.clear ();
      flags.add (eventflag::timeout_event (1000*1000));
      thread::poll (flags);
      board::set_led (false);
      thread::poll_section ();
      flags.clear ();
      flags.add (eventflag::timeout_event (500*1000));
      thread::poll (flags);
    }
}
