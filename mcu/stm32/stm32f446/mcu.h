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

/* Simple STM32F446 definitions.  Not complete. */

/* Cotex-M4 system controll block definition. */

struct SCB
{
  volatile uint32_t CPUID;
  volatile uint32_t ICSR;
  volatile uint32_t VTOR;
  volatile uint32_t AIRCR;
  volatile uint32_t SCR;
  volatile uint32_t CCR;
  volatile uint8_t  SHP[12];
  volatile uint32_t SHCSR;
  volatile uint32_t CFSR;
  volatile uint32_t HFSR;
  volatile uint32_t DFSR;
  volatile uint32_t MMFAR;
  volatile uint32_t BFAR;
  volatile uint32_t AFSR;
  volatile uint32_t PFR[2];
  volatile uint32_t DFR;
  volatile uint32_t ADR;
  volatile uint32_t MMFR[4];
  volatile uint32_t ISAR[5];
  uint32_t dummy0[5];
  volatile uint32_t CPACR;
};

#define SCS_BASE	(0xE000E000)
#define SCB_BASE	(SCS_BASE +  0x0D00)
static struct SCB *const SCB = ((struct SCB *const) SCB_BASE);

struct FPU
{
  uint32_t dummy0[1];
  volatile uint32_t FPCCR;
  volatile uint32_t FPCAR;
  volatile uint32_t FPDSCR;
  volatile uint32_t MVFR0;
  volatile uint32_t MVFR1;
};

#define FPU_BASE	(SCS_BASE + 0x0F30)
static struct FPU *const FPU = ((struct FPU *const) FPU_BASE);

/* Some STM32F446 peripheral definitions.  */

#define PERIPH_BASE		0x40000000
#define APB1PERIPH_BASE  	PERIPH_BASE
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x10000)
#define AHB1PERIPH_BASE		(PERIPH_BASE + 0x20000)
#define AHB2PERIPH_BASE		(PERIPH_BASE + 0x10000000)

/* RCC */
struct RCC {
  volatile uint32_t CR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t AHB1RSTR;
  volatile uint32_t AHB2RSTR;
  volatile uint32_t AHB3RSTR;
  uint32_t dummy0[1];
  volatile uint32_t APB1RSTR;
  volatile uint32_t APB2RSTR;
  uint32_t dummy1[2];
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  volatile uint32_t AHB3ENR;
  uint32_t dummy2[1];
  volatile uint32_t APB1ENR;
  volatile uint32_t APB2ENR;
  uint32_t dummy3[2];
  volatile uint32_t AHB1LPENR;
  volatile uint32_t AHB2LPENR;
  volatile uint32_t AHB3LPENR;
  uint32_t dummy4[1];
  volatile uint32_t APB1LPENR;
  volatile uint32_t APB2LPENR;
  uint32_t dummy5[2];
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
  uint32_t dummy6[2];
  volatile uint32_t SSCGR;
  volatile uint32_t PLLI2SCFGR;
  volatile uint32_t PLLSAICFGR;
  volatile uint32_t DCKCFGR;
  volatile uint32_t CKGATENR;
  volatile uint32_t DCKCFGR2;
};

#define RCC_CR_HSION		0x00000001
#define RCC_CR_HSIRDY		0x00000002
#define RCC_CR_HSITRIM		0x000000F8
#define RCC_CR_HSEON		0x00010000
#define RCC_CR_HSERDY		0x00020000
#define RCC_CR_HSEBYP		0x00040000
#define RCC_CR_CSSON		0x00080000
#define RCC_CR_PLLON		0x01000000
#define RCC_CR_PLLRDY		0x02000000
#define RCC_CR_PLLI2SON		0x04000000
#define RCC_CR_PLLI2SRDY	0x08000000
#define RCC_CR_PLLSAION		0x10000000
#define RCC_CR_PLLSAIRDY	0x20000000

#define RCC_PLLCFGR_PLLM	0x0000003F
#define RCC_PLLCFGR_PLLN	0x00007FC0
#define RCC_PLLCFGR_PLLP	0x00030000
#define RCC_PLLCFGR_PLLSRC	0x00400000
#define RCC_PLLCFGR_PLLSRC_HSE	0x00400000
#define RCC_PLLCFGR_PLLQ	0x0F000000
#define RCC_PLLCFGR_PLLR	0x70000000

