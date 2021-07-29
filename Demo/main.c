/*
    FreeRTOS V7.0.1 - Copyright (C) 2011 Real Time Engineers Ltd.


	FreeRTOS supports many tools and architectures. V7.0.0 is sponsored by:
	Atollic AB - Atollic provides professional embedded systems development
	tools for C/C++ development, code analysis and test automation.
	See http://www.atollic.com


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

/* Standard includes. */
//#include <stdio.h>
#include <string.h>

///////////////////////////////////////
#include <gtimer.h>
#include <io-tk1.h>
///////////////////////////////////////

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Demo Includes. */
/*----------------------------------------------------------------------------*/

/* Delay between cycles of the 'check' task. */
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )

/* Demo task priorities. */
#define mainQUEUE_POLL_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY		( tskIDLE_PRIORITY + 3 )
#define mainSEM_TEST_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainGEN_Q_PRIORITY			( tskIDLE_PRIORITY )

/* Misc. */
#define mainQUEUE_SIZE				( 3 )
#define mainNO_DELAY				( ( portTickType ) 0 )
#define mainPRINT_PORT				( 0 )
/*----------------------------------------------------------------------------*/


/* Periodically checks to see whether the demo tasks are still running. */
static void IdleCheck( void *pvParameters );
static void em_Check2( void *pvParameters );
static void em_Check3( void *pvParameters );
static void led0( void *pvParameters );
static void led1( void *pvParameters );
static void led2( void *pvParameters );
static void vCheckTask( void *pvParameters );
/*----------------------------------------------------------------------------*/

/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );
/*----------------------------------------------------------------------------*/

void debug(struct gpio_bank *bank, int port)
{
	em_printf("\n === PORT debug === \n");
	em_printf("PORT base : %x\n", bank);
	em_printf("PORT CNF : %x\n", readgpio(&bank->cnf[port]));
	em_printf("PORT OE base : %x\n", &bank->oe[port]);
	em_printf("PORT OE : %x\n", readgpio(&bank->oe[port]));
	em_printf("PORT OUT base : %x\n", &bank->out[port]);
	em_printf("PORT OUT : %x\n", readgpio(&bank->out[port]));
	/*
	em_printf("PORT msk base : %x\n", &bank->msk_cnf[port]);
	em_printf("PORT msk CNF : %x\n", readgpio(&bank->msk_cnf[port]));
	em_printf("PORT msk OE base : %x\n", &bank->msk_oe[port]);
	em_printf("PORT msk OE : %x\n", readgpio(&bank->msk_oe[port]));
	em_printf("PORT msk OUT base : %x\n", &bank->msk_out[port]);
	em_printf("PORT msk OUT : %x\n", readgpio(&bank->msk_out[port]));
	*/
	em_printf("\n === PORT debug === \n");
}


