#ifndef __LIB_TIMER_GTIMER_H__
#define __LIB_TIMER_GTIMER_H__

#include "gic-types.h"
#include "gic-coproc.h"

#define VIRTUAL_GENERIC_TIMER_IRQ 	27

MRC_32(CNTFRQ,0,14,0,0)
MCR_32(CNTFRQ,0,14,0,0)

MRRC_64(CNTPCT,0,14)

MRC_32(CNTKCTL,0,14,1,0)
MCR_32(CNTKCTL,0,14,1,0)

MRC_32(CNTP_TVAL,0,14,2,0)
MCR_32(CNTP_TVAL,0,14,2,0)

MRC_32(CNTP_CTL,0,14,2,1)
MCR_32(CNTP_CTL,0,14,2,1)

MRC_32(CNTV_TVAL,0,14,3,0)
MCR_32(CNTV_TVAL,0,14,3,0)

MRC_32(CNTV_CTL,0,14,3,1)
MCR_32(CNTV_CTL,0,14,3,1)
em_MRC_32(CNTV_CTL,0,14,3,1)
em_MCR_32(CNTV_CTL,0,14,3,1)

MRRC_64(CNTVCT,1,14)

MRRC_64(CNTP_CVAL,2,14)
MCRR_64(CNTP_CVAL,2,14)

MRRC_64(CNTV_CVAL,3,14)
MCRR_64(CNTV_CVAL,3,14)

MRC_32(MPIDR,0,0,0,5)
int get_cpuid(void);

u64  gt_get_virtual_count(void);
u64  gt_get_physical_count(void);
void gt_disable_virtual_timer(void);
void gt_enable_virtual_timer(void);
int  gt_is_virtual_timer_irq_asserted(void);
void gt_mask_virtual_timer(void);
void gt_unmask_virtual_timer(void);
void gt_set_virtual_timer_deadline(u64 deadline);
u32  gt_get_cntv_ctl(void);
u32  gt_get_clock_freq();


#endif
