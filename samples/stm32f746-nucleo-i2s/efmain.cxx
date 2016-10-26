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

// Example: Recieve i2s audio data with DMA and do FFT to get its power
//  spectrum.
//
// fftsg.c file is the FFT Package in C which is a part of GPFFT.
// GPFFT(General Purpose FFT) is written and copyrighted by Takuya OOURA.
// See readme.GPFFT.txt for details.

extern void i2s_init (void);
extern void i2s_spectrum (void *arg);
extern void lwip_thread (void *arg);

static uint32_t millis;

// Yet another led brinker?

void
ef::main (void *arg  __attribute__ ((unused)))
{
  i2s_init ();

  thread *ltp = thread::create (5, lwip_thread, NULL, NULL, 2048, 0);
  ltp->run ();
  thread *tp = thread::create (6, i2s_spectrum, NULL, NULL, 8192, 0);
  tp->run ();

  while (1)
    {
      bitset flags;

      thread::poll_section ();
      flags.clear ();
      flags.add (eventflag::timeout_event (1000));
      thread::poll (flags);
      millis++;
    }
}

extern "C" {
  uint32_t sys_now (void)
  {
    return millis;
  }
}