#define RCC_CFGR_SW		0x00000003
#define RCC_CFGR_SW_PLL		0x00000002
#define RCC_CFGR_SWS		0x0000000C
#define RCC_CFGR_SWS_HSI	0x00000000
#define RCC_CFGR_HPRE		0x000000F0
#define RCC_CFGR_PPRE1		0x00001C00
#define RCC_CFGR_PPRE1_DIV4	0x00001400
#define RCC_CFGR_PPRE2		0x0000E000
#define RCC_CFGR_PPRE2_DIV2	0x00008000
#define RCC_CFGR_RTCPRE		0x001F0000
#define RCC_CFGR_MCO1		0x00600000
#define RCC_CFGR_I2SSRC		0x00800000
#define RCC_CFGR_MCO1PRE	0x07000000
#define RCC_CFGR_MCO2PRE	0x38000000
#define RCC_CFGR_MCO2		0xC0000000

#define RCC_AHB1RSTR_GPIOARST	0x00000001
#define RCC_AHB1RSTR_GPIOBRST	0x00000002
#define RCC_AHB1RSTR_GPIOCRST	0x00000004
#define RCC_AHB1RSTR_GPIODRST	0x00000008
#define RCC_AHB1RSTR_GPIOERST	0x00000010
#define RCC_AHB1RSTR_GPIOFRST	0x00000020
#define RCC_AHB1RSTR_GPIOGRST	0x00000040
#define RCC_AHB1RSTR_GPIOHRST	0x00000080
#define RCC_AHB1RSTR_CRCRST	0x00001000
#define RCC_AHB1RSTR_DMA1RST	0x00200000
#define RCC_AHB1RSTR_DMA2RST	0x00400000
#define RCC_AHB1RSTR_OTGHRST	0x20000000

#define RCC_APB1RSTR_TIM2RST	0x00000001
#define RCC_APB1RSTR_TIM3RST	0x00000002
#define RCC_APB1RSTR_TIM4RST	0x00000004
#define RCC_APB1RSTR_TIM5RST	0x00000008
#define RCC_APB1RSTR_TIM6RST	0x00000010
#define RCC_APB1RSTR_TIM7RST	0x00000020
#define RCC_APB1RSTR_TIM12RST	0x00000040
#define RCC_APB1RSTR_TIM13RST	0x00000080
#define RCC_APB1RSTR_TIM14RST	0x00000100
#define RCC_APB1RSTR_WWDGRST	0x00000800
#define RCC_APB1RSTR_SPI2RST	0x00004000
#define RCC_APB1RSTR_SPI3RST	0x00008000
#define RCC_APB1RSTR_SPDIFRXRST	0x00010000
#define RCC_APB1RSTR_USART2RST	0x00020000
#define RCC_APB1RSTR_USART3RST	0x00040000
#define RCC_APB1RSTR_UART4RST	0x00080000
#define RCC_APB1RSTR_UART5RST	0x00100000
#define RCC_APB1RSTR_I2C1RST	0x00200000
#define RCC_APB1RSTR_I2C2RST	0x00400000
#define RCC_APB1RSTR_I2C3RST	0x00800000
#define RCC_APB1RSTR_FMPI2C1RST	0x01000000
#define RCC_APB1RSTR_CAN1RST	0x02000000
#define RCC_APB1RSTR_CAN2RST	0x04000000
#define RCC_APB1RSTR_CECRST	0x08000000
#define RCC_APB1RSTR_PWRRST	0x10000000
#define RCC_APB1RSTR_DACRST	0x20000000

#define RCC_APB2RSTR_TIM1RST	0x00000001
#define RCC_APB2RSTR_TIM8RST	0x00000002
#define RCC_APB2RSTR_USART1RST	0x00000010
#define RCC_APB2RSTR_USART6RST	0x00000020
#define RCC_APB2RSTR_ADCRST	0x00000100
#define RCC_APB2RSTR_SDIORST	0x00000800
#define RCC_APB2RSTR_SPI1RST	0x00001000
#define RCC_APB2RSTR_SPI4RST	0x00002000
#define RCC_APB2RSTR_SYSCFGRST	0x00004000
#define RCC_APB2RSTR_TIM9RST	0x00010000
#define RCC_APB2RSTR_TIM10RST	0x00020000
#define RCC_APB2RSTR_TIM11RST	0x00040000
#define RCC_APB2RSTR_SAI1RST	0x00400000
#define RCC_APB2RSTR_SAI2RST	0x00800000
#define RCC_AHB1ENR_GPIOAEN	0x00000001
#define RCC_AHB1ENR_GPIOBEN	0x00000002
#define RCC_AHB1ENR_GPIOCEN	0x00000004
#define RCC_AHB1ENR_GPIODEN	0x00000008
#define RCC_AHB1ENR_GPIOEEN	0x00000010
#define RCC_AHB1ENR_GPIOFEN	0x00000020
#define RCC_AHB1ENR_GPIOGEN	0x00000040
#define RCC_AHB1ENR_GPIOHEN	0x00000080

