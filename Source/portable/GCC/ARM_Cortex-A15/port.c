/*
    FreeRTOS V7.0.1 - Copyright (C) 2011 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM Cortex-A9 port.
 *----------------------------------------------------------*/

#include <gic.h>
#include <gtimer.h>

#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* For backward compatibility, ensure configKERNEL_INTERRUPT_PRIORITY is
defined.  The value should also ensure backward compatibility.
FreeRTOS.org versions prior to V4.4.0 did not include this definition. */
#ifndef configKERNEL_INTERRUPT_PRIORITY
	#define configKERNEL_INTERRUPT_PRIORITY 255
#endif

/* Constants required to set up the initial stack. */
#define portINITIAL_XPSR			( 0x0000001F )

/* Interrupt Handler Support. */
typedef struct STRUCT_HANDLER_PARAMETER
{
	void (*vHandler)(void *);
    void *pvParameter;
} xInterruptHandlerDefinition;
xInterruptHandlerDefinition pxInterruptHandlers[ portMAX_VECTORS ] = { { NULL, NULL } };

extern unsigned portBASE_TYPE * volatile pxCurrentTCB;

/* Definition of the max vector id. */
static unsigned long ulMaxVectorId = portMAX_VECTORS;

/* The priority used by the kernel is assigned to a variable to make access
from inline assembler easier. */
const unsigned long ulKernelPriority = configKERNEL_INTERRUPT_PRIORITY;

/* Critical Nesting is a system wide variable which is saved to each task's stack. */
//static unsigned portBASE_TYPE uxCriticalNesting = 0xaaaaaaaa;

/* Declare some stack space for each mode. */
static portSTACK_TYPE puxFIQStack[ portFIQ_STACK_SIZE ];
static portSTACK_TYPE puxIRQStack[ portIRQ_STACK_SIZE ];
static portSTACK_TYPE puxAbortStack[ portABORT_STACK_SIZE ];
static portSTACK_TYPE puxSVCStack[ portSVC_STACK_SIZE ];
static portSTACK_TYPE *puxFIQStackPointer = &(puxFIQStack[ portFIQ_STACK_SIZE - 1 ] );
static portSTACK_TYPE *puxIRQStackPointer = &(puxIRQStack[ portIRQ_STACK_SIZE - 1 ] );
static portSTACK_TYPE *puxAbortStackPointer = &(puxAbortStack[ portABORT_STACK_SIZE - 1 ] );
static portSTACK_TYPE *puxSVCStackPointer = &(puxSVCStack[ portSVC_STACK_SIZE - 1 ] );

/*
 * Setup the timer to generate the tick interrupts.
 */
static void prvSetupTimerInterrupt( void );

/*
 * Exception handlers.
 */
void vPortPendSVHandler( void *pvParameter ) __attribute__((naked));
void vPortSysTickHandler( void *pvParameter );
void vPortSVCHandler( void ) __attribute__ (( naked ));
void vPortInterruptContext( void ) __attribute__ (( naked ));
void vPortSMCHandler( void ) __attribute__ (( naked ));

// EASW
void em_SwitchContext( void ) __attribute__((naked));

/*
 * Start first task is a separate function so it can be tested in isolation.
 */
void vPortStartFirstTask( void ) __attribute__ (( naked ));

/* Interrupt Handler code. */
void vPortInstallInterruptHandler( void (*vHandler)(void *), void *pvParameter, unsigned long ulVector, unsigned char ucEdgeTriggered, unsigned char ucPriority, unsigned char ucProcessorTargets );
/*-----------------------------------------------------------*/

/* Linker defined variables. */
/*
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
extern unsigned long _stack_top;
*/
/*----------------------------------------------------------------------------*/

char *itoa_tmp( int iIn, char *pcBuffer )
{
char *pcReturn = pcBuffer;
int iDivisor = 0;
int iResult = 0;
	for ( iDivisor = 100; iDivisor > 0; iDivisor /= 10 )
	{
		iResult = ( iIn / iDivisor ) % 10;
		*pcBuffer++ = (char)iResult + '0';
	}
	return pcReturn;
}
/*----------------------------------------------------------------------------*/

