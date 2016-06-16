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

#include "systimer.h"
#include "ef.h"

#if defined(MCU_STM32F446)
# include "mcu/stm32/systimer.cxx"
#else
# error "Unsupported MCU"
#endif

struct timer_cell { uint32_t val, id; };
static std::list<timer_cell> timer_list;

void
ef::timers::add (uint32_t usec, uint32_t id)
{
  uint32_t max_ticks = systimer::max_ticks ();

  timer_cell tc;
  tc.id = id;

  if (!timer_list.empty ())
    {
      std::list<timer_cell>::iterator it = timer_list.begin ();
      (*it).val += systimer::ticks_to_usec (systimer::get ());
      systimer::init ();
  
      for (; it != timer_list.end (); it++)
	{
	  if (usec < (*it).val)
	    {
	      (*it).val = (*it).val - usec;
	      tc.val = usec;
	      timer_list.insert (it, tc);
	      break;
	    }
	  else
	    usec -= (*it).val;
	}

      if (it == timer_list.end ())
	{
	  tc.val = usec;
	  timer_list.push_back (tc);
	}

      // Fixup head value
      it = timer_list.begin ();
      usec = (*it).val;
      uint32_t ticks = systimer::usec_to_ticks (usec);
      if (ticks > max_ticks)
	ticks = max_ticks;
      usec -= systimer::ticks_to_usec (ticks);
      systimer::reload (ticks);
      (*it).val = usec;
    }
  else
    {
      uint32_t ticks = systimer::usec_to_ticks (usec);
      if (ticks > max_ticks)
	ticks = max_ticks;
      usec -= systimer::ticks_to_usec (ticks);
      systimer::reload (ticks);
      tc.val = usec;
      timer_list.push_front (tc);
    }
}

void
ef::timers::update (void)
{
  systimer::init ();

  // ???
  if (timer_list.empty ())
    return;

  std::list<timer_cell>::iterator it = timer_list.begin ();
  uint32_t usec = (*it).val;

  if (usec == 0)
    {
      eventflag::signal ((*it).id);
      eventflag::release_id ((*it).id);
      timer_list.pop_front ();
      if (timer_list.empty ())
	return;

      it = timer_list.begin ();
      usec = (*it).val;
    }

  // Fixup head
  uint32_t max_usec = systimer::ticks_to_usec (systimer::max_ticks ());
  if (usec > max_usec)
    {
      usec = max_usec;
      (*it).val -= max_usec;
    }
  else
    (*it).val = 0;
  
  systimer::reload (systimer::usec_to_ticks (usec));
}