#define RCC_AHB1ENR_CRCEN	0x00001000
#define RCC_AHB1ENR_BKPSRAMEN	0x00040000
#define RCC_AHB1ENR_DMA1EN	0x00200000
#define RCC_AHB1ENR_DMA2EN	0x00400000

#define RCC_AHB1ENR_OTGHSEN	0x20000000
#define RCC_AHB1ENR_OTGHSULPIEN	0x40000000
#define RCC_APB1ENR_TIM2EN	0x00000001
#define RCC_APB1ENR_TIM3EN	0x00000002
#define RCC_APB1ENR_TIM4EN	0x00000004
#define RCC_APB1ENR_TIM5EN	0x00000008
#define RCC_APB1ENR_TIM6EN	0x00000010
#define RCC_APB1ENR_TIM7EN	0x00000020
#define RCC_APB1ENR_TIM12EN	0x00000040
#define RCC_APB1ENR_TIM13EN	0x00000080
#define RCC_APB1ENR_TIM14EN	0x00000100
#define RCC_APB1ENR_WWDGEN	0x00000800
#define RCC_APB1ENR_SPI2EN	0x00004000
#define RCC_APB1ENR_SPI3EN	0x00008000
#define RCC_APB1ENR_SPDIFRXEN	0x00010000
#define RCC_APB1ENR_USART2EN	0x00020000
#define RCC_APB1ENR_USART3EN	0x00040000
#define RCC_APB1ENR_UART4EN	0x00080000
#define RCC_APB1ENR_UART5EN	0x00100000
#define RCC_APB1ENR_I2C1EN	0x00200000
#define RCC_APB1ENR_I2C2EN	0x00400000
#define RCC_APB1ENR_I2C3EN	0x00800000
#define RCC_APB1ENR_FMPI2C1EN	0x01000000
#define RCC_APB1ENR_CAN1EN	0x02000000
#define RCC_APB1ENR_CAN2EN	0x04000000
#define RCC_APB1ENR_CECEN	0x08000000
#define RCC_APB1ENR_PWREN	0x10000000
#define RCC_APB1ENR_DACEN	0x20000000

#define RCC_APB2ENR_TIM1EN	0x00000001
#define RCC_APB2ENR_TIM8EN	0x00000002
#define RCC_APB2ENR_USART1EN	0x00000010
#define RCC_APB2ENR_USART6EN	0x00000020
#define RCC_APB2ENR_ADC1EN	0x00000100
#define RCC_APB2ENR_ADC2EN	0x00000200
#define RCC_APB2ENR_ADC3EN	0x00000400
#define RCC_APB2ENR_SDIOEN	0x00000800
#define RCC_APB2ENR_SPI1EN	0x00001000
#define RCC_APB2ENR_SPI4EN	0x00002000
#define RCC_APB2ENR_SYSCFGEN	0x00004000
#define RCC_APB2ENR_TIM9EN	0x00010000
#define RCC_APB2ENR_TIM10EN	0x00020000
#define RCC_APB2ENR_TIM11EN	0x00040000
#define RCC_APB2ENR_SAI1EN	0x00400000
#define RCC_APB2ENR_SAI2EN	0x00800000

/* SYSCFG */
struct SYSCFG {
  volatile uint32_t MEMRMP;
  volatile uint32_t PMC;
  volatile uint32_t EXTICR[4];
  uint32_t dummy0[2];
  volatile uint32_t CMPCR;
  uint32_t dummy1[2];
  volatile uint32_t CFGR;
};

#define SYSCFG_MEMRMP_MEM_MODE 0x07

/* FLASH */
struct FLASH {
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
};

#define FLASH_ACR_LATENCY_5WS	0x00000005
#define FLASH_ACR_PRFTEN	0x00000100
#define FLASH_ACR_ICEN		0x00000200
#define FLASH_ACR_DCEN		0x00000400

/* GPIO */
struct GPIO {
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
};