/*
 * See header file for description.
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
portSTACK_TYPE *pxOriginalStack = pxTopOfStack;


	//em_printf("pxPortInitStack!!!!!!!!!!!!!!!!!!!!!!%x, %x, %x\n", pxTopOfStack, pxCode, pvParameters);


	/* Simulate the stack frame as it would be created by a context switch
	interrupt. */
	pxTopOfStack--; /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
	*pxTopOfStack = portINITIAL_XPSR;	/* xPSR */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) pxCode;	/* PC */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0;	/* LR */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) pxOriginalStack;	/* SP */
	pxTopOfStack -= 12;		/* R1 through R12 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) pvParameters;	/* R0 */

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

void vPortSVCHandler( void )
{
	//em_printf("vPortSVCHandler!!!!!!!!!!!!!!!!!!!!!!\n");
	__asm volatile(
			" ldr r9, pxCurrentTCBConst2	\n"		/* Load the pxCurrentTCB pointer address. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" ldr lr, [r8]					\n"		/* Load the Task Stack Pointer into LR. */
			" ldmia lr, {r0-lr}^		 	\n"		/* Load the Task's registers. */
			" add lr, lr, #60			 	\n"		/* Re-adjust the stack for the Task Context */
			" nop						 	\n"
			" rfeia lr				 		\n"		/* Return from exception by loading the PC and CPSR from Task Stack. */
			" nop						 	\n"
			"								\n"
			"	.align 2					\n"
			" pxCurrentTCBConst2: .word pxCurrentTCB	\n"
			);
}
/*-----------------------------------------------------------*/

void vPortInterruptContext( void )
{
	//em_printf("vPortInterruptContext!!!!!!!!!!!!!!!!!!!!!!");
	__asm volatile(
			" sub lr, lr, #4				\n"		/* Adjust the return address. */
			" srsdb SP, #31					\n"		/* Store the return address and SPSR to the Task's stack. */
			" stmdb SP, {SP}^			 	\n"		/* Store the SP_USR to the stack. */
			" sub SP, SP, #4			 	\n"		/* Decrement the Stack Pointer. */
			" ldmia SP!, {lr}			 	\n"		/* Load the SP_USR into LR. */
			" sub LR, LR, #8 				\n"		/* Make room for the previously stored LR and CPSR. */
			" stmdb LR, {r0-lr}^	 		\n"		/* Store the Task's registers. */
			" sub LR, LR, #60		 		\n"		/* Adjust the Task's stack pointer. */
			" ldr r9, pxCurrentTCBConst2	\n"		/* Load the pxCurrentTCB pointer address. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" str lr, [r8]	 				\n"		/* Store the Task stack pointer to the TCB. */
			" bl vPortGICInterruptHandler	\n"		/* Branch and link to find specific service handler. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" ldr lr, [r8]					\n"		/* Load the Task Stack Pointer into LR. */
			" ldmia lr, {r0-lr}^		 	\n"		/* Load the Task's registers. */
			" add lr, lr, #60			 	\n"		/* Re-adjust the stack for the Task Context */
			" rfeia lr				 		\n"		/* Return from exception by loading the PC and CPSR from Task Stack. */
			" nop						 	\n"
			);
}
/*-----------------------------------------------------------*/

