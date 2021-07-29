#include "gic.h"

static inline void write(u32 addr, int value){
        *((volatile unsigned long *)addr) = value;
}
static inline u32 read(u32 addr){
        return *((u32 *)addr);
}

/******************************************************************/
/****************************** DISTRIBUTOR ***********************/
/******************************************************************/
/* GICD_CTLR */
void gicd_enable_irq_forwarding(void){
	write(GICD_BASE+GICD_CTLR,0x1);
}
void gicd_disable_irq_forwarding(void){
	write(GICD_BASE+GICD_CTLR,0x0);
}
void write_GICD_CTLR(u32 val){
	write(GICD_BASE+GICD_CTLR,val);
}
u32 read_GICD_CTLR(){
	return read(GICD_BASE+GICD_CTLR);
}

/* GICD_TYPER */
void write_GICD_TYPER(u32 val){
	write(GICD_BASE+GICD_TYPER,val);
}
u32  read_GICD_TYPER(){
	return read(GICD_BASE+GICD_TYPER);
}

/* GICD_IIDR */
void write_GICD_IIDR(u32 val){
	write(GICD_BASE+GICD_IIDR,val);
}
u32  read_GICD_IIDR(){
	return read(GICD_BASE+GICD_IIDR);
}

/* ISENABLER / ICENABLER */
void gicd_set_enable_irq(int irq){
	int idx = irq >> 5;
	int off = irq & 31;
	u32 val;
	
//	val = read(GICD_BASE+GICD_ISENABLER(idx));
//	val |= (1 << off);
	val = (1 << off);	
	write(GICD_BASE+GICD_ISENABLER(idx), val);
}
void gicd_clr_enable_irq(int irq){
	int idx = irq >> 5;
	int off = irq & 31;
	u32 val;
	
//	val = read(GICD_BASE+GICD_ICENABLER(idx));
//	val |= (1 << off);
	val = (1 << off);
	write(GICD_BASE+GICD_ICENABLER(idx), val);
}
void write_GICD_ISENABLER(int idx, u32 val){
	write(GICD_BASE+GICD_ISENABLER(idx), val);
}
void write_GICD_ICENABLER(int idx, u32 val){
	write(GICD_BASE+GICD_ICENABLER(idx), val);
}
u32 read_GICD_ISENABLER(int idx){
	return read(GICD_BASE+GICD_ISENABLER(idx));
}
u32 read_GICD_ICENABLER(int idx){
	return read(GICD_BASE+GICD_ICENABLER(idx));
}
/* ISPENDR / ICPENDR */
void gicd_set_pending_irq(int irq){
	int idx = irq >> 5;
	int off = irq & 31;
	u32 val;
	
	val = read(GICD_BASE+GICD_ISPENDR(idx));
	val |= (1 << off);
	write(GICD_BASE+GICD_ISPENDR(idx), val);
}
void gicd_clr_pending_irq(int irq){
	int idx = irq >> 5;
	int off = irq & 31;
	u32 val;
	
	val = read(GICD_BASE+GICD_ICPENDR(idx));
	val |= (1 << off);
	write(GICD_BASE+GICD_ICPENDR(idx), val);
}
void write_GICD_ISPENDR(int idx, u32 val){
	write(GICD_BASE+GICD_ISPENDR(idx), val);
}
void write_GICD_ICPENDR(int idx, u32 val){
	write(GICD_BASE+GICD_ICPENDR(idx), val);
}
u32 read_GICD_ISPENDR(int idx){
	return read(GICD_BASE+GICD_ISPENDR(idx));
}
u32 read_GICD_ICPENDR(int idx){
	return read(GICD_BASE+GICD_ICPENDR(idx));
}