/* Timer */
struct TIM
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SMCR;
  volatile uint32_t DIER;
  volatile uint32_t SR;
  volatile uint32_t EGR;
  volatile uint32_t CCMR1;
  volatile uint32_t CCMR2;
  volatile uint32_t CCER;
  volatile uint32_t CNT;
  volatile uint32_t PSC;
  volatile uint32_t ARR;
  volatile uint32_t RCR;
  volatile uint32_t CCR1;
  volatile uint32_t CCR2;
  volatile uint32_t CCR3;
  volatile uint32_t CCR4;
  volatile uint32_t BDTR;
  volatile uint32_t DCR;
  volatile uint32_t DMAR;
  volatile uint32_t OR;
};

/* USART */
struct USART
{
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t BRR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
  volatile uint32_t GTPR;
};

/* bxCAN */
struct TXMB
{
  volatile uint32_t TIR;
  volatile uint32_t TDTR;
  volatile uint32_t TDLR;
  volatile uint32_t TDHR;
};

struct RXMB
{
  volatile uint32_t RIR;
  volatile uint32_t RDTR;
  volatile uint32_t RDLR;
  volatile uint32_t RDHR;
};

struct FILT
{
  volatile uint32_t FR1;
  volatile uint32_t FR2;
};

struct CAN
{
  volatile uint32_t MCR;
  volatile uint32_t MSR;
  volatile uint32_t TSR;
  volatile uint32_t RF0R;
  volatile uint32_t RF1R;
  volatile uint32_t IER;
  volatile uint32_t ESR;
  volatile uint32_t BTR;
  uint32_t dummy0[88];
  struct TXMB TXMB[3];
  struct RXMB RXMB[2];
  uint32_t dummy1[12];
  volatile uint32_t FMR;
  volatile uint32_t FM1R;
  uint32_t dummy2;
  volatile uint32_t FS1R;
  uint32_t dummy3;
  volatile uint32_t FFA1R;
  uint32_t dummy4;
  volatile uint32_t FA1R;
  uint32_t dummy5[8];
  struct FILT BANK[28];
};

/* SPI/I2S */
struct SPI
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t CRCPR;
  volatile uint32_t RXCRCR;
  volatile uint32_t TXCRCR;
  volatile uint32_t I2SCFGR;
  volatile uint32_t I2SPR;
};

/* I2C */

struct I2C
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t OAR1;
  volatile uint32_t OAR2;
  volatile uint32_t DR;
  volatile uint32_t SR1;
  volatile uint32_t SR2;
  volatile uint32_t CCR;
  volatile uint32_t TRISE;
  volatile uint32_t FLTR;
};

/* ADC */

struct ADC
{
  volatile uint32_t SR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SMPR1;
  volatile uint32_t SMPR2;
  volatile uint32_t JOFR1;
  volatile uint32_t JOFR2;
  volatile uint32_t JOFR3;
  volatile uint32_t JOFR4;
  volatile uint32_t HTR;
  volatile uint32_t LTR;
  volatile uint32_t SQR1;
  volatile uint32_t SQR2;
  volatile uint32_t SQR3;
  volatile uint32_t JSQR;
  volatile uint32_t JDR1;
  volatile uint32_t JDR2;
  volatile uint32_t JDR3;
  volatile uint32_t JDR4;
  volatile uint32_t DR;
};

/* common ADC registers */

struct ADCC
{
  volatile uint32_t CSR;
  volatile uint32_t CCR;
  volatile uint32_t CDR;
};

/* DMA */

struct DMA
{
  volatile uint32_t LISR;
  volatile uint32_t HISR;
  volatile uint32_t LIFCR;
  volatile uint32_t HIFCR;
};

#define DMA_TCIF3		(1 << 27)
#define DMA_HTIF3		(1 << 26)
#define DMA_TEIF3		(1 << 25)
#define DMA_DMEIF3		(1 << 24)
#define DMA_FEIF3		(1 << 22)
#define DMA_TCIF2		(1 << 21)
#define DMA_HTIF2		(1 << 20)
#define DMA_TEIF2		(1 << 19)
#define DMA_DMEIF2		(1 << 18)
#define DMA_FEIF2		(1 << 16)
#define DMA_TCIF1		(1 << 11)
#define DMA_HTIF1		(1 << 10)
#define DMA_TEIF1		(1 << 9)
#define DMA_DMEIF1		(1 << 8)
#define DMA_FEIF1		(1 << 6)
#define DMA_TCIF0		(1 << 5)
#define DMA_HTIF0		(1 << 4)
#define DMA_TEIF0		(1 << 3)
#define DMA_DMEIF0		(1 << 2)
#define DMA_FEIF0		(1 << 0)

