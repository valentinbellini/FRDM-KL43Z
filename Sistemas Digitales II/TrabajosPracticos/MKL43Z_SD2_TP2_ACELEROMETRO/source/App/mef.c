
/*==================[inclusions]=============================================*/

#include <stdio.h>
#include <math.h>
#include "fsl_debug_console.h"
#include "fsl_smc.h"

#include <App/mef.h>
#include "Drivers/PowerMode/power_mode_switch.h"
#include "Drivers/Board/SD2_board.h"
#include "Drivers/Key/key.h"
#include "Drivers/MMA8451/mma8451.h"
//#include "./Drivers/oled.h"

/*==================[macros and typedef]=====================================*/
/* ------------------------- Macros IO ------------------------- */
#define LR_ON 				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON)
#define LR_OFF 				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF)
#define LR_TOGGLE 			board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE)

#define LV_ON				board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON)
#define LV_OFF				board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF)
#define LV_TOGGLE 			board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_TOGGLE)

#define GET_SW1 			key_getPressEv(BOARD_SW_ID_1)
#define GET_SW3 			key_getPressEv(BOARD_SW_ID_3)
/* ------------------------- Macros Timers ------------------------- */
#define T_BLINK				500
#define T_SEC				10000
/* ------------------------- Macros Display ------------------------- */
//#define DISPLAY_OFF 		oled_clearScreen(OLED_COLOR_BLACK)
/* ------------------------- Macros Acelerómetro ------------------------- */
#define CONFIG_FREEFALL		mma8451_freefall_config()				// Configura Interrupciones por freefall
#define CONFIG_DATAREADY	mma8451_dataReady_config()				// Configura Interrupciones por data ready
#define CONFIG_CLEAR_INTS	mma8451_clearInterruptions_config()		// Configura modo sin interrupciones
#define IS_FREEFALL			mma8451_getFreefallInterruptStatus()	// 1 si se está en freefall
#define IS_DATA_READY		mma8451_getDataReadyInterruptStatus()	// 1 si se está en dataready


typedef enum{
	BAJO_CONSUMO = 0,
	CAIDA_LIBRE,
	SHOW_DISPLAY,
} MEF_State;

/*==================[internal data definition]===============================*/

static int16_t timSec, timBlink;
static int16_t MaxAccValue = 0;
static int NewAccValue = 0, LastAccValue = 0, cuenta = 0;

static MEF_State state;

uint32_t freq = 0;
smc_power_state_t currentPowerState;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mef_init(){
	MaxAccValue = 0;
	LastAccValue = 0;
	NewAccValue = 0;
	cuenta = 0;
//	DISPLAY_OFF;
	CONFIG_FREEFALL;
	LR_OFF;
	timBlink = T_BLINK;
	timSec = T_SEC;
	state = BAJO_CONSUMO;

	APP_PowerModeSwitch(kSMC_PowerStateRun, kAPP_PowerModeVlpr);
	/* Verification and Print */
	currentPowerState = SMC_GetPowerModeState(SMC);
	APP_ShowPowerMode(currentPowerState);
	freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
	PRINTF("    Core Clock = %dMHz \r\r", freq/1000000);

}

extern void mef_Tr_To_Bajo_Consumo(){
	mef_init();
}


extern void mef(){
	switch(state){
		case BAJO_CONSUMO:
			// Transición
			if(IS_FREEFALL){ // Si se está en caida libre
				state = CAIDA_LIBRE;
				CONFIG_DATAREADY;	// Interrupciones por DataReady
				LR_ON;
				/* Verification and Print */
				currentPowerState = SMC_GetPowerModeState(SMC);
				APP_ShowPowerMode(currentPowerState);
				freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
				PRINTF("    Core Clock = %dMHz \r\r", freq/1000000);
			}

		break;
		case CAIDA_LIBRE:
			// Acciones
			if (timBlink == 0){
				LR_TOGGLE;
				timBlink = T_BLINK;
			}

			if(IS_DATA_READY){ // Si la conversión finalizó
				int16_t x = mma8451_getAcX();
				int16_t y = mma8451_getAcY();
				int16_t z = mma8451_getAcZ();

				NewAccValue = x*x + y*y + z*z; // No se aplica la raiz para no tener mayor consumo de tiempo en procesamiento.
				// Si el nuevo valor de medición es mayor al máximo, se setea como el nuevo máximo
				if(NewAccValue >= MaxAccValue) MaxAccValue = NewAccValue;

				// Por otro lado, para detectar una finalización de la caida, se compara sumando una
				// cuenta, cada vez que de manera consecutiva, llegan nuevos datos que varian en
				// menos de un 10% del dato anterior.
				if (NewAccValue < 1.1 * LastAccValue && NewAccValue > 0.9 * LastAccValue){
					cuenta++;
				} else {
					cuenta = 0;
				}
				LastAccValue = NewAccValue;
			}

			// Transición: cuando se dan 10 valores consecutivos de medicion menor al 10% anterior.
			if(cuenta >= 10){
				CONFIG_CLEAR_INTS;
				// DISPLAY_ON;
				key_clearFlags(BOARD_SW_ID_1); // Limpio la flag del SW1 por si fue presionada antes.
				timSec = T_SEC; // Tiempo en 10 segundos para salir del estado

				NewAccValue = sqrt(MaxAccValue); // Ahora si aplicamos la raiz y casteamos a entero

				// Imprimir en OLED

				PRINTF("    Max Aceleration = %d g \r\r", NewAccValue);
				state = SHOW_DISPLAY;
			}

		break;
		case SHOW_DISPLAY:
			// Acciones
			if (timBlink == 0){	// Toggle led rojo
				LR_TOGGLE;
				timBlink = T_BLINK;
			}
			// Transicion: si se toca el switch 1 o si pasan 10 segundos
			if(!timSec | GET_SW1){
				mef_Tr_To_Bajo_Consumo();
			}
		break;
	}
}

extern void mef_task1ms(void){
	if(state != BAJO_CONSUMO && timBlink) 	timBlink--;
	if(state == SHOW_DISPLAY && timSec)  	timSec--;
}

/*==================[end of file]============================================*/

