/**
 * @file    cont_autos.c
 * @brief   Car counting thru the switches.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/
#include "cont_autos.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

static uint8_t car_count[BOARD_SW_ID_TOTAL]; // Contador de autos como array

/*==================[external functions definition]==========================*/

/* updateCarCount actualiza el valor de la cuenta del pulsador que enviamos según el MODO que enviamos */
void count_updateCarCount(board_swId_enum id, car_count_mode_enum mode){
	if(key_getPressEv(id)){
		if(mode == SUMAR)	car_count[id]++;
		else{
			if(car_count[id])	car_count[id]--;
		}
	}
}

uint8_t count_getCarCount(board_swId_enum id){
	return car_count[id];
}

void count_resetCarCount(board_swId_enum id){
	car_count[id] = 0;
}
