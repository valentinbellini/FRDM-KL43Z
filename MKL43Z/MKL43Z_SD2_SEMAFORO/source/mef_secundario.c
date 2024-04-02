#include "mef_secundario.h"
#include <stdint.h>
#include "SD2_board.h"

/*==================[macros and typedef]====================================*/

#define DURACION_EST_SECUNDARIO_1   5000	// 5seg		
#define DURACION_EST_SECUNDARIO_3   5000	// 5seg		
#define DURACION_BLINK_LED_EST_1    200     // 200ms
#define DURACION_BLINK_LED_EST_3    200     // 200ms

typedef enum{
	EST_SECUNDARIO_1 = 0,
	EST_SECUNDARIO_2,
	EST_SECUNDARIO_3
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
    timSec_secundario = DURACION_EST_SECUNDARIO_1;
    timBlink_secundario = DURACION_BLINK_LED_EST_1;
	estado_MEF_secundario = EST_SECUNDARIO_1;
}


extern bool mef_secundario(void){
    switch(estado_MEF_secundario){
        case EST_SECUNDARIO_1:
            board_setLed(LRS, BOARD_LED_MSG_ON);
		    board_setLed(LRR, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timBlink_secundario <= 0){
                timBlink_secundario = DURACION_BLINK_LED_EST_1;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_secundario <= 0){
                estado_MEF_secundario = EST_SECUNDARIO_2
            }
            break;
        case EST_SECUNDARIO_2:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVS, BOARD_LED_MSG_ON);
            if(getAutos() <= 0){ /* Definir getAutos() que devuelva un entero de autos contadoss */
                timSec_secundario = DURACION_EST_SECUNDARIO_3;
                timBlink_secundario = DURACION_BLINK_LED_EST_3;
                estado_MEF_secundario = EST_SECUNDARIO_3;
            }
            break;
        case EST_SECUNDARIO_3:
            board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVR, BOARD_LED_MSG_OFF);
            if(timBlink_secundario){
                timBlink_secundario = DURACION_BLINK_LED_EST_3;
                board_setLed(LRR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_secundario <= 0){
                return true;
            }
            break;
    }
    return false;
}

extern void mef_secundario_task1ms(void){ /* Cada 1ms decrementa los contadores de tiempo */
    if(timSec_secundario) timSec_secundario--;
    if(timBlink_secundario) timBlink_secundario--;
}

/*==================[end of file]============================================*/