/* ISACTIVER / ICACTIVER */
void gicd_set_active_irq(int irq){
	int idx = irq >> 5;
	int off = irq & 31;
	u32 val;
	
	val = read(GICD_BASE+GICD_ISACTIVER(idx));
	val |= (1 << off);
	write(GICD_BASE+GICD_ISACTIVER(idx), val);

}
void gicd_clr_active_irq(int irq){
	int idx = irq >> 5;
	int off = irq & 31;
	u32 val;
	
	val = read(GICD_BASE+GICD_ICACTIVER(idx));
	val |= (1 << off);
	write(GICD_BASE+GICD_ICACTIVER(idx), val);
}
void write_GICD_ISACTIVER(int idx, u32 val){
	write(GICD_BASE+GICD_ISACTIVER(idx), val);
}
void write_GICD_ICACTIVER(int idx, u32 val){
	write(GICD_BASE+GICD_ICACTIVER(idx), val);
}
u32 read_GICD_ISACTIVER(int idx){
	return read(GICD_BASE+GICD_ISACTIVER(idx));
}
u32 read_GICD_ICACTIVER(int idx){
	return read(GICD_BASE+GICD_ICACTIVER(idx));
}

/* GICD_IPRIORITYR */
void gicd_set_priority_irq(int irq, int priority){
	int idx = irq >> 2;
	int off = (irq & 3) << 3;
	u32 val = read(GICD_BASE+GICD_IPRIORITYR(idx));
	u32 mask = 0xff << off;
	/* clear the old priority */
	val &= ~mask;
	/* set new peiority */
	val |= (priority & 0xff) << off;
	write(GICD_BASE+GICD_IPRIORITYR(idx), val);
}
void write_GICD_IPRIORITYR(int idx, u32 val){
	write(GICD_BASE+GICD_IPRIORITYR(idx), val);
}
u32 read_GICD_IPRIORITYR(int idx){
	return read(GICD_BASE+GICD_IPRIORITYR(idx));
}

/* GICD_ITARGETSR */
void gicd_set_target_irq(int irq, int cpu_id){
	int idx = irq >> 2;
	int off = (irq & 3) << 3;
	u32 val = read(GICD_BASE+GICD_ITARGETSR(idx));
	/* update the register value and copy back */
	val |= ((1 << cpu_id) << off);
	write(GICD_BASE+GICD_ITARGETSR(idx),val);
}
void gicd_clr_target_irq(int irq, int cpu_id){
	int idx = irq >> 2;
	int off = (irq & 3) << 3;
	u32 val = read(GICD_BASE+GICD_ITARGETSR(idx));
	/* update the register value and copy back */
	val &= ~((1 << cpu_id) << off);
	write(GICD_BASE+GICD_ITARGETSR(idx),val);
}
void write_GICD_ITARGETSR(int idx, u32 val){
	write(GICD_BASE+GICD_ITARGETSR(idx),val);
}
u32 read_GICD_ITARGETSR(int idx){
	return read(GICD_BASE+GICD_ITARGETSR(idx));
}

/* GICD_ICFGR */
void write_GICD_ICFGR(int idx, u32 val){
	write(GICD_BASE+GICD_ICFGR(idx),val);
}
u32 read_GICD_ICFGR(int idx){
	return read(GICD_BASE+GICD_ICFGR(idx));
}

/* GICD_SGIR */
void gicd_send_sgi_to_cpu(int sgi, int cpu_id){
	u32 val = (sgi & 0xf) | ((1 << cpu_id) << 16) | (0x0 << 23);
	write(GICD_BASE+GICD_SGIR, val);
}
void gicd_send_sgi_to_cpu_list(int sgi, u32 list_cpus){
	u32 val = (sgi & 0xf) | ((list_cpus & 0xff) << 16) | (0x0 << 23);
	write(GICD_BASE+GICD_SGIR, val);
}
void write_GICD_SGIR(u32 val){
	write(GICD_BASE+GICD_SGIR, val);
}
u32 read_GICD_SGIR(){
	return read(GICD_BASE+GICD_SGIR);
}

/* sgi pending register */
//void set_sgi_pending(int){
//
//}
//void clr_sgi_pending(int){
//
//}
void write_GICD_CPENDSGIR(int idx, u32 val){
	write(GICD_BASE+GICD_CPENDSGIR(idx),val);
}
void write_GICD_SPENDSGIR(int idx, u32 val){
	write(GICD_BASE+GICD_SPENDSGIR(idx),val);
}
u32  read_GICD_CPENDSGIR(int idx){
	return read(GICD_BASE+GICD_CPENDSGIR(idx));
}
u32  read_GICD_SPENDSGIR(int idx){
	return read(GICD_BASE+GICD_SPENDSGIR(idx));
}


