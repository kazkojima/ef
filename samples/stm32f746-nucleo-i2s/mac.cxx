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

#define INTR_REQ_ETH	61

// MAC registers

#define MACCR_CSTF		(1 << 25)
#define MACCR_WD		(1 << 23)
#define MACCR_JD		(1 << 22)
#define MACCR_IFG_shift		17
#define MACCR_CSD		(1 << 16)
#define MACCR_FES		(1 << 14)
#define MACCR_ROD		(1 << 13)
#define MACCR_LM		(1 << 12)
#define MACCR_DM		(1 << 11)
#define MACCR_IPCO		(1 << 10)
#define MACCR_RD		(1 << 9)
#define MACCR_APCS		(1 << 8)
#define MACCR_BL_shift		5
#define MACCR_DC		(1 << 4)
#define MACCR_TE		(1 << 3)
#define MACCR_RE		(1 << 2)

#define MACFFR_RA		(1 << 31)
#define MACFFR_HPF		(1 << 10)
#define MACFFR_SAF		(1 << 9)
#define MACFFR_SAIF		(1 << 8)
#define MACFFR_PCF_shift	6
#define MACFFR_BFD		(1 << 5)
#define MACFFR_PAM		(1 << 4)
#define MACFFR_DAIF		(1 << 3)
#define MACFFR_HM		(1 << 2)
#define MACFFR_HU		(1 << 1)
#define MACFFR_PM		(1 << 0)

#define MACMIIAR_PA_shift	11
#define MACMIIAR_MR_shift	6
#define MACMIIAR_CR_shift	2
#define MACMIIAR_MW		(1 << 1)
#define MACMIIAR_MB		(1 << 0)

// 0b100: HCLK/102 for RMII REF CLK
#define MACMIIAR_CR		(4 << MACMIIAR_CR_shift)

#define MACSR_TSTS		(1 << 9)
#define MACSR_MMCTS		(1 << 6)
#define MACSR_MMCRS		(1 << 5)
#define MACSR_MMCS		(1 << 4)
#define MACSR_PMTS		(1 << 3)

#define MACIMR_TSTIM		(1 << 9)
#define MACIMR_PMTIM		(1 << 3)

// MAC DMA registers

#define DMABMR_MB		(1 << 26)
#define DMABMR_AAB		(1 << 25)
#define DMABMR_FPM		(1 << 24)
#define DMABMR_USP		(1 << 23)
#define DMABMR_RDP_shift	17
#define DMABMR_FB		(1 << 16)
#define DMABMR_PM_shift		14
#define DMABMR_PBL_shift	8
#define DMABMR_EDFE		(1 << 7)
#define DMABMR_DSL_shift	2
#define DMABMR_DA		(1 << 1)
#define DMABMR_SR		(1 << 0)

#define DMASR_TSTS		(1 << 29)
#define DMASR_PMTS		(1 << 28)
#define DMASR_MMCS		(1 << 27)
#define DMASR_EBS_shift		23
#define DMASR_TPS_shift		20
#define DMASR_RPS_shift		17
#define DMASR_NIS		(1 << 16)
#define DMASR_AIS		(1 << 15)
#define DMASR_ERS		(1 << 14)
#define DMASR_FBES		(1 << 13)
#define DMASR_ETS		(1 << 10)
#define DMASR_RWTS		(1 << 9)
#define DMASR_RPSS		(1 << 8)
#define DMASR_RBUS		(1 << 7)
#define DMASR_RS		(1 << 6)
#define DMASR_TUS		(1 << 5)
#define DMASR_ROS		(1 << 4)
#define DMASR_TJTS		(1 << 3)
#define DMASR_TBUS		(1 << 2)
#define DMASR_TPSS		(1 << 1)
#define DMASR_TS		(1 << 0)

#define DMAOMR_DTCEFD		(1 << 26)
#define DMAOMR_RSF		(1 << 25)
#define DMAOMR_DFRF		(1 << 24)
#define DMAOMR_TSF		(1 << 21)
#define DMAOMR_FTF		(1 << 20)
#define DMAOMR_TTC_shift	14
#define DMAOMR_ST		(1 << 13)
#define DMAOMR_FEF		(1 << 7)
#define DMAOMR_FUGF		(1 << 6)
#define DMAOMR_RTC		3
#define DMAOMR_OSC		(1 << 2)
#define DMAOMR_SR		(1 << 1)

