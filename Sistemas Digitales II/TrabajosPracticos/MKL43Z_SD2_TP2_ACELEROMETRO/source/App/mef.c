
/*==================[inclusions]=============================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "fsl_debug_console.h"
#include "fsl_smc.h"
#include "board.h"

#include <App/mef.h>
#include "Drivers/PowerMode/power_mode_switch.h"
#include "Drivers/Board/SD2_board.h"
#include "Drivers/Key/key.h"
#include "Drivers/MMA8451/mma8451.h"
#include "Drivers/time.h"
#include "Drivers/SSD1306/oled.h"

/*==================[macros and typedef]=====================================*/
/* ------------------------- Macros IO ------------------------- */
#define LR_ON 				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON)
#define LR_OFF 				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF)
#define LR_TOGGLE 			board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE)

#define GET_SW1 			key_getPressEv(BOARD_SW_ID_1)
/* ------------------------- Macros Timers ------------------------- */
#define T_BLINK				500
#define T_SEC				10000
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
uint32_t timTranscurrido;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mef_init(){
	MaxAccValue = 0;
	LastAccValue = 0;
	NewAccValue = 0;
	cuenta = 0;
	CONFIG_FREEFALL;
	LR_OFF;
	timBlink = T_BLINK;
	timSec = T_SEC;
	state = BAJO_CONSUMO;
//	oled_clearScreen(OLED_COLOR_BLACK);
//	oled_fillRect(0+8, 16+8, 0+128-8, 16+32-8, OLED_COLOR_WHITE);
//	oled_putString(20, 29, (uint8_t*)"Bajo Consumo" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	PRINTF("    Estado: Bajo consumo\n");
	APP_PowerModeSwitch(kSMC_PowerStateRun, kAPP_PowerModeVlpr);
	/* Verification and Print */
	currentPowerState = SMC_GetPowerModeState(SMC);
	APP_ShowPowerMode(currentPowerState);
	freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
	PRINTF("    Core Clock = %dMHz \r\r", freq/1000000);

//	head = NULL;
//	clearList(&head); // Limpia la lista y establece head en NULL

}

extern void mef_Tr_To_Bajo_Consumo(){
	mef_init();
}


extern void mef(){
	char max_acc_str[16];
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

				time_restart();
			}

		break;
		case CAIDA_LIBRE:
			//oled_fillRect(32+8, 16+8, 32+64-8, 16+32-8, OLED_COLOR_BLACK);
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

				//append(&head,NewAccValue);

				// Si el nuevo valor de medición es mayor al máximo, se setea como el nuevo máximo
				if(NewAccValue >= MaxAccValue) MaxAccValue = NewAccValue;

				if (NewAccValue < 1.1 * LastAccValue && NewAccValue > 0.9 * LastAccValue){
					cuenta++;
				} else {
					cuenta = 0;
				}

				LastAccValue = NewAccValue;
			}

			// Transición: cuando se dan 10 valores consecutivos de medicion menor al 10% anterior.
			if(cuenta >= 5){
				CONFIG_CLEAR_INTS;
				timTranscurrido = time_elapsed_us();

				timSec = T_SEC; // Tiempo en 10 segundos para salir del estado
				MaxAccValue = sqrt(MaxAccValue); // Ahora si aplicamos la raiz

				key_clearFlags(BOARD_SW_ID_1); // Limpio la flag del SW1 por si fue presionada antes.
				state = SHOW_DISPLAY;

				PRINTF("    Max Aceleration en cg = %d \r\r\n",MaxAccValue);
				PRINTF("    Max Aceleration = %d.%d g \r\r", MaxAccValue/10,MaxAccValue%10);
				PRINTF("    Tiempo transcurrido en caida: %d ms \r\r",timTranscurrido/1000);
				//PRINTF("Lista: ");
				//printList(head);

				oled_fillRect(32, 16, 32+64, 16+32, OLED_COLOR_WHITE);
				oled_fillRect(32+8, 16+8, 32+64-8, 16+32-8, OLED_COLOR_BLACK);
				oled_putString(56, 29, (uint8_t *)"SD3", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
				oled_circle(64, 32, 31, OLED_COLOR_WHITE);
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