/******************************************************************/
/************************* CPU INTERFACE **************************/
/******************************************************************/
u32 read_GICC_CTLR(){
	return read(GICC_BASE+GICC_CTLR);
}
void write_GICC_CTLR(u32 val){
	write(GICC_BASE+GICC_CTLR, val);
}
void gicc_enable_cpu_interface(){
	u32 val = read_GICC_CTLR();
	val |= 0x1;
	write_GICC_CTLR(val);
}
void gicc_disable_cpu_interface(){
	u32 val = read_GICC_CTLR();
	val &= ~(0x1);
	write_GICC_CTLR(val);
}
void gicc_eoi_mode(enum gic_op op){
	u32 val = read_GICC_CTLR();
	if(op == GIC_SET_VALUE) val |= (0x1 << 9);
	else val &= ~(0x1);
	write_GICC_CTLR(val);
}


u32 read_GICC_PMR(){
	return read(GICC_BASE+GICC_PMR);
}
void write_GICC_PMR(u32 val){
	write(GICC_BASE+GICC_PMR,val);
}
void gicc_set_priority_register(u32 val){
	write_GICC_PMR((val & 0xff));
}

u32 read_GICC_BPR(){
	return read(GICC_BASE+GICC_BPR);
}
void write_GICC_BPR(u32 val){
	write(GICC_BASE+GICC_BPR,val);
}
void gicc_set_binary_point_register(u32 val){
	write_GICC_BPR((val & 0x7));	
}

u32 read_GICC_IAR(){
	return read(GICC_BASE+GICC_IAR);
}
u32 gicc_ack_irq(int* cpu_source){
	u32 val = read(GICC_BASE+GICC_IAR);
	*cpu_source = (val >> 10) & 0x7;
	return (val & 0x3ff);	
}

void write_GICC_EOIR(u32 val){
	write(GICC_BASE+GICC_EOIR,val);
}
void gicc_eoied_irq(int irq, int cpu_source){
	u32 val = irq & 0x3ff;
	if (irq < 16) val |= ((cpu_source & 0x7) << 10);
	write_GICC_EOIR(val);
}

u32 read_GICC_RPR(){
	return read(GICC_BASE+GICC_RPR);
}
u32 gicc_running_priority(){
	return (read_GICC_RPR() & 0xff);
}

u32 read_GICC_HPPIR(){
	return read(GICC_BASE+GICC_HPPIR);
}
u32 gicc_highest_prio_irq(int* cpu){
	u32 val = read_GICC_HPPIR();
	*cpu = (val >> 10) & 0x7;
	return (val & 0x3ff);  }

void write_GICC_APR(int idx, u32 val){
	write(GICC_BASE+GICC_APR(idx), val);
}
u32  read_GICC_APR(int idx){
	return read(GICC_BASE+GICC_APR(idx));
}

void write_GICC_NSAPR(int idx, u32 val){
	write(GICC_BASE+GICC_NSAPR(idx), val);
}
u32  read_GICC_NSAPR(int idx){
	return read(GICC_BASE+GICC_NSAPR(idx));
}

u32  read_GICC_IIDR(){
	return read(GICC_BASE+GICC_IIDR);
}

void write_GICC_DIR(u32 val){
	write(GICC_BASE+GICC_DIR, val);
}
void gicc_deactivate_irq(int irq, int cpu_source){
	u32 val = irq & 0x3ff;
	if (irq < 16) val |= ((cpu_source & 0x7) << 10);
	write_GICC_DIR(val);
}
/*
void gicc_init_example(){
	gicc_disable_cpu_interface();
	gicc_eoi_mode(GIC_SET_VALUE);
	gicc_set_priority_register(0xf0);
	gicc_enable_cpu_interface();
}
*/

