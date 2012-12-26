#define MEMORY_MAX_ADDR 0x04000000
#define IDT_ENTRY_MAX 128
#define STACK_SIZE 0x4000

#define DPS_SIZE 0x200
#define GCI_HUB_SIZE 0x400
#define GCI_HUB_HEADER_SIZE 0x100
#define GCI_NODE_SIZE 0x400

#define GCI_KEYBOARD_NUM 0
#define GCI_DISPLAY_NUM 1

#define IDT_FLAGS_NONE 0x0
#define IDT_FLAGS_VALID 0x1
#define IDT_FLAGS_ENABLE 0x2

#define IDT_GCI_START_NUM 5
#define IDT_KEYBOARD_NUM (IDT_GCI_START_NUM + GCI_KEYBOARD_NUM)

#define DISPLAY_BITMAP_OFFSET 0xc000

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 34

#define WIDTH 640
#define HEIGHT 480

#define IMAGE_MAX 13

typedef volatile struct _idt_entry {
  unsigned int flags;
  void (*handler)(void);
} idt_entry;

typedef volatile struct _gci_hub_info {
  unsigned int total;
  unsigned int space_size;
} gci_hub_info;

typedef volatile struct _gci_hub_node {
  unsigned int size;
  unsigned int priority;
  unsigned int _reserved1;
  unsigned int _reserved2;
  unsigned int _reserved3;
} gci_hub_node;

typedef volatile struct _gci_node_info {
  unsigned int area_size;
  unsigned int int_priority;
  unsigned int int_factor;
  unsigned int _reserved;
} gci_node_info;

typedef struct _gci_node {
  gci_node_info *node_info;
  void *device_area;
} gci_node;

typedef unsigned int size_t;

extern gci_hub_info *gci_hub;
extern gci_hub_node *gci_hub_nodes;
extern gci_node *gci_nodes;

void *malloc(size_t size);

idt_entry *idt_setup(void);
inline void interrupt_enable(void);
inline void interrupt_disable(void);
inline void idt_set(idt_entry *idtr);
inline void idt_load(void);
void idt_entry_setup(volatile idt_entry *idt, unsigned int num, void (*handler)(void));
void idt_entry_enable(volatile idt_entry *idt, unsigned int num);

void gci_setup(void);
inline void display_putc(void *display_io, unsigned int pos, char c,
			 unsigned int forecolor, unsigned int backcolor);
inline void display_put(void *display_io, unsigned int x, unsigned int y, unsigned int color);
