/**
 * @file    mef_principal.c
 * @brief   main MEF
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/

#include "App/mef_trama_rec.h"
#include "App/mef_principal.h"
#include "Drivers/Board/SD2_board.h"
#include "Drivers/Key/key.h"
#include "Drivers/MMA8451/mma8451.h"
#include "Drivers/SSD1306/oled.h"
#include <Drivers/Transceiver/transceiver_RS485_UART.h>


/*==================[macros and definitions]=================================*/

#define IS_DATA_MMA8451_READY		mma8451_getDataReadyInterruptStatus()	/* return bool true if dataready for reading */
#define UART_TRANSMISSION_DELAY		150 //ms

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} aceleracion_t;

/* Posibles estados de la MEF Principal */
typedef enum{
	Est_Master,
	Est_3D
}mef_principal_estado_enum;

/*==================[internal data declaration]==============================*/

static mef_principal_estado_enum estado_mef_principal;
static volatile int timer;
static aceleracion_t mma8451_acel_reading;

/*==================[external functions definition]==========================*/

void mef_principal_init(){
	estado_mef_principal = Est_Master;
	timer = UART_TRANSMISSION_DELAY;
	oled_clearScreen(OLED_COLOR_BLACK);
	oled_putString(30, 29, (uint8_t*)"Estado Master" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void mef_principal(){

	switch(estado_mef_principal){

		/*------ Estado Master -------------------------------------------*/
		case Est_Master:

			if (board_rs485_isDataAvailable()){ /* Redundancia para chequeo de dato disponible */
				mefRecTrama_task();
			}

			/* Transición a ESTADO 3D */
			if(key_getPressEv(BOARD_SW_ID_1)){
				estado_mef_principal = Est_3D;
				oled_clearScreen(OLED_COLOR_BLACK);
				oled_putString(35, 29, (uint8_t*)"Estado 3D" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
				mma8451_dataReady_config(); /* Se habilitan interrupciones por dataReady */
			}
			break;

		/*------ Estado 3D -------------------------------------------*/
		case Est_3D:
			uint8_t buffer_modo_3d[32];
			if(IS_DATA_MMA8451_READY){ /* Si la conversión finalizo --> guardamos los datos localmente */
				mma8451_acel_reading.x = mma8451_getAcX();
				mma8451_acel_reading.y = mma8451_getAcY();
				mma8451_acel_reading.z = mma8451_getAcZ();
			}
			if(timer <= 0) // Para no sobrecargar el puerto UART, Ejecutamos cada un tiempo mayor a 100ms.
			{
				/* Impresión en consola para verificar (Solo el modo DEBUG)	 */
				DEBUG_PRINT("Eje X: %d | ", mma8451_acel_reading.x);
				DEBUG_PRINT("Eje Y: %d | ", mma8451_acel_reading.y);
				DEBUG_PRINT("Eje Z: %d\n", mma8451_acel_reading.z);

				timer = UART_TRANSMISSION_DELAY;

				/* Se formatea el buffer y se envia por uart via DMA: */
				snprintf((char*)buffer_modo_3d, sizeof(buffer_modo_3d), "%d %d %d\n",  mma8451_acel_reading.x, mma8451_acel_reading.y, mma8451_acel_reading.z);
				uart_drv_envDatos_DMA(buffer_modo_3d, strlen((char*)buffer_modo_3d));
			}

			/* Transición a ESTADO MASTER */
			if(key_getPressEv(BOARD_SW_ID_1)) {
				mma8451_clearInterruptions_config(); /* Se deja de interrumpir por interrupciones */
				estado_mef_principal = Est_Master;
				oled_clearScreen(OLED_COLOR_BLACK);
				oled_putString(30, 29, (uint8_t*)"Estado Master" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
			}
	}
}

void mef_principal_task1ms(){
	if(timer && estado_mef_principal == Est_3D) timer--; /* se decrementa el timer de transmisión por UART en estado 3D */
}