void em_SwitchContext( void )
{
	em_printf("Context Switch!!!!!!!!!!!!!!!!!!!!!");
	__asm volatile( 
			" sub lr, lr, #4                \n"     /* Adjust the return address. */
			" srsdb SP, #31                 \n"     /* Store the return address and SPSR to the Task's stack. */
			" stmdb SP, {SP}^               \n"     /* Store the SP_USR to the stack. */
			" sub SP, SP, #4                \n"     /* Decrement the Stack Pointer. */
			" ldmia SP!, {lr}               \n"     /* Load the SP_USR into LR. */
			" sub LR, LR, #8                \n"     /* Make room for the previously stored LR and CPSR. */
			" stmdb LR, {r0-lr}^            \n"     /* Store the Task's registers. */
			" sub LR, LR, #60               \n"     /* Adjust the Task's stack pointer. */
			" ldr r9, pxCurrentTCBConst2    \n"     /* Load the pxCurrentTCB pointer address. */
			" ldr r8, [r9]                  \n"     /* Load the pxCurrentTCB address. */
			" str lr, [r8]                  \n"     /* Store the Task stack pointer to the TCB. */
			" b vTaskSwitchContext         \n"     /* Branch and link to find specific service handler. */
			" ldr r8, [r9]					\n"		/* Load the pxCurrentTCB address. */
			" ldr lr, [r8]					\n"		/* Load the Task Stack Pointer into LR. */
			" ldmia lr, {r0-lr}^		 	\n"		/* Load the Task's registers. */
			" add lr, lr, #60			 	\n"		/* Re-adjust the stack for the Task Context */
			" rfeia lr				 		\n"		/* Return from exception by loading the PC and CPSR from Task Stack. */
			" nop						 	\n"
			);
	em_printf("Context S End\n");
}

void vPortStartFirstTask( void )
{
	//em_printf("vPortStartFirstTask!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	__asm volatile(
					" mov SP, %[svcsp]			\n" /* Set-up the supervisor stack. */
					" svc 0 					\n" /* Use the supervisor call to be in an exception. */
					" nop						\n"
					: : [pxTCB] "r" (pxCurrentTCB), [svcsp] "r" (puxSVCStackPointer) :
				);
}
/*-----------------------------------------------------------*/

void vPortSMCHandler( void )
{
	/* Nothing to do. */
}
/*-----------------------------------------------------------*/

