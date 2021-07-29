#ifndef __LIB_IRQ_IRQ_H__
#define __LIB_IRQ_IRQ_H__

#include <gic-types.h>

//#define GICD_BASE				0x10481000
//#define GICC_BASE				0x10482000
//#define NIRQS					160

#define GICD_BASE 0x50041000
#define GICC_BASE 0x50042000
extern int NIRQS;

#define GICD_CTLR               0x0000
#define GICD_TYPER              0x0004
#define GICD_IIDR               0x0008

#define GICD_IPRIORITYR_BASE    0x0400
#define GICD_IPRIORITYR(n)      (GICD_IPRIORITYR_BASE + n * 4)

#define GICD_ISENABLER_BASE     0x0100
#define GICD_ISENABLER(n)       (GICD_ISENABLER_BASE + n * 4)

#define GICD_ICENABLER_BASE     0x0180
#define GICD_ICENABLER(n)       (GICD_ICENABLER_BASE + n * 4)

#define GICD_ITARGETSR_BASE     0x0800
#define GICD_ITARGETSR(n)       (GICD_ITARGETSR_BASE + n * 4)

#define GICD_ICFGR_BASE         0x0C00
#define GICD_ICFGR(n)           (GICD_ICFGR_BASE + n * 4)

#define GICD_ISPENDR_BASE       0x0200
#define GICD_ISPENDR(n)         (GICD_ISPENDR_BASE + n * 4)

#define GICD_ICPENDR_BASE       0x0280
#define GICD_ICPENDR(n)         (GICD_ICPENDR_BASE + n * 4)

#define GICD_CPENDSGIR_BASE     0x0F10
#define GICD_CPENDSGIR(n)       (GICD_CPENDSGIR_BASE + n * 4)

#define GICD_SPENDSGIR_BASE     0x0F20
#define GICD_SPENDSGIR(n)       (GICD_SPENDSGIR_BASE + n * 4)

#define GICD_ICACTIVER_BASE     0x0380
#define GICD_ICACTIVER(n)       (GICD_ICACTIVER_BASE + n * 4)

#define GICD_ISACTIVER_BASE     0x0300
#define GICD_ISACTIVER(n)       (GICD_ISACTIVER_BASE + n * 4)

#define GICD_SGIR               0x0F00



#define GICC_CTLR				0x0000
#define GICC_PMR				0x0004
#define GICC_BPR				0x0008
#define GICC_IAR				0x000C
#define GICC_EOIR				0x0010
#define GICC_RPR				0x0014
#define GICC_HPPIR				0x0018
#define GICC_APR_BASE			0x00D0
#define GICC_APR(n)				(GICC_APR_BASE + n * 4)
#define GICC_NSAPR_BASE			0x00E0
#define GICC_NSAPR(n)			(GICC_NSAPR_BASE + n * 4)
#define GICC_IIDR				0x00FC
#define GICC_DIR				0x1000

#define GIC_SPURIOUS_IRQ		1023

enum gic_op {
	GIC_CLR_VALUE = 0,
	GIC_SET_VALUE = 1,
};


/* Configuration Register */
void gicd_enable_irq_forwarding(void);
void gicd_disable_irq_forwarding(void);
void write_GICD_CTLR(u32);
u32  read_GICD_CTLR(void);

/* GICD_TYPER */
void write_GICD_TYPER(u32);
u32  read_GICD_TYPER(void);

/* GICD_IIDR */
void write_GICD_IIDR(u32);
u32  read_GICD_IIDR(void);

/* Enable Register */
void gicd_set_enable_irq(int);
void gicd_clr_enable_irq(int);
void write_GICD_ISENABLER(int,u32);
void write_GICD_ICENABLER(int,u32);
u32  read_GICD_ISENABLER(int);
u32  read_GICD_ICENABLER(int);

/* Pending Register  */
void gicd_set_pending_irq(int);
void gicd_clr_pending_irq(int);
void write_GICD_ISPENDR(int, u32);
void write_GICD_ICPENDR(int, u32);
u32  read_GICD_ISPENDR(int);
u32  read_GICD_ICPENDR(int);

/* Active register */
void gicd_set_active_irq(int);
void gicd_clr_active_irq(int);
void write_GICD_ISACTIVER(int, u32);
void write_GICD_ICACTIVER(int, u32);
u32  read_GICD_ISACTIVER(int);
u32  read_GICD_ICACTIVER(int);

/* Priority register */
void gicd_set_priority_irq(int, int);
void write_GICD_IPRIORITYR(int, u32);
u32  read_GICD_IPRIORITYR(int);

/* Target register */
void gicd_set_target_irq(int, int);
void gicd_clr_target_irq(int, int);
void write_GICD_ITARGETSR(int, u32);
u32  read_GICD_ITARGETSR(int);

/* interrupt configuration register */
void write_GICD_ICFGR(int, u32);
u32  read_GICD_ICFGR(int);

/* SGI register */
void gicd_send_sgi_to_cpu(int, int);
void gicd_send_sgi_to_cpu_list(int, u32);
//void send_sgi_to_all_except_me(int);
//void send_sgi_to_me(int);
void write_GICD_SGIR(u32);
u32  read_GICD_SGIR(void);

/* GICD_CPENDSGIR / GICD_SPENDSGIR */
//void set_sgi_pending(int);
//void clr_sgi_pending(int);
void write_GICD_CPENDSGIR(int, u32);
void write_GICD_SPENDSGIR(int, u32);
u32  read_GICD_CPENDSGIR(int);
u32  read_GICD_SPENDSGIR(int);



void gicc_enable_cpu_interface(void);
void gicc_disable_cpu_interface(void);
void gicc_eoi_mode(enum gic_op);
u32  read_GICC_CTLR(void);
void write_GICC_CTLR(u32);

void write_GICC_PMR(u32);
u32  read_GICC_PMR(void);
void gicc_set_priority_register(u32);

void write_GICC_BPR(u32);
u32  read_GICC_BPR(void);
void gicc_set_binary_point_register(u32);

u32  read_GICC_IAR(void);
u32  gicc_ack_irq(int*);

void write_GICC_EOIR(u32);
void gicc_eoied_irq(int, int);

u32  read_GICC_RPR(void);
u32  gicc_running_priority(void);

u32  read_GICC_HPPIR(void);
u32  gicc_highest_prio_irq(int*);

void write_GICC_APR(int, u32);
u32  read_GICC_APR(int);

void write_GICC_NSAPR(int, u32);
u32  read_GICC_NSAPR(int);

u32  read_GICC_IIDR(void);

void write_GICC_DIR(u32);
void gicc_deactivate_irq(int, int);

#endif
