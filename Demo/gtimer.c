#include <gtimer.h>

int get_cpuid(void)
{
	return (get_MPIDR() & 0xFF);
}

u32 gt_get_clock_freq(){
	return get_CNTFRQ(); 
}

u64 gt_get_virtual_count(){
	return get_CNTVCT();
}

u64 gt_get_physical_count(){
	return get_CNTPCT();
}

void gt_disable_virtual_timer(){
	u32 val = get_CNTV_CTL();
	val &= ~(1 << 0);
	set_CNTV_CTL(val);
}

void gt_enable_virtual_timer(){
	u32 val = em_get_CNTV_CTL();
	val |= (1 << 0);
//	val &= ~(1 << 1);
	em_set_CNTV_CTL(val);
}

int gt_is_virtual_timer_irq_asserted(){
	return (get_CNTV_CTL() >> 2) & 0x1;
}

void gt_mask_virtual_timer(){
	u32 val = get_CNTV_CTL();
	val |= (1 << 1);
	set_CNTV_CTL(val);
}

void gt_unmask_virtual_timer(){
	u32 val = em_get_CNTV_CTL();
	val &= ~(1 << 1);
	em_set_CNTV_CTL(val);
	//em_printf("unmask %d\n", get_CNTV_CTL());
}

void gt_set_virtual_timer_deadline(u64 deadline){
	u64 current_virtual_time = gt_get_virtual_count();
	set_CNTV_CVAL(current_virtual_time+deadline); 
}

u32 gt_get_cntv_ctl(){
	return get_CNTV_CTL();
}