#define DMAIER_NISE		(1 << 16)
#define DMAIER_AISE		(1 << 15)
#define DMAIER_ERIE		(1 << 14)
#define DMAIER_FBEIE		(1 << 13)
#define DMAIER_ETIE		(1 << 10)
#define DMAIER_RWTIE		(1 << 9)
#define DMAIER_RPSIE		(1 << 8)
#define DMAIER_RBUIE		(1 << 7)
#define DMAIER_RIE		(1 << 6)
#define DMAIER_TUIE		(1 << 5)
#define DMAIER_ROIE		(1 << 4)
#define DMAIER_TJTIE		(1 << 3)
#define DMAIER_TBUIE		(1 << 2)
#define DMAIER_TPSIE		(1 << 1)
#define DMAIER_TIE		(1 << 0)


// Descriptors

struct tdes
{
  volatile uint32_t tdes0;
  volatile uint32_t tdes1;
  volatile uint32_t tdes2;
  volatile uint32_t tdes3;
};

#define TDES0_OWN		(1 << 31)
#define TDES0_IC		(1 << 30)
#define TDES0_LS		(1 << 29)
#define TDES0_FS		(1 << 28)
#define TDES0_DC		(1 << 27)
#define TDES0_DP		(1 << 26)
#define TDES0_TTSE		(1 << 25)
#define TDES0_CIC_shift		22
#define TDES0_TER		(1 << 21)
#define TDES0_TCH		(1 << 20)
#define TDES0_TTSS		(1 << 17)
#define TDES0_STATUS_mask	0x1ffff
#define TDES1_COUNT2_shift	16
#define TDES1_COUNT1_shift	0

struct rdes
{
  volatile uint32_t rdes0;
  volatile uint32_t rdes1;
  volatile uint32_t rdes2;
  volatile uint32_t rdes3;
};

#define RDES0_OWN		(1 << 31)
#define RDES0_FL_shift		16
#define RDES1_DIC		(1 << 31)
#define RDES1_PER		(1 << 15)
#define RDES1_RCH		(1 << 14)

// MII registers

#define MII_BMCR		0x00
#define MII_BMSR		0x01
#define MII_PHYSID1		0x02
#define MII_PHYSID2		0x03
#define MII_ADVERTISE		0x04
#define MII_LPA			0x05

#define BMCR_RESET		(1 << 15)
#define BMCR_SPEED100		(1 << 13)
#define BMCR_ANENABLE		(1 << 12)
#define BMCR_PDOWN		(1 << 11)
#define BMCR_FULLDPLX		(1 << 8)

#define BMSR_ANEGCOMPLETE	(1 << 5)
#define BMSR_RFAULT		(1 << 4)
#define BMSR_LSTATUS		(1 << 2)
#define LPA_100BASE4		(1 << 9)
#define LPA_100FULL		(1 << 8)
#define LPA_100HALF		(1 << 7)
#define LPA_10FULL		(1 << 6)

// PHY(LAN8742A-CZ-TR)

#define PHY_ADDR		0
#define PHY_ID			0x0007C130

//
#define NRD 4
#define NTD 2
static struct rdes eth_rd[NRD] __attribute__ ((section(".eth")));
static struct tdes eth_td[NTD] __attribute__ ((section(".eth")));
static uint32_t eth_rb[NRD][512] __attribute__ ((section(".eth")));
static uint32_t eth_tb[NTD][512] __attribute__ ((section(".eth")));

#define ETH_BUFSIZE 1522

