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

#define INTR_REQ_I2S1		35
// SPI1_RX DMA2 CH3 Stream2
#define INTR_REQ_DMA2_Stream2	58

#define CR2_RXDMAEN	(1 << 0)
#define CR2_TXDMAEN	(1 << 1)
#define CR2_ERRIE	(1 << 5)
#define CR2_RXNEIE	(1 << 6)
#define CR2_TXEIE	(1 << 7)

#define SR_RXNE		(1 << 0)
#define SR_TXE		(1 << 1)
#define SR_CHSIDE	(1 << 2)
#define SR_UDR		(1 << 3)
#define SR_OVR		(1 << 6)
#define SR_BSY		(1 << 7)
#define SR_FRE		(1 << 8)

#define CFGR_CHLEN	(1 << 0)
#define CFGR_DATALEN_16	(0 << 1)
#define CFGR_DATALEN_24	(1 << 1)
#define CFGR_DATALEN_32	(2 << 1)
#define CFGR_CKPOL	(1 << 3)
#define CFGR_I2SSTD_I2S	(0 << 4)
#define CFGR_I2SSTD_MSB	(1 << 4)
#define CFGR_I2SSTD_LSB	(2 << 4)
#define CFGR_I2SSTD_PCM	(3 << 4)
#define CFGR_PCMSYNC	(1 << 7)
#define CFGR_RECIEVE	(1 << 8)
#define CFGR_MASTER	(1 << 9)
#define CFGR_I2SE	(1 << 10)
#define CFGR_I2SMOD	(1 << 11)
#define CFGR_ASTREN	(1 << 12)

#define PR_ODD		(1 << 8)
#define PR_MCKOE	(1 << 9)

#define I2S_BUFSIZE	4096
#define I2S_BUFSIZEAQRT	64
static uint32_t buf0[I2S_BUFSIZE/2+1];
static uint32_t buf1[I2S_BUFSIZE/2+1];

void
i2s_init (void)
{
  // Enable DMA2
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  RCC->APB1RSTR = RCC_AHB1RSTR_DMA2RST;
  RCC->APB1RSTR = 0;

  // Enable SPI1 clock.
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  RCC->APB2RSTR = RCC_APB2RSTR_SPI1RST;
  RCC->APB2RSTR = 0;

  /* fs=16k: SYSCLK=96MHz I2SDIV=47 ODD=0
     fs=32k: SYSCLK=96MHz I2SDIV=23 ODD=1 */
  I2S1->I2SPR = PR_ODD|23;
  // master recieve, I2S Philips, data 24bit, 32-bit channel
  I2S1->I2SCFGR = (CFGR_I2SMOD | CFGR_MASTER | CFGR_RECIEVE
		   | CFGR_I2SSTD_I2S | CFGR_DATALEN_24 | CFGR_CHLEN);

  // Configure interrupt and DMA

  I2S1->CR2 |= CR2_RXDMAEN;
  // SPI1_RX: DMA2 stream 2 channel 3
  // Stop stream0 first
  DMA2_Stream[2].CR &= ~DMA_SxCR_EN;
  while (DMA2_Stream[2].CR & DMA_SxCR_EN)
    ;
  DMA2->LIFCR = (DMA2->LISR
		 & (DMA_TCIF2|DMA_HTIF2|DMA_TEIF2|DMA_DMEIF2|DMA_FEIF2));
  DMA2_Stream[2].PAR = (uint32_t) (void *) &(I2S1->DR);
  DMA2_Stream[2].M0AR = (uint32_t) (void *) buf0;
  DMA2_Stream[2].M1AR = (uint32_t) (void *) buf1;
  DMA2_Stream[2].NDTR = I2S_BUFSIZE;
  // ch3, single burst, double buffer, prio high, half to half, minc, p2m
  DMA2_Stream[2].CR = ((3 << DMA_SxCR_CHSEL_shift)
		       | (DMA_SxCR_BURST_SINGLE << DMA_SxCR_MBURST_shift)
		       | (DMA_SxCR_BURST_SINGLE << DMA_SxCR_PBURST_shift)
		       | DMA_SxCR_DBM
		       | (DMA_SxCR_PL_HIGH << DMA_SxCR_PL_shift)
		       | (DMA_SxCR_SIZE_HALF << DMA_SxCR_MSIZE_shift)
		       | (DMA_SxCR_SIZE_HALF << DMA_SxCR_PSIZE_shift)
		       | DMA_SxCR_MINC
		       | (DMA_SxCR_DIR_P2M << DMA_SxCR_DIR_shift)
		       | (DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE));
}

