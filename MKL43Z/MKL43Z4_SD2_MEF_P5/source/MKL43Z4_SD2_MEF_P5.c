
/* --------------- [Inclusions] --------------- */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"
#include "SD2_board.h"
#include "key.h"
#include "mefLed.h"

int main(void) {

    BOARD_InitBootClocks();
    board_init();
    key_init();
    mefLed_init();

    /* Configuración de Systick 1 ms */
    SysTick_Config(SystemCoreClock / 1000U);

    while(1){
    	mefLed();
    }

    return 0;
}


void SysTick_Handler(void)
{
	key_periodicTask1ms();
	mefLed_task1ms();
}
