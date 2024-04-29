/**
 * @file    mef_peaton.c
 * @brief   MEF for the pedestrian crossing
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "SD2_board.h"
#include "mef_peaton.h"

/*==================[macros and typedef]==================================== */

#define DURATION_EST_BLINK_1 	5000
#define DURATION_EST_PASO 		10000
#define DURATION_EST_BLINK_2 	5000
#define DURATION_BLINK 			200
// Times in mS.

typedef enum{
	EST_BLINK_1 = 0,	// Initial state (blink_1): Blink LVR
	EST_PASO,			// "Paso" state: Pedestrian crossing on main route enabled
	EST_BLINK_2			// blink_2 state: Blink LRR
} estMefPeaton_enum;

/*==================[internal data definition]============================== */

static estMefPeaton_enum estado_MEF_peaton;
static uint32_t timSec_peaton;				// Timer to move on differents states.
static uint32_t timBlink_peaton;			// Timer to blink leds.

/*==================[external functions definition]=========================*/

extern void mef_peaton_init(void){
    mef_peaton_reset();
}

extern void mef_peaton_reset(void){
    timSec_peaton = DURATION_EST_BLINK_1;
    timBlink_peaton = DURATION_BLINK;
    estado_MEF_peaton = EST_BLINK_1;
}

/* mef_peaton: Return bool True to move on to MEF_Habitual */
extern bool mef_peaton(void){
    switch(estado_MEF_peaton){
        case EST_BLINK_1:
            board_setLed(LRS, BOARD_LED_MSG_ON);
		    board_setLed(LRR, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_OFF);

            if(timBlink_peaton == 0){
                timBlink_peaton = DURATION_BLINK;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_peaton == 0) {
                timSec_peaton = DURATION_EST_PASO;
                estado_MEF_peaton = EST_PASO;
            }
            break;
        case EST_PASO:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVS, BOARD_LED_MSG_ON);

            if(timSec_peaton == 0) {
                timSec_peaton = DURATION_EST_BLINK_2;
                timBlink_peaton = DURATION_BLINK;
                estado_MEF_peaton = EST_BLINK_2;
            }
            break;
        case EST_BLINK_2:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_ON);

            if(timBlink_peaton == 0){
                timBlink_peaton = DURATION_BLINK;
                board_setLed(LRR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_peaton == 0) return true;	// Returns true for the mef_modo to enable go to the mef_habitual SEC_5 state
            break;
    }
    return false; // By default the MEF returns false.
}

extern void mef_peaton_task1ms(void){
    if(timSec_peaton) timSec_peaton--;
    if(timBlink_peaton) timBlink_peaton--;
}

/*==================[end of file]============================================*/
