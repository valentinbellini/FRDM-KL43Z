
#include "mef_peaton.h"
#include <stdint.h>
#include "SD2_board.h"

/*==================[macros and typedef]====================================*/

#define DURACION_EST_BLINK_1 	5000	//10 seg
#define DURACION_EST_PASO 		10000	//1 min
#define DURACION_EST_BLINK_2 	5000   //10 seg
#define DURACION_BLINK 			200 	// 200 ms

typedef enum{
	EST_BLINK_1 = 0,	// Estado inicial: Blink LVR
	EST_PASO,			// Estado paso: Cruce peatonal en ruta habilitado
	EST_BLINK_2			// Estado blink_2: Blink LRR
} estMefPeaton_enum;

/*==================[internal data definition]==============================*/

static estMefPeaton_enum estado_MEF_peaton;
static uint32_t timSec_peaton;				// Timer de secuencia
static uint32_t timBlink_peaton;			// Timer de blink led

/*==================[external functions definition]=========================*/

extern void mef_peaton_init(void){
    mef_peaton_reset();
}

extern void mef_peaton_reset(void){
    timSec_peaton = DURACION_EST_BLINK_1;
    timBlink_peaton = DURACION_BLINK;
    estado_MEF_peaton = EST_BLINK_1;
}

extern bool mef_peaton(void){ /* Bool porque tengo que evaluarla para ver si salgo o no*/
    switch(estado_MEF_peaton){
        case EST_BLINK_1:
            board_setLed(LRS, BOARD_LED_MSG_ON);
		    board_setLed(LRR, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_OFF);
            if(timBlink_peaton == 0){
                timBlink_peaton = DURACION_BLINK;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_peaton == 0) {
                timSec_peaton = DURACION_EST_PASO;
                estado_MEF_peaton = EST_PASO;
            }
            break;
        case EST_PASO:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LRR, BOARD_LED_MSG_ON);
		    board_setLed(LVS, BOARD_LED_MSG_ON);
		    count_resetCarCount(BOARD_SW_ID_3); // Seteo la cuenta en 0 --> Pasan todos los autos (nuestra elección)
            if(timSec_peaton == 0) {
                timSec_peaton = DURACION_EST_BLINK_2;
                timBlink_peaton = DURACION_BLINK;
                estado_MEF_peaton = EST_BLINK_2;
            }
            break;
        case EST_BLINK_2:
            board_setLed(LVR, BOARD_LED_MSG_OFF);
		    board_setLed(LRS, BOARD_LED_MSG_OFF);
		    board_setLed(LVS, BOARD_LED_MSG_ON);
            if(timBlink_peaton == 0){
                timBlink_peaton = DURACION_BLINK;
                board_setLed(LRR, BOARD_LED_MSG_TOGGLE);
            }
            if(timSec_peaton == 0){
                return true;		// Devuelve true para que la mef_modo habilite ir hacia la mef_habitual estado SEC5
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