#define DMA_TCIF7		(1 << 27)
#define DMA_HTIF7		(1 << 26)
#define DMA_TEIF7		(1 << 25)
#define DMA_DMEIF7		(1 << 24)
#define DMA_FEIF7		(1 << 22)
#define DMA_TCIF6		(1 << 21)
#define DMA_HTIF6		(1 << 20)
#define DMA_TEIF6		(1 << 19)
#define DMA_DMEIF6		(1 << 18)
#define DMA_FEIF6		(1 << 16)
#define DMA_TCIF5		(1 << 11)
#define DMA_HTIF5		(1 << 10)
#define DMA_TEIF5		(1 << 9)
#define DMA_DMEIF5		(1 << 8)
#define DMA_FEIF5		(1 << 6)
#define DMA_TCIF4		(1 << 5)
#define DMA_HTIF4		(1 << 4)
#define DMA_TEIF4		(1 << 3)
#define DMA_DMEIF4		(1 << 2)
#define DMA_FEIF4		(1 << 0)

/* DMA stream */
struct DMA_Stream
{
  volatile uint32_t CR;
  volatile uint32_t NDTR;
  volatile uint32_t PAR;
  volatile uint32_t M0AR;
  volatile uint32_t M1AR;
  volatile uint32_t FCR;
};

#define DMA_SxCR_CHSEL_shift	25
#define DMA_SxCR_MBURST_shift	23
#define DMA_SxCR_PBURST_shift	21
#define DMA_SxCR_BURST_SINGLE	0
#define DMA_SxCR_BURST_INCR4	1
#define DMA_SxCR_BURST_INCR8	2
#define DMA_SxCR_BURST_INCR16	3
#define DMA_SxCR_CT		(1 << 19)
#define DMA_SxCR_DBM		(1 << 18)
#define DMA_SxCR_PL_shift	16
#define DMA_SxCR_PL_LOW		0
#define DMA_SxCR_PL_MEDIUM	1
#define DMA_SxCR_PL_HIGH	2
#define DMA_SxCR_PL_VHIGH	3
#define DMA_SxCR_PINCOS		(1 << 15)
#define DMA_SxCR_MSIZE_shift	13
#define DMA_SxCR_PSIZE_shift	11
#define DMA_SxCR_SIZE_BYTE	0
#define DMA_SxCR_SIZE_HALF	1
#define DMA_SxCR_SIZE_WORD	2
#define DMA_SxCR_MINC		(1 << 10)
#define DMA_SxCR_PINC		(1 << 9)
#define DMA_SxCR_CIRC		(1 << 8)
#define DMA_SxCR_DIR_shift	6
#define DMA_SxCR_DIR_P2M	0
#define DMA_SxCR_DIR_M2P	1
#define DMA_SxCR_DIR_M2M	2
#define DMA_SxCR_PFCTRL		(1 << 5)
#define DMA_SxCR_TCIE		(1 << 4)
#define DMA_SxCR_HTIE		(1 << 3)
#define DMA_SxCR_TEIE		(1 << 2)
#define DMA_SxCR_DMEIE		(1 << 1)
#define DMA_SxCR_EN		(1 << 0)

#define DMA_SxFCR_FEIE		(1 << 7)
#define DMA_SxFCR_FS_shift	3
#define DMA_SxFCR_FS_1BY4	0
#define DMA_SxFCR_FS_1BY2	1
#define DMA_SxFCR_FS_3BY4	2
#define DMA_SxFCR_FS_ALMOST	3
#define DMA_SxFCR_FS_ENMPT	4
#define DMA_SxFCR_FS_FULL	5
#define DMA_SxFCR_DMDIS		(1 << 2)
#define DMA_SxFCR_FTH_shift	0
#define DMA_SxFCR_FTH_1BY4	0
#define DMA_SxFCR_FTH_1BY2	1
#define DMA_SxFCR_FTH_3BY4	2
#define DMA_SxFCR_FTH_FULL	3

