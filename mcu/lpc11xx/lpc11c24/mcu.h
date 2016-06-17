/* Simplified lpc11xx.h */

struct SYSCON
{
  volatile uint32_t SYSMEMREMAP;
  volatile uint32_t PRESETCTRL;
  volatile uint32_t SYSPLLCTRL;
  volatile uint32_t SYSPLLSTAT;
  volatile uint32_t USBPLLCTRL;
  volatile uint32_t USBPLLSTAT;
  uint32_t dummy0[2];
  volatile uint32_t SYSOSCCTRL;
  volatile uint32_t WDTOSCCTRL;
  volatile uint32_t IRCCTRL;
  volatile uint32_t LFOSCCTRL;
  volatile uint32_t SYSRSTSTAT;
  uint32_t dummy2[3];
  volatile uint32_t SYSPLLCLKSEL;
  volatile uint32_t SYSPLLCLKUEN;
  volatile uint32_t USBPLLCLKSEL;
  volatile uint32_t USBPLLCLKUEN;
  uint32_t dummy3[8];
  volatile uint32_t MAINCLKSEL;
  volatile uint32_t MAINCLKUEN;
  volatile uint32_t SYSAHBCLKDIV;
  uint32_t dummy4[1];
  volatile uint32_t SYSAHBCLKCTRL;
  uint32_t dummy5[4];
  volatile uint32_t SSP0CLKDIV;
  volatile uint32_t UARTCLKDIV;
  volatile uint32_t SSP1CLKDIV;
  uint32_t dummy6[8];
  volatile uint32_t USBCLKSEL;
  volatile uint32_t USBCLKUEN;
  volatile uint32_t USBCLKDIV;
  uint32_t dummy7[1];
  volatile uint32_t WDTCLKSEL;
  volatile uint32_t WDTCLKUEN;
  volatile uint32_t WDTCLKDIV;
  uint32_t dummy8[1];
  volatile uint32_t CLKOUTSEL;
  volatile uint32_t CLKOUTUEN;
  volatile uint32_t CLKOUTDIV;
  uint32_t dummy9[5];
  volatile uint32_t PIOPORCAP[2];
  uint32_t dummy10[18];
  volatile uint32_t BODCTRL;
  volatile uint32_t SYSTCKCAL;
  uint32_t dummy11[6];
  volatile uint32_t IRQLATENCY;
  volatile uint32_t NMISRC;
  volatile uint32_t PINTSEL[8];
  volatile uint32_t USBCLKCTRL;
  volatile uint32_t USBCLKST;
  uint32_t dummy12[24];
  volatile uint32_t STARTAPRP0;
  volatile uint32_t STARTERP0;
  volatile uint32_t STARTRSRP0CLR;
  volatile uint32_t STARTSRP0;
  uint32_t dummy13[1];
  volatile uint32_t STARTERP1;
  uint32_t dummy14[6];
  volatile uint32_t PDSLEEPCFG;
  volatile uint32_t PDAWAKECFG;
  volatile uint32_t PDRUNCFG;
  uint32_t dummy15[110];
  volatile uint32_t DEVICE_ID;
};

struct IOCON
{
  volatile uint32_t PIO0[24];
  volatile uint32_t PIO1[32];
};

struct FLASHCTRL
{
  uint32_t dummy0[4];
  volatile uint32_t FLASHTIM;
  uint32_t dummy1[3];
  volatile uint32_t FMSSTART;
  volatile uint32_t FMSSTOP;
  uint32_t dummy2[1];
  volatile uint32_t FMSW[4];
  uint32_t dummy3[25];
  volatile uint32_t EEMSSTART;
  volatile uint32_t EEMSSTOP;
  volatile uint32_t EEMSSIG;
  uint32_t dummy4[974];
  volatile uint32_t FMSTAT;
  uint32_t dummy5[1];
  volatile uint32_t FMSTATCLR;
};

struct PMU
{
  volatile uint32_t PCON;
  volatile uint32_t GPREG[4];
};

struct WDT
{
  volatile uint32_t WDMOD;
  volatile uint32_t WDTC;
  volatile uint32_t WDFEED;
  volatile uint32_t WDTV;
};

struct GPIO
{
  volatile uint32_t DATA[4096];
  uint32_t dummy0[4096];
  volatile uint32_t DIR;
  volatile uint32_t IS;
  volatile uint32_t IBE;
  volatile uint32_t IEV;
  volatile uint32_t IE;
  volatile uint32_t RIS;
  volatile uint32_t MIS;
  volatile uint32_t IC;
  uint32_t dummy1[8184];
};

struct UART
{
  union { volatile uint32_t DLL, THR, RBR; };
  union { volatile uint32_t IER, DLM; };
  union { volatile uint32_t FCR, IIR; };

  volatile uint32_t LCR;
  volatile uint32_t MCR;
  volatile uint32_t LSR;
  volatile uint32_t MSR;
  volatile uint32_t SCR;
  volatile uint32_t ACR;
  uint32_t dummy0[1];
  volatile uint32_t FDR;
  uint32_t dummy1[1];
  volatile uint32_t TER;
  uint32_t dummy2[6];

  volatile uint32_t RS485CTRL;
  volatile uint32_t RS485ADRMATCH;
  volatile uint32_t RS485DLY;
};

/* IO memory map */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#define LPC_ROM_BASE          (0x1FFF0000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_AHB_BASE          (0x50000000UL)

/* APB0 peripherals */
#define LPC_I2C_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_WDT_BASE          (LPC_APB0_BASE + 0x04000)
#define LPC_UART_BASE         (LPC_APB0_BASE + 0x08000)
#define LPC_ADC_BASE          (LPC_APB0_BASE + 0x1C000)
#define LPC_PMU_BASE          (LPC_APB0_BASE + 0x38000)
#define LPC_FLASHCTRL_BASE    (LPC_APB0_BASE + 0x3C000)
#define LPC_SPI0_BASE         (LPC_APB0_BASE + 0x40000)
#define LPC_IOCON_BASE        (LPC_APB0_BASE + 0x44000)
#define LPC_SYSCON_BASE       (LPC_APB0_BASE + 0x48000)
#define LPC_CAN_BASE          (LPC_APB0_BASE + 0x50000)
#define LPC_SPI1_BASE         (LPC_APB0_BASE + 0x58000)

/* AHB peripherals */
#define LPC_GPIO_BASE         (LPC_AHB_BASE + 0x00000)

static struct WDT *const WDT = ((struct WDT *const) LPC_WDT_BASE);
static struct PMU *const PMU = ((struct PMU *const) LPC_PMU_BASE);
static struct FLASHCTRL *const FLASHCTRL =
  ((struct FLASHCTRL *const) LPC_FLASHCTRL_BASE);
static struct IOCON *const IOCON = ((struct IOCON *const) LPC_IOCON_BASE);
static struct SYSCON *const SYSCON = ((struct SYSCON *const) LPC_SYSCON_BASE);
static struct UART *const UART = ((struct UART *const) LPC_UART_BASE);
static struct GPIO *const GPIO = ((struct GPIO *const) LPC_GPIO_BASE);
