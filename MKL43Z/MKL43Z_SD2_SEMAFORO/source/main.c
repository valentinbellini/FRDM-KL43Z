/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <MEFs/mef_modo.h>
#include <stdio.h>
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include "SD2_board.h"
#include "board.h"
#include "key.h"
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

	board_init();
	BOARD_InitDebugConsole();
	key_init();

	/* interrupción de systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);

	mef_modo_init();

	while (1)
	{
		mef_modo();
	}
	return 0 ;
}


void SysTick_Handler(void){
	key_periodicTask1ms();
	mef_modo_task1ms();
}
