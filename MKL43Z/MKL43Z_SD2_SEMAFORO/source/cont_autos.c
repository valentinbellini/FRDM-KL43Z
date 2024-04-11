/*==================[inclusions]=============================================*/
#include "cont_autos.h"
#include "key.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
static uint8_t count_pressed;

/*==================[external functions definition]==========================*/
void count_updateCarCount(board_swId_enum id, key_count_mode_enum mode){
	if(key_getPressEv(BOARD_SW_ID_3)){
		if(mode == SUMAR)	count_pressed++;
		else{
			if(count_pressed)	count_pressed--;
		}
	}
}

uint8_t count_getCarCount(board_swId_enum id){
	return count_pressed;
}

void count_setCarCount(board_swId_enum id, uint8_t setValue){
	count_pressed= setValue;
}