int
i2s_current_buffer (void)
{
  if (DMA2_Stream[2].CR & DMA_SxCR_CT)
    return 1;
  return 0;
}

bool
i2s_read (uint32_t *bp)
{
  uint32_t v;

  while ((I2S1->SR & SR_RXNE) == 0)
    ;
  v = I2S1->DR;
  v <<= 16;
  while ((I2S1->SR & SR_RXNE) == 0)
    ;
  v |= (I2S1->DR & 0xffff);
  if (bp)
    *bp = v;

  return true;
}

extern "C" {
  extern float log10f (float);
  extern void cdft (int, int, float *, int *, float *);
  extern void makewt (int nw, int *ip, float *w);
}

#define NMAX I2S_BUFSIZE/2
#define NMAXSQRT I2S_BUFSIZEAQRT
static int cfdt_ip[NMAXSQRT + 2];
static float cfdt_w[NMAX * 5 / 4];
float spectrum[NMAX/2];

void
i2s_spectrum (void *arg  __attribute__ ((unused)))
{
  uint32_t isr;
  int i;
  float *ap, *tp;

  // Prepare cfdt_w table.
  makewt (NMAX >> 2, cfdt_ip, cfdt_w);
  
  // Start stream2
  DMA2_Stream[2].CR |= DMA_SxCR_EN;
  
  // Start I2S
  I2S1->I2SCFGR |= CFGR_I2SE;

  while (1)
    {
      bitset flags;

      // Wait DMA2 Stream2 interrupt
      thread::poll_section ();
      flags.clear ();
      id_t irq_id = eventflag::irq_event (INTR_REQ_DMA2_Stream2);
      flags.add (irq_id);
      id_t id = thread::poll (flags);
      // Get status and handle errors
      isr = DMA2->LISR;
      if (isr & (DMA_TEIF2|DMA_DMEIF2|DMA_FEIF2))
	{
	  // Report error and restart dma - NOTYET
	  // Clear these flags
	  DMA2->LIFCR = isr & (DMA_TEIF2|DMA_DMEIF2|DMA_FEIF2);
	}
      if (!(isr & DMA_TCIF2))
	{
	  DMA2->LIFCR = isr & DMA_TCIF2;
	  continue;
	}
      DMA2->LIFCR = isr & DMA_TCIF2;
      
      board::set_led (i2s_current_buffer ());

      // Do FFT on buffer
      ap = (float *)(void *) ((DMA2_Stream[2].CR & DMA_SxCR_CT)? buf0 : buf1);
      for (tp = ap, i = 0; i < NMAX/2; i++)
	{
	  uint16_t v0 = ((uint16_t *)(void *) tp)[0];
	  uint16_t v1 = ((uint16_t *)(void *) tp)[1];
	  int32_t v = (int32_t)((v0 << 16) | v1);

	  *tp++ = ((float) (v >> 8))/(1<<23);
	  *tp++ = 0;
	}

      cdft (NMAX, 1, ap, cfdt_ip, cfdt_w);

      // Gen power spectrum and report if needed
      for (tp = ap, i = 0; i < NMAX/2; i++)
	{
	  float re, im;
	  re = *tp++;
	  im = *tp++;
	  spectrum[i] = log10f (re * re + im * im);
	  if (spectrum[i] >= 0)
	    {
	      uint8_t buf[6];
	      union { float x; uint8_t bytes[4]; } u;
	      u.x = spectrum[i];
	      buf[0] = i >> 8;
	      buf[1] = i & 0xff;
	      buf[2] = u.bytes[0];
	      buf[3] = u.bytes[1];
	      buf[4] = u.bytes[2];
	      buf[5] = u.bytes[3];
	      // can_send (333, buf, sizeof (buf));
	    }
	}
      // EOM
      // can_send (334, NULL, 0);
    }
}
