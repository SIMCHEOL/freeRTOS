.global _reset
_reset:
	ldr	r2, =_vector
	mcr	p15, 0, r2, c12, c0, 0

	ldr	sp, =stack_top
	

	msr	cpsr_c, 0x12
	ldr	sp, =stack_irq
	msr	cpsr_c, 0x13

	bl	main
	b 	.
.global _vector
.align 5
_vector:
/*
        b       _reset
        b       _undef
        b       _svc
        b       _pabort
        b       _dabort 
        b       .
        b       _irq
        b       _fiq
 */
	b	.
	b	.
	b	_svc
	b	.
	b	.
	b	.
	b	_irq
	b	.               
_undef:
_svc:
	B	vPortSVCHandler
_pabort:
_dabort:
_fiq:   
_irq:
	B	vPortInterruptContext

.global _setup_sctlr
_setup_sctlr:
	
        mrc     p15, 0, r0, c1, c0, 0 @read SCTLR
        orr     r0, r0, #0 << 13
        mcr     p15, 0, r0, c1, c0, 0 @write SCTLR
	dsb
	isb

