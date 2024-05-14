/**
 * @file    mef.c
 * @brief   Application work flow
 * @autor	Valentin Bellini & Iván Saitta
 */

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
#define CONFIG_CLEAR_INTS	mma8451_clearInterruptions_config()		// Configura modo sin interrupciones
#define IS_FREEFALL			mma8451_getFreefallInterruptStatus()	// 1 si se está en freefall
#define IS_DATA_READY		mma8451_getDataReadyInterruptStatus()	// 1 si se está en dataready

#define GRAVITY_BOUND		0.05

typedef enum{
	BAJO_CONSUMO = 0,
	CAIDA_LIBRE,
	SHOW_DISPLAY,
} MEF_State;

/*==================[internal data definition]===============================*/
static int16_t timSec, timBlink;
static int16_t MaxAccValue, NewAccValue;
static int cuenta;

static MEF_State state;

uint32_t freq = 0;
smc_power_state_t currentPowerState;

uint16_t limit_high = (uint16_t) (98*98)*(1+GRAVITY_BOUND);
uint16_t limit_low = (uint16_t) (98*98)*(1-GRAVITY_BOUND);

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mef_init(){
	/* Set Variables */
	MaxAccValue = 0;
	NewAccValue = 0;
	cuenta = 0;
	timBlink = T_BLINK;
	timSec = T_SEC;
	state = BAJO_CONSUMO;

	/* Settings */
	LR_OFF;
	mma8451_freefall_config();	// Freefall interrupt configured
	APP_PowerModeSwitch(kSMC_PowerStateRun, kAPP_PowerModeVlpr);
	oled_clearScreen(OLED_COLOR_BLACK);
	oled_putString(30, 29, (uint8_t*)"Bajo Consumo" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);

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
			// Do Nothing
			if(IS_FREEFALL){
				state = CAIDA_LIBRE;

				/* Settings */
				LR_ON;
				mma8451_dataReady_config();
				/* uC is set in run mode at the mma8451 IRQ_Hanlder */

				/* Verification and Print */
				currentPowerState = SMC_GetPowerModeState(SMC);
				APP_ShowPowerMode(currentPowerState);
				freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
				PRINTF("    Core Clock = %dMHz \r\r", freq/1000000);
			}

		break;
		case CAIDA_LIBRE:
			if (timBlink == 0){
				LR_TOGGLE;
				timBlink = T_BLINK;
			}
			if(IS_DATA_READY){ // Si la conversión finalizó
				int16_t x = mma8451_getAcX();
				int16_t y = mma8451_getAcY();
				int16_t z = mma8451_getAcZ();

				NewAccValue = x*x + y*y + z*z; // Do not apply the square root here for eficient.

				/* Let's take a look if we have a new max value */
				if(NewAccValue >= MaxAccValue) MaxAccValue = NewAccValue;

				/* Here we could implement any method to get out when the fall is finished */
				if (NewAccValue < limit_high && NewAccValue > limit_low){
					cuenta++;
				} else {
					cuenta = 0;
				}
			}

			/* Here we could implement any method to get out when the fall is finished */
			if(cuenta >= 5){
				// Transition to next state so:
				state = SHOW_DISPLAY;				// Set Next State
				MaxAccValue = sqrt(MaxAccValue); 	// Now we apply the square root to have the exact value

				CONFIG_CLEAR_INTS;

				key_clearFlags(BOARD_SW_ID_1); 		// Clear switch flags just in case

				PRINTF("    Max Aceleration = %d.%d g \r\r", MaxAccValue/100,MaxAccValue-(MaxAccValue/100)*100);

				char max_acc_str[16];	// Variable to save the max aceleration value into a string to print.
				oled_clearScreen(OLED_COLOR_BLACK);
				sprintf(max_acc_str, "Max Acc: %d.%d g",MaxAccValue/100,MaxAccValue-(MaxAccValue/100)*100);
				oled_fillRect(8, 16, 56+64, 16+32, OLED_COLOR_WHITE);
				oled_fillRect(8+4, 16+4, 56+64-4, 16+32-4, OLED_COLOR_BLACK);
				oled_putString(20, 29, (uint8_t *)max_acc_str , OLED_COLOR_WHITE, OLED_COLOR_BLACK);

			}

		break;
		case SHOW_DISPLAY:
			if (timBlink == 0){
				LR_TOGGLE;
				timBlink = T_BLINK;
			}
			char char_time[16];
			sprintf(char_time, "%d.%03d S", timSec / 1000, timSec % 1000);
			oled_putString(46, 2, (uint8_t *)char_time , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
			/* Transition to next state if time over or switch 1 is pressed */
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