int main( void )
{
	int i = 0;
	char str[64];
	em_printf("Start FreeRTOS!\n");
	/* Initialise the Hardware. */
	//prvSetupHardware();
	//	* Start the tasks defined within the file. 
	em_printf("Create Tasks!\n");

	
	struct gpio_bank *gpio2 = (struct gpio_bank *)GPIO_CONTROLLER_2;
	struct gpio_bank *gpio3 = (struct gpio_bank *)GPIO_CONTROLLER_3;
	struct gpio_bank *gpio4 = (struct gpio_bank *)GPIO_CONTROLLER_4;
	struct gpio_bank *gpio5 = (struct gpio_bank *)GPIO_CONTROLLER_5;
	struct gpio_bank *gpio6 = (struct gpio_bank *)GPIO_CONTROLLER_6;
	struct gpio_bank *gpio8 = (struct gpio_bank *)GPIO_CONTROLLER_8;

	//debug(gpio6, U);
		gpio_cfg_pin(gpio5, PT, 5, GPIO_OUTPUT);
		debug(gpio5, PT);
		gpio_cfg_pin(gpio8, PFF, 0, GPIO_OUTPUT);
		debug(gpio8, PFF);

		for(i = 0; i < 8; i++)
		{
			gpio_cfg_pin(gpio6, PU, i, GPIO_OUTPUT);
		}
		gpio_set_value(gpio5, PT, 5, LOW);


	//xTaskCreate( IdleCheck, (const signed char *)"IdleCheck", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	xTaskCreate( em_Check2, (const signed char *)"Check2", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	//xTaskCreate( em_Check3, (const signed char *)"Check3", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	
	xTaskCreate( led0, (const signed char *)"led0", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	xTaskCreate( led1, (const signed char *)"led1", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	xTaskCreate( led2, (const signed char *)"led2", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );

	//em_printf("[%s]: %d\r\n", __func__, __LINE__ );
	//vSerialPutString(configUART_PORT, str, strlen(str) );

	vTaskStartScheduler();

			/* Should never reach here. */
			//vSerialPutString(mainPRINT_PORT, (const signed char * const)"Should never reach here!\r\n", 26 );

			/* Will only get here if there was not enough heap space to create the idle task. */
			return 0;
}
/*----------------------------------------------------------------------------*/

void vApplicationTickHook( void )
{
}

/*----------------------------------------------------------------------------*/


void vApplicationIdleHook( void )
{
}

/*----------------------------------------------------------------------------*/
static void IdleCheck( void *pvParameters )
{
	int i = 0;
	//em_printf("Idle Task\n");
	for(i = 0;;i++)
	{
		//em_printf("Idle Task\n");
		vTaskDelay(50);
	}
}
static void em_Check2( void *pvParameters )
{
	int i = 0;
	struct gpio_bank *gpio5 = (struct gpio_bank *)GPIO_CONTROLLER_5;
	for(i = 0;;i++)
	{
		qph_gpio_set_value(gpio5, PT, 5, HIGH);
		//em_printf("HIGH : %x \n", readgpio(&gpio5->out[PT]));
		vTaskDelay(20);
		qph_gpio_set_value(gpio5, PT, 5, LOW);
		//em_printf("LOW : %x \n", readgpio(&gpio5->out[PT]));
		vTaskDelay(20);
	}
}
static void em_Check3( void *pvParameters )
{
	int i = 0;
	struct gpio_bank *gpio8 = (struct gpio_bank *)GPIO_CONTROLLER_8;
	for(i = 0;;i++)
	{
		qph_gpio_set_value(gpio8, PFF, 0, HIGH);
		//em_printf("Check 333 HIGH : %x \n", readgpio(&gpio8->out[PFF]));
		vTaskDelay(20);
		qph_gpio_set_value(gpio8, PFF, 0, LOW);
		//em_printf("Check 333 LOW : %x \n", readgpio(&gpio8->out[PFF]));
		vTaskDelay(20);
	}
}

static void led0( void *pvParameters)
{
	int i = 0;
	struct gpio_bank *gpio6 = (struct gpio_bank *)GPIO_CONTROLLER_6;
	for(;;)
	{
		qph_gpio_set_value(gpio6, PU, 0, HIGH);
		//em_printf("=== led0 HIGH === \n");
		vTaskDelay(20);
		qph_gpio_set_value(gpio6, PU, 0, LOW);
		//em_printf("=== led0 LOW === \n");
		vTaskDelay(20);
	}
}
static void led1( void *pvParameters)
{
	int i = 0;
	struct gpio_bank *gpio6 = (struct gpio_bank *)GPIO_CONTROLLER_6;
	for(;;)
	{
		qph_gpio_set_value(gpio6, PU, 1, HIGH);
		//em_printf("led1 HIGH\n");
		vTaskDelay(25);
		qph_gpio_set_value(gpio6, PU, 1, LOW);
		//em_printf("led1 LOW\n");
		vTaskDelay(25);
	}
}
static void led2( void *pvParameters)
{
	int i = 0;
	struct gpio_bank *gpio6 = (struct gpio_bank *)GPIO_CONTROLLER_6;
	for(;;)
	{
		qph_gpio_set_value(gpio6, PU, 2, HIGH);
		//em_printf("led2 HIGH\n");
		//debug(gpio6, PU);
		vTaskDelay(150);
		qph_gpio_set_value(gpio6, PU, 2, LOW);
		//em_printf("led2 LOW\n");
		//debug(gpio6, PU);
		vTaskDelay(150);
	}
}
static void vCheckTask( void *pvParameters )
{
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	unsigned long ulVector = 0UL;
	char str[64];


	portDISABLE_INTERRUPTS();

	// Install the Spurious Interrupt Handler to help catch interrupts. 
	extern void vPortUnknownInterruptHandler( void *pvParameter );
	extern void vPortInstallInterruptHandler( void (*vHandler)(void *), void *pvParameter, unsigned long ulVector, unsigned char ucEdgeTriggered, unsigned char ucPriority, unsigned char ucProcessorTargets );
	for ( ulVector = 0; ulVector < 160; ulVector++ )
		vPortInstallInterruptHandler( vPortUnknownInterruptHandler, (void *)ulVector, ulVector, pdTRUE, configMAX_SYSCALL_INTERRUPT_PRIORITY, 1 );


	/* Perform any other peripheral configuration. */
}
/*----------------------------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	__asm volatile (" smc #0 ");
}
/*----------------------------------------------------------------------------*/
