#define MEMORY_MAX_ADDR 0x04000000
#define STACK_SIZE 0x100000 /* 1MB */

#define PSR_MMUMOD_MASK 0x3
#define PSR_MMUMOD (PSR & PSR_MMUMOD_MASK)
#define PSR_MMUMOD_DIRECT 0x0
#define PSR_MMUMOD_L1 0x1
#define PSR_MMUMOD_L2 0x2

#define PSR_MMUPS_MASK 0x380
#define PSR_MMUPS_OFFSET 7
#define PSR_MMUPS ((PSR & PSR_MMUPS_MASK) >> PSR_MMUPS_OFFSET)
#define PSR_MMUPS_4KB 0x1

#define MMU_PAGE_SIZE 4096
#define MMU_PAGE_SIZE_PSE (4096 * 1024)
#define MMU_PAGE_INDEX_L1 0xffc00000
#define MMU_PAGE_INDEX_L2 0x003ff000
#define MMU_PAGE_OFFSET 0x0000fff
#define MMU_PAGE_OFFSET_PSE 0x003fffff
#define MMU_PAGE_NUM 0xfffff000

#define MMU_PTE_VALID 0x001
#define MMU_PTE_R 0x002
#define MMU_PTE_D 0x004
#define MMU_PTE_EX 0x008
#define MMU_PTE_PP 0x030
#define MMU_PTE_PP_RWXX 0x000
#define MMU_PTE_PP_RDXX 0x010
#define MMU_PTE_PP_RWRD 0x020
#define MMU_PTE_PP_RWRW 0x030
#define MMU_PTE_CD 0x040
#define MMU_PTE_G 0x080
#define MMU_PTE_PE 0x100

#define OFFSET(ptr, offset) ((void *)((char *)ptr + offset))

typedef union {
  struct {
    unsigned int indexL1 : 10;
    unsigned int indexL2 : 10;
    unsigned int flags  : 12;
  };
  unsigned int raw;
  void *ptr;
} page_table_entry;

void *memcpy_bswap32(void *dest, const void *src, size_t n);

static inline void memory_paging_start(void)
{
  unsigned int psr;

  __asm__("srpsr %0" : "=r"(psr));
  psr = (psr & PSR_MMUPS_MASK) | (PSR_MMUPS_4KB << PSR_MMUPS_OFFSET);
  __asm__("srpsw %0" : : "r"(psr));

  __asm__("srmmuw 2");
}

static inline void memory_paging_stop(void)
{
  __asm__("srmmuw 0");
}

static inline void memory_pdt_set(page_table_entry *pdt)
{
  __asm__("srpdtw %0" : : "r"(pdt));
}

static inline void memory_kpdt_set(page_table_entry *pdt)
{
  __asm__("srkpdtw %0" : : "r"(pdt));
}
