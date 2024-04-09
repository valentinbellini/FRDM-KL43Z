#include "mef_habitual.h"
#include <stdint.h>
#include "SD2_board.h"

/*==================[macros and typedef]====================================*/

#define DURACION_SEC_1				120000	// 2 min
#define DURACION_SEC_2				5000	// 5 seg
#define DURACION_SEC_3				30000 	// 30 seg
#define DURACION_SEC_4				5000	// 5 seg
#define DURACION_BLINK    			200     // 200ms


typedef enum{
	EST_SEC_1 = 0,
	EST_SEC_2,
	EST_SEC_3,
    EST_SEC_4,
	EST_SEC_5
} estMefHabitual_enum;

/*==================[internal data definition]==============================*/

static estMefHabitual_enum estado_MEF_habitual;
static uint32_t timSec_habitual;
static uint32_t timBlink_habitual;

/*==================[external functions definition]=========================*/

extern void mef_habitual_init(){
    timSec_habitual = DURACION_SEC_1;
    estado_MEF_habitual = EST_SEC_1;
}

extern void mef_habitual_init_sec_5(){
	estado_MEF_habitual = EST_SEC_5;
}

extern tr_enum mef_habitual(void){
    switch(estado_MEF_habitual){
        case EST_SEC_1:
            board_setLed(LVR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timSec_habitual == 0){
                timSec_habitual = DURACION_SEC_2;
                timBlink_habitual = DURACION_BLINK;
                estado_MEF_habitual = EST_SEC_2;
            }
            if(key_getPressEv(BOARD_SW_ID_1)){
            	return TR_TO_PEATON;
            }
            if(getAutos() == 3){
            	return TR_TO_SECUNDARIO;
            }
            break;
        case EST_SEC_2:
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timBlink_habitual == 0){
                timBlink_habitual = DURACION_BLINK;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
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
                timBlink_habitual = DURACION_BLINK;
                estado_MEF_habitual = EST_SEC_4;
            }
            break;
        case EST_SEC_4:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            if(timBlink_habitual == 0){
                timBlink_habitual = DURACION_BLINK;
                board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_habitual == 0){
                timSec_habitual = DURACION_SEC_1;
                estado_MEF_habitual = EST_SEC_1;
                key_clearFlags(BOARD_SW_ID_1);
                key_clearFlags(BOARD_SW_ID_3);
                resetAutos();
            }
            break;
        case EST_SEC_5:
        	board_setLed(LVR, BOARD_LED_MSG_ON);
        	board_setLed(LRS, BOARD_LED_MSG_ON);
        	board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timSec_habitual == 0){
            	timSec_habitual = DURACION_SEC_2;
            	timBlink_habitual = DURACION_BLINK;
            	estado_MEF_habitual = EST_SEC_2;
            }
            break;
    }
}

extern void mef_habitual_task1ms(void){
    if(timSec_habitual) timSec_habitual--;
    if(timBlink_habitual) timBlink_habitual--;
}

/*==================[end of file]============================================*/