extern "C" {

extern char __eth_start__[];
  
// ethrenet MAC routines.

static uint32_t mii_read (uint32_t reg)
{
  ETH->MACMIIAR = ((PHY_ADDR << 11) | (reg << 6) | MACMIIAR_CR
		   | MACMIIAR_MB);
  while ((ETH->MACMIIAR & MACMIIAR_MB) != 0)
    ;
  return ETH->MACMIIDR;
}

static void __attribute__ ((unused)) mii_write (uint32_t reg, uint32_t value) 
{
  ETH->MACMIIDR = value;
  ETH->MACMIIAR = ((PHY_ADDR << 11) | (reg << 6) | MACMIIAR_CR
		   | MACMIIAR_MW | MACMIIAR_MB);
  while ((ETH->MACMIIAR & MACMIIAR_MB) != 0)
    ;
}

void
mac_init (void)
{
  // Select RMII
  SYSCFG->PMC |= (1 << 23);

  //  RCC->AHB1RSTR = RCC_AHB1RSTR_ETHMACRST;
  //  RCC->APB1RSTR = 0;

  // Enable ETHMAC clock
  RCC->AHB1ENR |= (RCC_AHB1ENR_ETHMACEN
		   | RCC_AHB1ENR_ETHMACTXEN
		   | RCC_AHB1ENR_ETHMACRXEN);

  // Read PHY(LAN8742A) ID
  ETH->MACMIIDR = (PHY_ADDR << 6) | MACMIIAR_CR;
  uint32_t sid1 = mii_read (MII_PHYSID1);
  uint32_t sid2 = mii_read (MII_PHYSID2);
  if (sid1 != (PHY_ID >> 16)
      || (sid2 & 0xfff0) != (PHY_ID & 0xfff0))
    {
      // Fail to find PHY
      while (1)
	;
    }

  // Powerup PHY
  // mii_write (MII_BMCR, mii_read (MII_BMCR) & ~BMCR_PDOWN);

  // Initialize ETHMAC DMA
  // Reset DMA
  ETH->DMABMR |= DMABMR_SR;
  while(ETH->DMABMR & DMABMR_SR)
    ;

  // ETH->DMABMR = bus access parameter;
  ETH->DMABMR = DMABMR_AAB | (1 << DMABMR_RDP_shift) | (1 << DMABMR_PBL_shift);
  // ETH->DMAIER = necessary interrupts;
  ETH->DMASR = ETH->DMASR;
  ETH->DMAIER = 0;

  // Create desc. lists
  memset (__eth_start__, 0, 16 * 1024);

  for (int i = 0; i < NRD; i++)
    {
      eth_rd[i].rdes0 = RDES0_OWN;
      eth_rd[i].rdes1 = RDES1_RCH | ETH_BUFSIZE;
      eth_rd[i].rdes2 = reinterpret_cast<uint32_t>(&eth_rb[i][0]);
      eth_rd[i].rdes3 = reinterpret_cast<uint32_t>(&eth_rd[(i+1)%NRD]);
    }

  for (int i = 0; i < NTD; i++)
    {
      eth_td[i].tdes0 = TDES0_TCH;
      eth_td[i].tdes1 = 0;
      eth_td[i].tdes2 = reinterpret_cast<uint32_t>(&eth_tb[i][0]);
      eth_td[i].tdes3 = reinterpret_cast<uint32_t>(&eth_td[(i+1)%NTD]);
    }

  ETH->DMARDLAR = reinterpret_cast<uint32_t>(&eth_rd[0]);
  ETH->DMATDLAR = reinterpret_cast<uint32_t>(&eth_td[0]);

  // Write MAC register 1,2,3 to choose filtering options
  ETH->MACFFR = 0;
  ETH->MACHTHR = 0;
  ETH->MACHTLR = 0;
  ETH->MACFCR = 0;
  ETH->MACVLANTR = 0;

  // Set MAC address
  // default aa:55:13:37:01:10
  ETH->MACA0HR = 0x00001001;
  ETH->MACA0LR = 0x371355aa;
  ETH->MACA1HR = 0x0000ffff;
  ETH->MACA1LR = 0xffffffff;
  ETH->MACA2HR = 0x0000ffff;
  ETH->MACA2LR = 0xffffffff;
  ETH->MACA3HR = 0x0000ffff;
  ETH->MACA3LR = 0xffffffff;

  // ETH->MACCR = configure&enabel transmits;
#if 0
  // Reset PHY
  mii_write (MII_BMCR, BMCR_RESET);
  while (mii_read (MII_BMCR) & BMCR_RESET)
    ;
#endif

  uint32_t maccr, bmsr, bmcr;
  maccr = ETH->MACCR;
  (void) mii_read (MII_BMSR);
  bmsr = mii_read (MII_BMSR);
  bmcr = mii_read (MII_BMCR);
  if (bmcr & BMCR_ANENABLE)
    {
      uint32_t lpa;

      while ((bmsr & (BMSR_LSTATUS | BMSR_RFAULT | BMSR_ANEGCOMPLETE))
	     != (BMSR_LSTATUS | BMSR_ANEGCOMPLETE))
	bmsr = mii_read (MII_BMSR);

      lpa = mii_read (MII_LPA);
      if (lpa & (LPA_100HALF | LPA_100FULL | LPA_100BASE4))
	maccr |= MACCR_FES;
      else
	maccr &= ~MACCR_FES;

    if (lpa & (LPA_10FULL | LPA_100FULL))
      maccr |= MACCR_DM;
    else
      maccr &= ~MACCR_DM;
    }
  else
    {
      while (!(bmsr & BMSR_LSTATUS))
	bmsr = mii_read (MII_BMSR);

      if (bmcr & BMCR_SPEED100)
	maccr |= MACCR_FES;
      else
	maccr &= ~MACCR_FES;

      if (bmcr & BMCR_FULLDPLX)
	maccr |= MACCR_DM;
      else
	maccr &= ~MACCR_DM;
    }

  ETH->MACCR = maccr | MACCR_RE | MACCR_TE;

  // Flush transmit fifo
  ETH->DMAOMR = DMAOMR_FTF;
  while (ETH->DMAOMR & DMAOMR_FTF)
    ;

  ETH->DMAOMR = DMAOMR_DTCEFD | DMAOMR_RSF | DMAOMR_TSF | DMAOMR_ST | DMAOMR_SR;

#if 0
  while (eth_rd[0].rdes0 & RDES0_OWN)
    ;

  (void) ETH->DMASR;
#endif
}

int
get_free_tdes_index (void)
{
  int i = 0;
  while (1)
    {
      if (!(eth_td[i].tdes0 & TDES0_OWN))
	break;
      i = (i+1) % NTD;
      if (((ETH->DMASR >> DMASR_TPS_shift) & 7) == 6)
	{
	  ETH->DMASR = DMASR_TBUS;
	  ETH->DMATPDR = DMASR_TBUS;
	}
    }
  return i;
}

void *
get_tdes_buffer (int idx)
{
  return reinterpret_cast<void *>(eth_td[idx].tdes2);
}

void
release_tdes (int idx, uint32_t ofs)
{
  eth_td[idx].tdes1 = (ofs - eth_td[idx].tdes2) & 0x1fff;
  eth_td[idx].tdes0 = TDES0_OWN | TDES0_FS | TDES0_LS | TDES0_TCH;
  if (((ETH->DMASR >> DMASR_TPS_shift) & 7) == 6)
    {
      ETH->DMASR = DMASR_TBUS;
      ETH->DMATPDR = DMASR_TBUS;
    }
}

int
get_rcvd_rdes_index (void)
{
  static int i = 0;
  while (1)
    {
      if (!(eth_rd[i].rdes0 & RDES0_OWN))
	break;
      i = (i+1) % NRD;
    }
  return i;
}

uint16_t
get_rcvd_rdes_len (int idx)
{
  return (eth_rd[idx].rdes0 >> RDES0_FL_shift) & 0x1fff;
}
  
void *
get_rdes_buffer (int idx)
{
  return reinterpret_cast<void *>(eth_rd[idx].rdes2);
}

void
release_rdes (int idx)
{
  eth_rd[idx].rdes0 |= RDES0_OWN;
}

static int
num_rcvd_rdes (void)
{
  int count = 0;
  for (int i = 0; i < NRD; i++)
    {
      if (!(eth_rd[i].rdes0 & RDES0_OWN))
	count++;
    }
  return count;
}

} // extern "C"

