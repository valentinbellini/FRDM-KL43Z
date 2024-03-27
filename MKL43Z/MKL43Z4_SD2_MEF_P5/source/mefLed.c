
/*==================[inclusions]=============================================*/
#include "mefLed.h"

#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/

#define PERIODO_PARPADEO 300
#define RETENTION_TIME_MS 2000

typedef enum {
	INICIO = 0,
	TOGGLE,
	PARPADEO
} estados_mef_led;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estados_mef_led estadoMefLed;
static int32_t tim1msLed;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefLed_init(void)
{
	estadoMefLed = INICIO;
}

extern void mefLed(void) {

	switch (estadoMefLed) {

	case INICIO:
		board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
		estadoMefLed = TOGGLE;
		break;

	case TOGGLE:
		if(key_getPressEv(BOARD_SW_ID_1)){
			board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
		}
		if(key_getPressed(BOARD_SW_ID_1, RETENTION_TIME_MS)){
			tim1msLed=PERIODO_PARPADEO;
			key_clearFlags(BOARD_SW_ID_1);
			estadoMefLed = PARPADEO;
		}
		break;

	case PARPADEO:
		if(!tim1msLed){
			tim1msLed=PERIODO_PARPADEO;
			board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
		}
		if(key_getReleaseEv(BOARD_SW_ID_1)){
			estadoMefLed = TOGGLE;
		}
		break;

	default:
		estadoMefLed = INICIO;
		break;
	}

}

extern void mefLed_task1ms(void)
{
    if (tim1msLed) tim1msLed--;
}


/*==================[end of file]============================================*/
