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
  // This is NOT clock setting but perhaps good time for it.
  SCB->CPACR |= (0xf << 20);
  asm volatile ("dsb\n\t"
		"isb" : : : "memory");
#if defined(USE_FPU_AUTO_SAVE)
  // Auto save/restore fp context on exceptions.
  FPU->FPCCR = (1 << 31);
#else
  // No auto save/restore.
  FPU->FPCCR = 0;
#endif

  // HSI setup
  RCC->CR |= RCC_CR_HSION;
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;
  // Reset CR
  RCC->CR = 0x83;
  RCC->CFGR = 0;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;
  // Reset PLLCFGR
  RCC->PLLCFGR = 0x24003010;
  // Reset PLLI2SCFGR
  RCC->PLLSAICFGR = 0x24003010;
  // Reset PLLSAICFGR
  RCC->PLLSAICFGR = 0x24003000;

  // Disable all interrupts
  RCC->CIR = 0;

  // HSE setup
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;

  // PLL setup
  RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE
		  | (8 << 0)		// PLLM=8
		  | (360 << 6)	// PLLN=360
		  | (0 << 16)		// PLLP=DIV2 0:DIV2 1:DIV4 2:DIV6 3:DIV8
		  | (6 << 24)		// PLLQ=6
		  | (2 << 28));	// PLLR=2
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;

  // PLLI2S setup
  RCC->PLLI2SCFGR = ((8 << 0)	    // PLLM=8 */
		     | (192 << 6)	// PLLN=192 */
		     | (0 << 16)	// PLLP=DIV2 0:DIV2 1:DIV4 2:DIV6 3:DIV8
		     | (6 << 24)	// PLLQ=6
		     | (2 << 28));// PLLR=2
  RCC->CR |= RCC_CR_PLLI2SON;
  while (!(RCC->CR & RCC_CR_PLLI2SRDY))
    ;

  // PLLSAI setup
  RCC->PLLSAICFGR = ((8 << 0)	    // PLLM=8
		     | (384 << 6)	// PLLN=384
		     | (3 << 16)	// PLLP=DIV8 0:DIV2 1:DIV4 2:DIV6 3:DIV8
		     | (6 << 24)	// PLLQ=6
		     | (2 << 28));// must be Reset value
  RCC->CR |= RCC_CR_PLLSAION;
  while (!(RCC->CR & RCC_CR_PLLSAIRDY))
    ;

  // Clock settings
  RCC->CFGR = ((0 << 30)		// MCO2 0:SYS 1:PLLI2S 2:HSE 3:PLL
	       | (6 << 27)	// MCO2PRE 4:DIV2 5:DIV3 6:DIV4 7:DIV5
	       | (4 << 24)	// MCO1PRE 4:DIV2 5:DIV3 6:DIV4 7:DIV5
	       | (0 << 23)	// I2SSRC 0:PLLI2S 1:I2S_CKIN
	       | (0 << 21)	// MCO1 0:HSI 1:LSE 2:HSE 3:PLL
	       | (0 << 16)	// RTCPRE HSE division factor
	       | RCC_CFGR_PPRE2_DIV2	// 4:DIV2 5:DIV4 6:DIV8 7:DIV16
	       | RCC_CFGR_PPRE1_DIV4
	       | (0 << 4)		// HPRE 0xxx:DIV1 4:DIV2 ... 15:DIV512
	       | (0 << 2)		// SWS read only bits
	       | (0 << 0));	// SW 0:HSI 1:HSE 2:PLL

  RCC->DCKCFGR2 = (1 << 27);	// CK48MSEL 0:PLL_Q 1:PLLSAI_P

  /*
   * We don't touch RCC->CR2, RCC->CFGR2, RCC->CFGR3, and RCC->CIR.
   */

  // Flash setup
  FLASH->ACR = (FLASH_ACR_LATENCY_5WS	// 6 CPU cycle wait
		| FLASH_ACR_PRFTEN
		| FLASH_ACR_ICEN
		| FLASH_ACR_DCEN);

  // CRC
  RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

  // Switching on the configured clock source.
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SW_PLL) != RCC_CFGR_SW_PLL)
    ;

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->APB2RSTR = RCC_APB2RSTR_SYSCFGRST;
  RCC->APB2RSTR = 0;

  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  RCC->APB1RSTR = RCC_AHB1RSTR_DMA2RST;
  RCC->APB1RSTR = 0;

  // Use vectors on RAM
  SYSCFG->MEMRMP = (SYSCFG->MEMRMP & ~SYSCFG_MEMRMP_MEM_MODE) | 0x3;
}
