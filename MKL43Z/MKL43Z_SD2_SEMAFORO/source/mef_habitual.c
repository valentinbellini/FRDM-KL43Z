#include "mef_habitual.h"
#include <stdint.h>
#include "SD2_board.h"

/*==================[macros and typedef]====================================*/

typedef enum{
	EST_MEF_HABITUAL_1 = 0,
	EST_MEF_HABITUAL_2,
	EST_MEF_HABITUAL_3,
    EST_MEF_HABITUAL_4
} estMefHabitual_enum;

/*==================[internal data definition]==============================*/

static estMefHabitual_enum estado_MEF_habitual;
static uint32_t timSec_habitual;
static uint32_t timBlink_habitual;

/*==================[external functions definition]=========================*/

extern void mef_habitual_init(void){
    timSec_habitual = 120000; /* 2 minutos */
    estado_MEF_habitual = EST_MEF_HABITUAL_1;
}

extern void mef_habitual(void){
    switch(estado_MEF_habitual){
        case EST_MEF_HABITUAL_1:
            board_setLed(LVR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timSec_habitual <= 0){
                timSec_habitual = 5000;
                timBlink_habitual = 500;
                estado_MEF_habitual = EST_MEF_HABITUAL_2;
            }
            break;
        case EST_MEF_HABITUAL_2:
            board_setLed(LRS, BOARD_LED_MSG_ON);
            board_setLed(LRR, BOARD_LED_MSG_OFF);
            board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timBlink_habitual <= 0){
                timBlink_habitual = 500;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_habitual <= 0){
                timSec_habitual = 30000; /* 30 segundos */
                estado_MEF_habitual = EST_MEF_HABITUAL_3;
            }
            break;
        case EST_MEF_HABITUAL_3:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LVS, BOARD_LED_MSG_ON);
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            if(timSec_habitual <= 0){
                timSec_habitual = 5000;
                timBlink_habitual = 500;
                estado_MEF_habitual = EST_MEF_HABITUAL_4;
            }
            break;
        case EST_MEF_HABITUAL_4:
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            if(timBlink_habitual <= 0){
                timBlink_habitual = 500;
                board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_habitual <= 0){
                timSec_habitual = 120000;
                estado_MEF_habitual = EST_MEF_HABITUAL_1;
            }
            break;
    }
}

extern void mef_habitual_task1ms(void){
    if(timSec_habitual) timSec_habitual--;
    if(timBlink_habitual) timBlink_habitual--;
}

/*==================[end of file]============================================*/
