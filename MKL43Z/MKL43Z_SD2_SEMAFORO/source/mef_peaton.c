
#include "mef_peaton.h"
#include <stdint.h>
#include "SD2_board.h"

/*==================[macros and typedef]====================================*/

#define DURACION_EST_1 10000	//10seg		
#define DURACION_EST_2 60000	//1min		
#define DURACION_EST_3 10000    //10seg		

typedef enum{
	EST_PEATON_1 = 0,
	EST_PEATON_2,
	EST_PEATON_3
} estMefPeaton_enum;

/*==================[internal data definition]==============================*/

static estMefPeaton_enum estado_MEF_peaton;
static uint32_t timSec_peaton;
static uint32_t timBlink_peaton;

/*==================[external functions definition]=========================*/

extern void mef_peaton_init(void){
    mef_peaton_reset();
}

extern void mef_peaton_reset(void){
    timSec_peaton = DURACION_EST_1;
    timBlink_peaton = 200;
    estado_MEF_peaton = EST_PEATON_1;
    board_setLed(LRS, BOARD_LED_MSG_OFF);
    board_setLed(LRR, BOARD_LED_MSG_OFF);
	board_setLed(LVS, BOARD_LED_MSG_OFF);
    board_setLed(LVR, BOARD_LED_MSG_OFF);
}

extern bool mef_peaton(void){ /* Bool porque tengo que evaluarla para ver si salgo o no*/
    switch(estado_MEF_peaton){
        case EST_PEATON_1:
            board_setLed(LRS, BOARD_LED_MSG_ON);
		    board_setLed(LRR, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_OFF);
            /* ¿Cuento autos acá ? */
            if(timBlink_peaton <= 0){
                timBlink_peaton = 200;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_peaton <= 0) {
                timSec_peaton = DURACION_EST_2;
                estado_MEF_peaton = EST_PEATON_2;
            }
            break;
        case EST_PEATON_2:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVS, BOARD_LED_MSG_ON);
            if(timSec_peaton <= 0) {
                timSec_peaton = DURACION_EST_3;
                estado_MEF_peaton = EST_PEATON_3
            }
            break;
        case EST_PEATON_3:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_ON);
            if(timBlink_peaton <= 0){
                timBlink_peaton = 200;
                board_setLed(LRR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_peaton <= 0){
                return true;
            }
            break;
    }
    return false;
}

extern void mef_peaton_task1ms(void){
    if(timSec_peaton) timSec_peaton--;
    if(timBlink_peaton) timBlink_peaton--;
}

/*==================[end of file]============================================*/
