/**
 * @file    main.c
 * @brief   Application entry point.
 */

/*==================[inclusions]=============================================*/
#include <MEFs/mef_modo.h>
#include <stdio.h>
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"

#include "SD2_board.h"
#include "board.h"
#include "key.h"

/*
 * @brief   Application entry point.
 */
int main(void) {
	/* Inicializaciones */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	    BOARD_InitDebugConsole();
	#endif
	board_init();
	key_init();
	mef_modo_init();

	/* interrupción de systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);

	while (1){
		mef_modo();
	}
	return 0 ;
}


void SysTick_Handler(void){
	key_periodicTask1ms();
	mef_modo_task1ms();
}
