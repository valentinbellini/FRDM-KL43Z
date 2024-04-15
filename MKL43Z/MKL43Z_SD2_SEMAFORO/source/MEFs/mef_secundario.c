/**
 * @file    mef_secundario.c
 * @brief   MEF for the secondary route enabled.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "SD2_board.h"
#include "key.h"
#include "cont_autos.h"
#include "mef_secundario.h"

/*==================[macros and typedef]====================================*/

#define DURATION_EST_BLINK_1   		5000
#define DURATION_EST_BLINK_2   		5000
#define DURATION_BLINK    			500
// Times in mS.

typedef enum{
	EST_BLINK_1 = 0, 	// Initial state (BLINK_1): Blink LVR
	EST_PASO,			// "Paso" state: Secondary route enabled
	EST_BLINK_2			// blink_2 state: Blink LVS
} estMefSecundario_enum;

/*==================[internal data definition]==============================*/

static estMefSecundario_enum estado_MEF_secundario;
static uint32_t timSec_secundario;					// Timer to move on differents states.
static uint32_t timBlink_secundario;				// Timer to blink leds.

/*==================[external functions definition]=========================*/

extern void mef_secundario_init(void){
	mef_secundario_reset();
}

extern void mef_secundario_reset(void){
    timSec_secundario = DURATION_EST_BLINK_1;
    timBlink_secundario = DURATION_BLINK;
	estado_MEF_secundario = EST_BLINK_1;
}

/* mef_secundario: Return bool True to move on to MEF_Habitual */
extern bool mef_secundario(void){
    switch(estado_MEF_secundario){
        case EST_BLINK_1:
            board_setLed(LRS, BOARD_LED_MSG_ON);
		    board_setLed(LRR, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_OFF);

            if(timBlink_secundario == 0){
                timBlink_secundario = DURATION_BLINK;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_secundario == 0)	estado_MEF_secundario = EST_PASO;
            break;
        case EST_PASO:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVS, BOARD_LED_MSG_ON);

		    if(key_getPressEv(BOARD_SW_ID_3)) count_updateCarCount(BOARD_SW_ID_3, RESTAR);
            if(count_getCarCount(BOARD_SW_ID_3) == 0){	// Will stay on this state until the switch is been pressed three times (Car Count in 0).
                timSec_secundario = DURATION_EST_BLINK_2;
                timBlink_secundario = DURATION_BLINK;
                estado_MEF_secundario = EST_BLINK_2;
            }
            break;
        case EST_BLINK_2:
            board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVR, BOARD_LED_MSG_OFF);

            if(timBlink_secundario == 0){
                timBlink_secundario = DURATION_BLINK;
                board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_secundario == 0)	return true;	// Returns true for the mef_modo to enable go to the mef_habitual
            break;
    }
    return false;	// By default the MEF returns false.
}

extern void mef_secundario_task1ms(void){
    if(timSec_secundario) timSec_secundario--;
    if(timBlink_secundario) timBlink_secundario--;
}

/*==================[end of file]============================================*/

