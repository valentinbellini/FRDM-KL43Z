/**
 * @file    mef_habitual.c
 * @brief   MEF for Normal Operation
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "mef_habitual.h"
#include "SD2_board.h"
#include "key.h"
#include "cont_autos.h"

/*==================[macros and typedef]====================================*/

// time duration macros definition
#define DURATION_SEC_1				20000
#define DURATION_SEC_2				5000
#define DURATION_SEC_3				10000
#define DURATION_SEC_4				5000
#define DURATION_BLINK    			100
#define CAR_COUNT_MAX				3		// Number of cars needed to enable secondary road.
// Times in mS.

typedef enum{
	EST_SEC_1 = 0, 	// "RUTA PRINCIPAL" enabled (LVR ON).
	EST_SEC_2,		// Blink LVR indicating change of enablement.
	EST_SEC_3,		// "CAMINO SECUNDARIO" enabled (LRR ON).
    EST_SEC_4,		// Blink LVS indicating change of enablement.
	EST_SEC_5		// Idem SEC_1 but you can't move on to MEF_PEATONAL
} estMefHabitual_enum;

/*==================[internal data definition]==============================*/

static estMefHabitual_enum estado_MEF_habitual;
static uint32_t timSec_habitual;	// Timer to move on differents states.
static uint32_t timBlink_habitual;	// Timer to blink leds.

/*==================[external functions definition]=========================*/

extern void mef_habitual_init(){
	mef_habitual_reset();
}

extern void mef_habitual_reset(){
	timSec_habitual = DURATION_SEC_1;
	estado_MEF_habitual = EST_SEC_1;
	key_clearFlags(BOARD_SW_ID_1);
}

extern void mef_habitual_init_sec_5(){
	estado_MEF_habitual = EST_SEC_5;
	key_clearFlags(BOARD_SW_ID_1);
	key_clearFlags(BOARD_SW_ID_3);
	count_resetCarCount(BOARD_SW_ID_3);
}

extern tr_enum mef_habitual(void){
    switch(estado_MEF_habitual){
        case EST_SEC_1:
            board_setLed(LVR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);

            if(key_getPressEv(BOARD_SW_ID_3)) count_updateCarCount(BOARD_SW_ID_3, SUMAR); // Add a car to the count if the switch flags is True
            if(key_getPressEv(BOARD_SW_ID_1)) return TR_TO_PEATON;	// Return transition to mef_peaton
            if(count_getCarCount(BOARD_SW_ID_3) >= CAR_COUNT_MAX)return TR_TO_SECUNDARIO;	// Return transition to mef_secundario
            if(timSec_habitual == 0){
            	timSec_habitual = DURATION_SEC_2;
                estado_MEF_habitual = EST_SEC_2;
                timBlink_habitual = DURATION_BLINK;
            }
            break;
        case EST_SEC_2:
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);

            if(timBlink_habitual == 0){
            	board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
                timBlink_habitual = DURATION_BLINK;
            }
            if(timSec_habitual == 0){
            	timSec_habitual = DURATION_SEC_3;
                estado_MEF_habitual = EST_SEC_3;
            }
            break;
        case EST_SEC_3:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LVS, BOARD_LED_MSG_ON);
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            board_setLed(LRS, BOARD_LED_MSG_OFF);

            if(timSec_habitual == 0){
                timSec_habitual = DURATION_SEC_4;
                estado_MEF_habitual = EST_SEC_4;
                timBlink_habitual = DURATION_BLINK;
            }
            break;
        case EST_SEC_4:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            board_setLed(LVR, BOARD_LED_MSG_OFF);

            if(timBlink_habitual == 0){
            	board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
                timBlink_habitual = DURATION_BLINK;
            }
            if(timSec_habitual == 0){
            	key_clearFlags(BOARD_SW_ID_1);
            	key_clearFlags(BOARD_SW_ID_3);
            	count_resetCarCount(BOARD_SW_ID_3);
                timSec_habitual = DURATION_SEC_1;
                estado_MEF_habitual = EST_SEC_1;
            }
            break;
        case EST_SEC_5: // It is entered only if mef_habitual_init_sec_5() is executed.
        	board_setLed(LVR, BOARD_LED_MSG_ON);
        	board_setLed(LRS, BOARD_LED_MSG_ON);
        	board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);

            if(key_getPressEv(BOARD_SW_ID_3)) count_updateCarCount(BOARD_SW_ID_3, SUMAR); // Add a car to the count if the switch flags is True
            if(count_getCarCount(BOARD_SW_ID_3) >= CAR_COUNT_MAX) return TR_TO_SECUNDARIO; // Return transition to mef_secundario
            if(timSec_habitual == 0){
				timSec_habitual = DURATION_SEC_2;
				estado_MEF_habitual = EST_SEC_2;
				timBlink_habitual = DURATION_BLINK;
            }
            break;
    }
    return TR_NONE;	// By default the MEF returns none transition
}

extern void mef_habitual_task1ms(void){
    if(timSec_habitual) timSec_habitual--;
    if(timBlink_habitual) timBlink_habitual--;
}

/*==================[end of file]============================================*/

