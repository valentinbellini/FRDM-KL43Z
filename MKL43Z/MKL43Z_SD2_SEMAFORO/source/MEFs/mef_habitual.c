#include "mef_habitual.h"
#include <stdint.h>
#include "SD2_board.h"
#include "key.h"
#include "cont_autos.h"

/*==================[macros and typedef]====================================*/

// time duration macros definition
#define DURACION_SEC_1				20000
#define DURACION_SEC_2				5000
#define DURACION_SEC_3				10000
#define DURACION_SEC_4				5000
#define DURACION_BLINK    			100
#define CAR_COUNT_MAX				3		// Cantidad de autos en espera para habilitar camino secundario.

typedef enum{
	EST_SEC_1 = 0, 	// Paso por la ruta principal (LVR ON)
	EST_SEC_2,		// Blink LVR indicando cambio de paso
	EST_SEC_3,		// Paso por el camino secundario (LRR ON)
    EST_SEC_4,		// Blink LVS indicando cambio de paso
	EST_SEC_5		// Idem SEC_1 pero no se puede pasar a la MEF_PEATONAL
} estMefHabitual_enum;

/*==================[internal data definition]==============================*/

static estMefHabitual_enum estado_MEF_habitual;
static uint32_t timSec_habitual;
static uint32_t timBlink_habitual;

/*==================[external functions definition]=========================*/

extern void mef_habitual_init(){
	mef_habitual_reset();
}

extern void mef_habitual_reset(){
	timSec_habitual = DURACION_SEC_1;
	estado_MEF_habitual = EST_SEC_1;
	key_clearFlags(BOARD_SW_ID_1);
}

extern void mef_habitual_init_sec_5(){
	estado_MEF_habitual = EST_SEC_5;
	count_resetCarCount(BOARD_SW_ID_3);
}

extern tr_enum mef_habitual(void){
    switch(estado_MEF_habitual){
        case EST_SEC_1:
            board_setLed(LVR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            count_updateCarCount(BOARD_SW_ID_3, SUMAR); // Suma si la bandera del interruptor lo dice.
            if(timSec_habitual == 0){
                timSec_habitual = DURACION_SEC_2;
                estado_MEF_habitual = EST_SEC_2;
                timBlink_habitual = DURACION_BLINK;
            }
            if(key_getPressEv(BOARD_SW_ID_1)) return TR_TO_PEATON;
            else if(count_getCarCount(BOARD_SW_ID_3) >= CAR_COUNT_MAX)return TR_TO_SECUNDARIO;
            break;
        case EST_SEC_2:
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timBlink_habitual == 0){
            	board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
                timBlink_habitual = DURACION_BLINK;
            }
            if(timSec_habitual == 0){
            	timSec_habitual = DURACION_SEC_3;
                estado_MEF_habitual = EST_SEC_3;
            }
            break;
        case EST_SEC_3:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LVS, BOARD_LED_MSG_ON);
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            if(timSec_habitual == 0){
                timSec_habitual = DURACION_SEC_4;
                estado_MEF_habitual = EST_SEC_4;
                timBlink_habitual = DURACION_BLINK;
            }
            break;
        case EST_SEC_4:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            if(timBlink_habitual == 0){
            	board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
                timBlink_habitual = DURACION_BLINK;
            }
            if(timSec_habitual == 0){
            	key_clearFlags(BOARD_SW_ID_1);
            	key_clearFlags(BOARD_SW_ID_3);
            	count_resetCarCount(BOARD_SW_ID_3);
                timSec_habitual = DURACION_SEC_1;
                estado_MEF_habitual = EST_SEC_1;
            }
            break;
        case EST_SEC_5: // Se entra solo si se ejecuta mef_habitual_init_sec_5()
        	board_setLed(LVR, BOARD_LED_MSG_ON);
        	board_setLed(LRS, BOARD_LED_MSG_ON);
        	board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            count_updateCarCount(BOARD_SW_ID_3, SUMAR); // Suma si la bandera del interruptor lo dice.
            if(timSec_habitual == 0){
            	timSec_habitual = DURACION_SEC_2;
            	estado_MEF_habitual = EST_SEC_2;
            	timBlink_habitual = DURACION_BLINK;
            }
            if(count_getCarCount(BOARD_SW_ID_3) >= CAR_COUNT_MAX) return TR_TO_SECUNDARIO;
            break;
    }
    return TR_NONE;
}

extern void mef_habitual_task1ms(void){
    if(timSec_habitual) timSec_habitual--;
    if(timBlink_habitual) timBlink_habitual--;
}

/*==================[end of file]============================================*/

