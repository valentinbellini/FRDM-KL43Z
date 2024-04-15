/**
 * @file    cont_autos.c
 * @brief   Car counting thru the switches.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/
#include "cont_autos.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

static uint8_t car_count[BOARD_SW_ID_TOTAL];	// One car counter for every switch in the array definition (SD_board.c)

/*==================[external functions definition]==========================*/

/* updateCarCount update the selected switch count with the selected mode. */
void count_updateCarCount(board_swId_enum id, car_count_mode_enum mode){
	if(mode == SUMAR)	car_count[id]++;
	else{
		if(car_count[id])	car_count[id]--;
	}
}

uint8_t count_getCarCount(board_swId_enum id){
	return car_count[id];
}

void count_resetCarCount(board_swId_enum id){
	car_count[id] = 0;
}
