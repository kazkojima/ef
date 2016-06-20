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

// raw operations like entry initialize, context switch, mutex and event
#include <cstring>
#include <unistd.h>
#include "mcu.h"
#include "ef.h"

#if defined(MCU_STM32F446)
# include "mcu/stm32/stm32f446/sys.cxx"
#elif defined(MCU_STM32F746)
# include "mcu/stm32/stm32f746/sys.cxx"
#elif defined(MCU_LPC11C24)
# include "mcu/lpc11xx/lpc11c24/sys.cxx"
#else
# error "Unsupported MCU"
#endif
