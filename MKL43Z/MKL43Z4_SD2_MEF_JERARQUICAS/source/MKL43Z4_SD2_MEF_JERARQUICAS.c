// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Project Included Files
#include "SD2_board.h"
#include "board.h"
#include "key.h"
#include "fsl_debug_console.h"

/*==================[macros and definitions]=================================*/

// Estados de la MEF TOP que representa los distintos modos de funcionamiento del sistema
typedef enum {
	EST_MODO_SEC = 0,
	EST_MODO_SW1,
} estMefModo_enum;

// Estados de la subMEF que asigna valores a variables internas ligadas a los LEDs para el modo secuencial
typedef enum {
	EST_SEC_1 = 0,
	EST_SEC_2,
	EST_SEC_3,
} estMefSec_enum;

// SubMEF que cambia funcionamiento respondiendo a SW1
typedef enum {
	EST_SW1_BLINK = 0,
	EST_SW1_ON,
} estMefSW1_enum;

/*==================[internal data declaration]==============================*/
int32_t TempSec; // Temporizador para la MEF de secuencia
int32_t TempBlink; // Temporizador para modo blink

bool secRun; // Para indicar que está activa la submef SEC
bool resetSW1;

/*==================[internal functions declaration]=========================*/
void mefSec(void);
int8_t mefSW1(void);
void mefModo(void);
/*==================[internal functions definition]==========================*/

// MEF secuencia que asigna las variables ligadas a los LED
void mefSec(void)
{
	static estMefSec_enum estMefSec = EST_SEC_1;

	switch (estMefSec)
	{
        case EST_SEC_1:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
            if (TempSec == 0)
            {
                TempSec = 2000;
                estMefSec = EST_SEC_2;
            }
            break;

        case EST_SEC_2:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
            if (TempSec == 0)
            {
                TempSec = 2000;
                estMefSec = EST_SEC_3;
            }
            break;

        case EST_SEC_3:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
            if (TempSec == 0)
            {
                TempSec = 2000;
                estMefSec = EST_SEC_1;
            }
            break;
	}
}

// MEF modo del funcionamiento del sistemas de acuerdo con SW1
int8_t mefSW1(void)
{
	static estMefSW1_enum estMefSW1 = EST_SW1_BLINK;
	int8_t ret = 0;

	if (resetSW1)
	{
		resetSW1 = 0;
		estMefSW1 = EST_SW1_BLINK;
		TempBlink = 500;
		board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
		board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);

	}
	switch (estMefSW1)
	{
        case EST_SW1_BLINK:
            if (TempBlink == 0)
            {
                TempBlink = 500;
                board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_TOGGLE);
                board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
            }
            if (key_getPressEv(BOARD_SW_ID_1))
            {
                board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
                board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
                estMefSW1 = EST_SW1_ON;
            }
            break;

        case EST_SW1_ON:
            if (key_getPressEv(BOARD_SW_ID_1))
            {
                estMefSW1 = EST_SW1_BLINK;
                board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
                board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
                ret = 1;
            }
            break;
	}

	return ret;
}

// MEF TOP
void MefModo(void)
{
	static estMefModo_enum estMefModo = EST_MODO_SEC;
	int8_t ret;

	switch (estMefModo)
	{
        case EST_MODO_SEC:
            secRun = true;
            mefSec();

            if (key_getPressEv(BOARD_SW_ID_1))
            {
            	resetSW1 = 1;
                secRun = false;
                estMefModo = EST_MODO_SW1;
            }
            break;

        case EST_MODO_SW1:
            ret = mefSW1();
            if (ret == 1)
            {
                estMefModo = EST_MODO_SEC;
            }
            break;
	}
}

/*

 * @brief   Application entry point.
 */
int main(void) {

	// Se inicializan funciones de la placa
	board_init();
	BOARD_InitDebugConsole();
	// Inicializa keyboard
	key_init();

	/* inicializa interrupcion de systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);
	TempSec = 2000; // Para iniciar la secuencia
	TempBlink = 500; //Para iniciar blinking
	secRun = false; // Superestado SEC inactivo

	/* Enter an infinite loop, just incrementing a counter. */
	while (1) {
		MefModo();
	}

}

void SysTick_Handler(void) // OJO cambiar
{
	key_periodicTask1ms();

	if ((TempSec > 0) & secRun) {
		TempSec--;
	}
	if (TempBlink > 0) {
		TempBlink--;
	}
}
