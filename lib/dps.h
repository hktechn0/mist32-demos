#include <stdbool.h>

#define DPS_SIZE 0x200

/* UTIM64 */
#define DPS_UTIM64A 0x000
#define DPS_UTIM64B 0x040
#define DPS_UTIM64_SIZE 0x7f
#define DPS_UTIM64_TIMER_SIZE 0x3c

#define DPS_UTIM64_MCFGR 0x000
#define DPS_UTIM64_MCR 0x004
#define DPS_UTIM64_CC0R 0x00c
#define DPS_UTIM64_CC1R 0x014
#define DPS_UTIM64_CC2R 0x01c
#define DPS_UTIM64_CC3R 0x024
#define DPS_UTIM64_CC0CFGR 0x02c
#define DPS_UTIM64_CC1CFGR 0x030
#define DPS_UTIM64_CC2CFGR 0x034
#define DPS_UTIM64_CC3CFGR 0x038

#define DPS_UTIM64AMCFGR 0x000
#define DPS_UTIM64AMCR 0x004
#define DPS_UTIM64ACC0R 0x00c
#define DPS_UTIM64ACC1R 0x014
#define DPS_UTIM64ACC2R 0x01c
#define DPS_UTIM64ACC3R 0x024
#define DPS_UTIM64ACC0CFGR 0x02c
#define DPS_UTIM64ACC1CFGR 0x030
#define DPS_UTIM64ACC2CFGR 0x034
#define DPS_UTIM64ACC3CFGR 0x038
#define DPS_UTIM64BMCFGR 0x040
#define DPS_UTIM64BMCR 0x044
#define DPS_UTIM64BCC0R 0x04c
#define DPS_UTIM64BCC1R 0x054
#define DPS_UTIM64BCC2R 0x05c
#define DPS_UTIM64BCC3R 0x064
#define DPS_UTIM64BCC0CFGR 0x06c
#define DPS_UTIM64BCC1CFGR 0x070
#define DPS_UTIM64BCC2CFGR 0x074
#define DPS_UTIM64BCC3CFGR 0x078
#define DPS_UTIM64FLAGS 0x07c

#define UTIM64MCFG_ENA 0x1
#define UTIM64CFG_ENA 0x1
#define UTIM64CFG_IE 0x2
#define UTIM64CFG_BIT 0x4
#define UTIM64CFG_MODE 0x8
#define UTIM64FLAGS_A0 0x01
#define UTIM64FLAGS_A1 0x02
#define UTIM64FLAGS_A2 0x04
#define UTIM64FLAGS_A3 0x08
#define UTIM64FLAGS_B0 0x10
#define UTIM64FLAGS_B1 0x20
#define UTIM64FLAGS_B2 0x40
#define UTIM64FLAGS_B3 0x80

/* SCI */
#define DPS_SCI 0x100
#define DPS_SCITXD 0x100
#define DPS_SCIRXD 0x104
#define DPS_SCICFG 0x108

#define SCIRXD_VALID 0x80000000
#define SCICFG_TEN 0x1
#define SCICFG_REN 0x2
#define SCICFG_TCLR 0x1000
#define SCICFG_RCLR 0x2000
#define SCICFG_BDR_OFFSET 2
#define SCICFG_TIRE_MASK 0x1c0
#define SCICFG_TIRE_OFFSET 6
#define SCICFG_RIRE_MASK 0xe00
#define SCICFG_RIRE_OFFSET 9

/* MI */
#define DPS_MIMSR 0x120

/* buffer */
#define SCI_RX_BUFFER_SIZE 128

/* LSFLAGS */
#define DPS_LSFLAGS 0x1fc
#define DPS_LSFLAGS_SCITIE 0x01
#define DPS_LSFLAGS_SCIRIE 0x02

/* DPS struct */
typedef volatile struct _dps_utim64 {
  volatile unsigned int mcfg;
  volatile unsigned int mc[2];
  volatile unsigned int cc[4][2];
  volatile unsigned int cccfg[4];
} dps_utim64;

typedef volatile struct _dps_sci {
  volatile unsigned int txd;
  volatile unsigned int rxd;
  volatile unsigned int cfg;
} dps_sci;

unsigned int dps_mi_memsize(void);
dps_utim64 *dps_utim64_init(int id);
void dps_utim64_enable(dps_utim64 *utim64);
void dps_utim64_disable(dps_utim64 *utim64);
dps_sci *dps_sci_init(void);
void dps_sci_setup(dps_sci *sci, bool t_en, bool r_en,
		   unsigned int baud, unsigned int t_int, unsigned int r_int);
void dps_sci_clear(dps_sci *sci, bool t_buf, bool r_buf);

int sci_putc(dps_sci *sci, int c);
int sci_getc(dps_sci *sci);

/* inline function */

/* DPS Low speed interrupt factor and ack */
static inline unsigned int dps_lsflags(void)
{
  volatile unsigned int *lsflags;

  lsflags = (unsigned int *)((char *)iosr_read() + DPS_LSFLAGS);
  return *lsflags;
}

static inline unsigned int dps_utim64_flags(void)
{
  char *iosr = iosr_read();
  return *(volatile unsigned int *)(iosr + DPS_UTIM64FLAGS);
}