#include "lwip/ip.h"
#include "lwip/init.h"

extern "C" {
  extern err_t ethernetif_init(struct netif *netif);
  extern void ethernetif_input(struct netif *netif) ;
  extern err_t ethernet_input(struct pbuf *p, struct netif *netif);
}

void
lwip_thread (void *arg __attribute__ ((unused)))
{
  ip4_addr_t ip, gateway, netmask;
  static struct netif thisif;

  lwip_init ();

  mac_init ();

  IP4_ADDR (&ip, 10, 253, 253, 81);
  IP4_ADDR (&netmask, 255, 255, 255, 0);
  IP4_ADDR (&gateway, 10, 253, 253, 1);
  netif_add(&thisif, &ip, &netmask, &gateway, NULL,
	    ethernetif_init, ethernet_input);
  netif_set_default(&thisif);
  netif_set_up(&thisif);

  // Handle input packets
  id_t irq_id = eventflag::irq_event (INTR_REQ_ETH);
  ETH->DMAIER = DMAIER_NISE | DMAIER_RIE;
  while (1)
    {
      while (num_rcvd_rdes () == 0)
	{
	  bitset flags;
	  flags.clear ();
	  thread::poll_section ();
	  flags.add (irq_id);
	  thread::poll (flags);
	  ETH->DMASR = ETH->DMASR;
	}
      ethernetif_input(&thisif);
    }
}