/* Physical addresses */
#define RCC_BASE		(AHB1PERIPH_BASE + 0x3800)
#define SYSCFG_BASE		(APB2PERIPH_BASE + 0x3800)
#define FLASH_R_BASE		(AHB1PERIPH_BASE + 0x3C00)
#define GPIOA_BASE		(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE		(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE		(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE		(AHB1PERIPH_BASE + 0x0C00)
#define TIM2_BASE		(APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE		(APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE		(APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE		(APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE		(APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE		(APB1PERIPH_BASE + 0x1400)
#define USART1_BASE		(APB2PERIPH_BASE + 0x1000)
#define USART2_BASE		(APB1PERIPH_BASE + 0x4400)
#define USART3_BASE		(APB1PERIPH_BASE + 0x4800)
#define UART4_BASE		(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE		(APB1PERIPH_BASE + 0x5000)
#define USART6_BASE		(APB2PERIPH_BASE + 0x1400)
#define CAN1_BASE		(APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE		(APB1PERIPH_BASE + 0x6800)
#define SPI1_BASE		(APB2PERIPH_BASE + 0x3000)
#define SPI2_BASE		(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE		(APB1PERIPH_BASE + 0x3C00)
#define I2C1_BASE		(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE		(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE		(APB1PERIPH_BASE + 0x5C00)
#define ADC1_BASE		(APB2PERIPH_BASE + 0x2000)
#define ADC2_BASE		(APB2PERIPH_BASE + 0x2100)
#define ADC3_BASE		(APB2PERIPH_BASE + 0x2200)
#define ADCC_BASE		(APB2PERIPH_BASE + 0x2300)
#define DMA1_BASE		(AHB1PERIPH_BASE + 0x6000)
#define DMA2_BASE		(AHB1PERIPH_BASE + 0x6400)

static struct RCC *const RCC = ((struct RCC *const)RCC_BASE);
static struct SYSCFG *const SYSCFG = ((struct SYSCFG *const) SYSCFG_BASE);
static struct FLASH *const FLASH = ((struct FLASH *const) FLASH_R_BASE);
static struct GPIO *const GPIOA = ((struct GPIO *const) GPIOA_BASE);
static struct GPIO *const GPIOB = ((struct GPIO *const) GPIOB_BASE);
static struct GPIO *const GPIOC = ((struct GPIO *const) GPIOC_BASE);
static struct GPIO *const GPIOD = ((struct GPIO *const) GPIOD_BASE);
static struct TIM *const TIM2 = ((struct TIM *const) TIM2_BASE);
static struct TIM *const TIM3 = ((struct TIM *const) TIM3_BASE);
static struct TIM *const TIM4 = ((struct TIM *const) TIM4_BASE);
static struct TIM *const TIM5 = ((struct TIM *const) TIM5_BASE);
static struct TIM *const TIM6 = ((struct TIM *const) TIM6_BASE);
static struct TIM *const TIM7 = ((struct TIM *const) TIM7_BASE);
static struct USART *const USART1 = ((struct USART *const) USART1_BASE);
static struct USART *const USART2 = ((struct USART *const) USART2_BASE);
static struct USART *const USART3 = ((struct USART *const) USART3_BASE);
static struct USART *const UART4 = ((struct USART *const) UART4_BASE);
static struct USART *const UART5 = ((struct USART *const) UART5_BASE);
static struct USART *const USART6 = ((struct USART *const) USART6_BASE);
static struct CAN *const CAN1 = ((struct CAN *const) CAN1_BASE);
static struct CAN *const CAN2 = ((struct CAN *const) CAN2_BASE);
static struct SPI *const I2S1 = ((struct SPI *const) SPI1_BASE);
static struct SPI *const I2S2 = ((struct SPI *const) SPI2_BASE);
static struct SPI *const I2S3 = ((struct SPI *const) SPI3_BASE);
static struct I2C *const I2C1 = ((struct I2C *const) I2C1_BASE);
static struct I2C *const I2C2 = ((struct I2C *const) I2C2_BASE);
static struct I2C *const I2C3 = ((struct I2C *const) I2C3_BASE);
static struct ADC *const ADC1 = ((struct ADC *const) ADC1_BASE);
static struct ADC *const ADC2 = ((struct ADC *const) ADC2_BASE);
static struct ADC *const ADC3 = ((struct ADC *const) ADC3_BASE);
static struct ADCC *const ADCC = ((struct ADCC *const) ADCC_BASE);
static struct DMA *const DMA1 = ((struct DMA *const) DMA1_BASE);
static struct DMA *const DMA2 = ((struct DMA *const) DMA2_BASE);
static struct DMA_Stream *const DMA1_Stream __attribute__((unused)) =
  ((struct DMA_Stream *const) (DMA1_BASE + sizeof (struct DMA)));
static struct DMA_Stream *const DMA2_Stream __attribute__((unused)) =
  ((struct DMA_Stream *const) (DMA2_BASE + sizeof (struct DMA)));
