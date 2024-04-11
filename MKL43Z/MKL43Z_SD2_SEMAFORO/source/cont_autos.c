/*==================[inclusions]=============================================*/
#include "cont_autos.h"
#include "key.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
static uint8_t car_count;

/*==================[external functions definition]==========================*/
void count_updateCarCount(board_swId_enum id, car_count_mode_enum mode){
	if(key_getPressEv(BOARD_SW_ID_3)){
		if(mode == SUMAR)	car_count++;
		else{
			if(car_count)	car_count--;
		}
	}
}

uint8_t count_getCarCount(board_swId_enum id){
	return car_count;
}

void count_resetCarCount(board_swId_enum id){
	car_count = 0;
}