void vPortYieldFromISR( void )
{
	vTaskSwitchContext();
	portSGI_CLEAR_YIELD( portGIC_DISTRIBUTOR_BASE, 0UL );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */

void tmp_init_gicd()
{
	int cpu_id = get_cpuid();
	int i = 0;
	int NIRQS = 160;
	gicd_disable_irq_forwarding();

	for(i;i<(NIRQS/32);i++){
		write_GICD_ICENABLER(i,0xffffffff);
	}   
	gicd_set_enable_irq(VIRTUAL_GENERIC_TIMER_IRQ);
	gicd_set_target_irq(VIRTUAL_GENERIC_TIMER_IRQ, cpu_id);
	gicd_set_priority_irq(VIRTUAL_GENERIC_TIMER_IRQ,0xA0);

	gicd_enable_irq_forwarding();
}

void tmp_init_gicc()
{
	gicc_disable_cpu_interface();
	gicc_set_priority_register(0xf0);
	gicc_enable_cpu_interface();
}

void tmp_init_gtimer()
{
	//em_printf("tmp_init_gtimer start\n");
	gt_set_virtual_timer_deadline(300000);
	gt_unmask_virtual_timer();
	gt_enable_virtual_timer();
	//em_printf("tmp_init_gtimer end\n");
}

portBASE_TYPE xPortStartScheduler( void )
{
	/* Start the timer that generates the tick ISR.  Interrupts are disabled
	here already. */

	prvSetupTimerInterrupt();
	em_printf("Generic Timer Setup!\n");
	/* Install the interrupt handler. */
	vPortInstallInterruptHandler( (void (*)(void *))vPortYieldFromISR, NULL, portSGI_YIELD_VECTOR_ID, pdTRUE, /* configMAX_SYSCALL_INTERRUPT_PRIORITY */ configKERNEL_INTERRUPT_PRIORITY, 1 );
	/* Finally, allow the GIC to pass interrupts to the processor. */

	//init gicd
	em_printf("GICD & GICC Initialization!\n");
	em_printf("Enable Generic Timer(PPI4, IRQ 27)\n");
	tmp_init_gicd();
	tmp_init_gicc();


	/* Start the first task. */
	em_printf("=== FreeRTOS Schedule Start ===\n");
	vPortStartFirstTask();

	/* Should not get here! */
	return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the CM3 port will require this function as there
	is nothing to return to.  */
}
/*-----------------------------------------------------------*/

void vPortSysTickHandler( void *pvParameter )
{
	//em_printf("new SysTick");
	gicd_clr_enable_irq(27);
	vTaskIncrementTick();


	gt_set_virtual_timer_deadline(300000);
	gt_unmask_virtual_timer();
	gicd_set_enable_irq(27);
	#if ( configUSE_PREEMPTION == 1 )
	{
		//EASW
		//All tasks enter Delay state, prvIdle task continuosly running.
		//So, We would switch Context from prvIdle
		
		vTaskSwitchContext();
		//portEND_SWITCHING_ISR(pdTRUE);
	}
	#endif
	//em_printf("end\n");
}

/*-----------------------------------------------------------*/

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void generic_timer_disable()
{
    //unsigned long long tval;
    unsigned int ctrl;
    ctrl = read_cntp_ctl();
    if (ctrl & GENERIC_TIMER_CTRL_ISTATUS) {
        ctrl |= GENERIC_TIMER_CTRL_IMASK;
        write_cntp_ctl(ctrl);
    }
}

void generic_timer_enable()
{
    unsigned long long tval;
    unsigned int ctrl;
    ctrl = read_cntp_ctl();
    ctrl |= GENERIC_TIMER_CTRL_ENABLE;
    ctrl &= ~GENERIC_TIMER_CTRL_IMASK;
    tval = SCHED_TICK * COUNT_PER_USEC;
    write_cntp_tval(tval);
    write_cntp_ctl(ctrl);
}

extern void vTimer0InterruptHandler( void *pvParameter );
extern void vTimer0Initialise( unsigned long ulLoadValue );
extern void vTimer0Enable();

void prvSetupTimerInterrupt( void )
{
	tmp_init_gtimer();

	vPortInstallInterruptHandler( vPortSysTickHandler, NULL, 27, pdTRUE, /* configMAX_SYSCALL_INTERRUPT_PRIORITY */ configKERNEL_INTERRUPT_PRIORITY, 1 );
}

/*-----------------------------------------------------------*/


void vPortInstallInterruptHandler( void (*vHandler)(void *), void *pvParameter, unsigned long ulVector, unsigned char ucEdgeTriggered, unsigned char ucPriority, unsigned char ucProcessorTargets )
{
char str[64];
unsigned long ulBank32 = 4 * ( ulVector / 32 );
unsigned long ulOffset32 = ulVector % 32;
unsigned long ulBank4 = 4 * ( ulVector / 4 );
unsigned long ulOffset4 = ulVector % 4;
unsigned long ulBank16 = 4 * ( ulVector / 16 );
unsigned long ulOffset16 = ulVector % 16;
unsigned long puxGICAddress = 0;
unsigned long puxGICDistributorAddress = 0;

	/* Select which GIC to use. */
	puxGICAddress = portGIC2_BASE;
	puxGICDistributorAddress = portGIC2_DISTRIBUTOR_BASE;

	/* Record the Handler. */
	if (ulVector < ulMaxVectorId )
	{
		pxInterruptHandlers[ ulVector ].vHandler = vHandler;
		pxInterruptHandlers[ ulVector ].pvParameter = pvParameter;

		/* Now calculate all of the offsets for the specific GIC. */
		ulBank32 = 4 * ( ulVector / 32 );
		ulOffset32 = ulVector % 32;
		ulBank4 = 4 * ( ulVector / 4 );
		ulOffset4 = ulVector % 4;
		ulBank16 = 4 * ( ulVector / 16 );
		ulOffset16 = ulVector % 16;

		/* First make the Interrupt a Secure one. */
		//*(portGIC_ICDISR_BASE(puxGICDistributorAddress) + ulBank32) &= ~( 1 << ulOffset32 );

		/* Is it Edge Triggered?. */
		if ( 0 != ucEdgeTriggered )
		{
            portGIC_SET( (portGIC_ICDICR_BASE(puxGICDistributorAddress + ulBank16) ), ( portGIC_READ( portGIC_ICDICR_BASE(puxGICDistributorAddress + ulBank16)) | ( 0x02 << ( ulOffset16 * 2 ) ) ) );
		}

		/* Set the Priority. */
		portGIC_WRITE( portGIC_ICDIPR_BASE(puxGICDistributorAddress) + ulBank4, ( ( (unsigned long)ucPriority ) << ( ulOffset4 * 8 ) ) );

		/* Set the targeted Processors. */
		portGIC_WRITE( portGIC_ICDIPTR_BASE(puxGICDistributorAddress + ulBank4), ( ( (unsigned long)ucProcessorTargets ) << ( ulOffset4 * 8 ) ) );

		/* Enable the Interrupt. */
		if ( NULL != vHandler )
		{
			portGIC_SET( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
			portGIC_SET( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
		}
		else
		{
			/* Or disable when passed a NULL handler. */
			portGIC_CLEAR( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDICPR_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
			portGIC_CLEAR( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32), ( portGIC_READ( portGIC_ICDISER_BASE(puxGICDistributorAddress + ulBank32) ) | ( 1 << ulOffset32 ) ) );
		}
	}
}
/*----------------------------------------------------------------------------*/

void vPortGICInterruptHandler( void )
{
unsigned long ulVector = 0UL;
unsigned long ulGICBaseAddress = portGIC_PRIVATE_BASE;
char cAddress[64];
unsigned long ulValue = 0UL;
	//em_printf("GICInterruptHandler \n");
	/* Query the private address first. */
	ulVector = portGIC_READ( portGIC_ICCIAR(portGIC_PRIVATE_BASE) );
	//em_printf("ulVector = %d\n", ulVector);
	if ( portGIC_SPURIOUS_VECTOR == ( ulVector & portGIC_VECTOR_MASK ) )
	{
		/* Query the private address first. */
        //vSerialPutString(0, (const signed char * const)"ISR\r\n", 5 );
		ulGICBaseAddress = portGIC2_BASE;
	}

	if ( ( ( ulVector & portGIC_VECTOR_MASK ) < ulMaxVectorId ) && ( NULL != pxInterruptHandlers[ ( ulVector & portGIC_VECTOR_MASK ) ].vHandler ) )
	{
        //sprintf( cAddress, "IRQ: %d\r\n", ulVector );
        //vSerialPutString(configUART_PORT,cAddress, strlen(cAddress) );
		/* Call the associated handler. */
		pxInterruptHandlers[ ( ulVector & portGIC_VECTOR_MASK ) ].vHandler( pxInterruptHandlers[ ( ulVector & portGIC_VECTOR_MASK ) ].pvParameter );

		/* And acknowledge the interrupt. */
		portGIC_WRITE( portGIC_ICCEOIR(ulGICBaseAddress), ulVector );
	}
	else
	{
		/* This is a spurious interrupt, do nothing. */
	}
}
/*----------------------------------------------------------------------------*/

portBASE_TYPE xPortSetInterruptMask( void )
{
    portBASE_TYPE xPriorityMask = portGIC_READ( portGIC_ICCPMR(portGIC_PRIVATE_BASE) );
	portGIC_WRITE( portGIC_ICCPMR(portGIC_PRIVATE_BASE), configMAX_SYSCALL_INTERRUPT_PRIORITY );
	return xPriorityMask;
}
/*----------------------------------------------------------------------------*/

void vPortClearInterruptMask( portBASE_TYPE xPriorityMask )
{
	portGIC_WRITE( portGIC_ICCPMR(portGIC_PRIVATE_BASE), xPriorityMask );
}
/*----------------------------------------------------------------------------*/

void vPortUnknownInterruptHandler( void *pvParameter )
{
	/* This is an unhandled interrupt, do nothing. */
	(void)pvParameter;
}
/*----------------------------------------------------------------------------*/

void _init(void)
{
extern int main( void );
unsigned long *pulSrc, *pulDest;
volatile unsigned long ulSCTLR = 0UL;
/*
extern unsigned long __isr_vector_start;
extern unsigned long __isr_vector_end;
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _bss;
extern unsigned long _ebss;
*/
unsigned long __isr_vector_start;
unsigned long __isr_vector_end;
unsigned long _etext;
unsigned long _data;
unsigned long _edata;
unsigned long _bss;
unsigned long _ebss;
	/* Copy the data segment initializers from flash to SRAM. */
	pulSrc = &_etext;
	for(pulDest = &_data; pulDest < &_edata; )
	{
		*pulDest++ = *pulSrc++;
	}

	/* Zero fill the bss segment. */
	for(pulDest = &_bss; pulDest < &_ebss; )
	{
		*pulDest++ = 0;
	}

	/* Configure the Stack Pointer for the Processor Modes. */
	__asm volatile (
			" cps #17							\n"
			" nop 								\n"
			" mov SP, %[fiqsp]					\n"
			" nop 								\n"
			" cps #18							\n"
			" nop 								\n"
			" mov SP, %[irqsp]					\n"
			" nop 								\n"
#if configPLATFORM == 1
			" cps #22							\n"
			" nop 								\n"
			" mov SP, %[abtsp]					\n"
			" nop 								\n"
#endif /* configPLATFORM */
			" cps #23							\n"
			" nop 								\n"
			" mov SP, %[abtsp]					\n"
			" nop 								\n"
			" cps #27							\n"
			" nop 								\n"
			" mov SP, %[abtsp]					\n"
			" nop 								\n"
			" cps #19							\n"
			" nop 								\n"
			" mov SP, %[svcsp]					\n"
			" nop 								\n"
			: : [fiqsp] "r" (puxFIQStackPointer),
				[irqsp] "r" (puxIRQStackPointer),
				[abtsp] "r" (puxAbortStackPointer),
				[svcsp] "r" (puxSVCStackPointer)
				:  );

	/* Finally, copy the exception vector table over the boot loader. */
	pulSrc = (unsigned long *)&__isr_vector_start;
	pulDest = (unsigned long *)portEXCEPTION_VECTORS_BASE;
	for ( pulSrc = &__isr_vector_start; pulSrc < &__isr_vector_end; )
	{
		*pulDest++ = *pulSrc++;
	}

	/* VBAR is modified to point to the new Vector Table. */
	pulDest = (unsigned long *)portEXCEPTION_VECTORS_BASE;
	__asm volatile(
			" mcr p15, 0, %[vbar], c12, c0, 0 			\n"
			: : [vbar] "r" (pulDest) :
			);

	/* Read Configuration Register C15, c0, 0. */
	__asm volatile(
			" mrc p15, 0, %[sctlr], c1, c0, 0 			\n"
			: [sctlr] "=r" (ulSCTLR) : :
			);

	/* Now we modify the SCTLR to change the Vector Table Address. */
	ulSCTLR &= ~( 1 << 13 );

	/* Write Configuration Register C15, c0, 0. */
	__asm volatile(
			" mcr p15, 0, %[sctlr], c1, c0, 0 			\n"
			: : [sctlr] "r" (ulSCTLR) :
			);

#if configPLATFORM == 1
	/* Now set-up the Monitor Mode Vector Table. */
	pulSrc = (unsigned long *)&__isr_vector_start;
	pulSrc[ 2 ] = (unsigned long)vPortSMCHandler;
	__asm volatile(
			" cps #22 									\n"
			" nop 										\n"
			" mcr p15, 0, %[vbar], c12, c0, 1 			\n"
			" mcr p15, 0, %[sctlr], c1, c0, 0 			\n"
			" cps #19 "
			: : [vbar] "r" (pulSrc), [sctlr] "r" (ulSCTLR) :
			);
#endif /* configPLATFORM */
}
/*----------------------------------------------------------------------------*/

void Undefined_Handler_Panic( void )
{
	__asm volatile ( " smc #0 " );
	for (;;);
}
/*----------------------------------------------------------------------------*/

void Prefetch_Handler_Panic( void )
{
	__asm volatile ( " smc #0 " );
	for (;;);
}
/*----------------------------------------------------------------------------*/

void Abort_Handler_Panic( void )
{
//	__asm volatile ( " smc #8 " );
	for (;;);
}
/*----------------------------------------------------------------------------*/

