#include "mef_secundario.h"
#include <stdint.h>
#include "SD2_board.h"
#include "key.h"
#include "cont_autos.h"


/*==================[macros and typedef]====================================*/

#define DURACION_EST_BLINK_1   		5000	// 5seg
#define DURACION_EST_BLINK_2   		5000	// 5seg
#define DURACION_BLINK    			200     // 200ms

typedef enum{
	EST_BLINK_1 = 0, 	// Estado inicial: Blink LVR
	EST_PASO,			// Estado paso: Habilitacion camino secundario
	EST_BLINK_2			// Estado blink_2: Blink LVS
} estMefSecundario_enum;

/*==================[internal data definition]==============================*/

static estMefSecundario_enum estado_MEF_secundario;
static uint32_t timSec_secundario;
static uint32_t timBlink_secundario;

/*==================[external functions definition]=========================*/

extern void mef_secundario_init(void){
	mef_secundario_reset();
}

extern void mef_secundario_reset(void){
    timSec_secundario = DURACION_EST_BLINK_1;
    timBlink_secundario = DURACION_BLINK;
	estado_MEF_secundario = EST_BLINK_1;
}

extern bool mef_secundario(void){
    switch(estado_MEF_secundario){
        case EST_BLINK_1:
            board_setLed(LRS, BOARD_LED_MSG_ON);
		    board_setLed(LRR, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timBlink_secundario == 0){
                timBlink_secundario = DURACION_BLINK;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_secundario == 0){
                estado_MEF_secundario = EST_PASO;
            }
            break;
        case EST_PASO:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVS, BOARD_LED_MSG_ON);
		    count_updateCarCount(BOARD_SW_ID_3, RESTAR);
            if(count_getCarCount(BOARD_SW_ID_3) == 0){
                timSec_secundario = DURACION_EST_BLINK_2;
                timBlink_secundario = DURACION_BLINK;
                estado_MEF_secundario = EST_BLINK_2;
            }
            break;
        case EST_BLINK_2:
            board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVR, BOARD_LED_MSG_OFF);
            if(timBlink_secundario == 0){
                timBlink_secundario = DURACION_BLINK;
                board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_secundario == 0){
                return true;
            }
            break;
    }
    return false;
}

extern void mef_secundario_task1ms(void){ 		/* Cada 1ms decrementa los contadores de tiempo */
    if(timSec_secundario) timSec_secundario--;
    if(timBlink_secundario) timBlink_secundario--;
//    if(estado_MEF_secundario == EST_PASO){
//    	count_updateCarCount(BOARD_SW_ID_3, RESTAR);
//    }
}

/*==================[end of file]============================================*/

