/*
 * mef_principal.c
 *
 *  Created on: 08.06.2023
 *      Author: valen
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "fsl_debug_console.h"

#include "App/mef_trama_rec.h"
#include "App/mef_principal.h"
#include "Drivers/Transceiver/uart_ringBuffer.h"
#include "Drivers/Board/SD2_board.h"
#include "Drivers/Key/key.h"
#include "Drivers/MMA8451/mma8451.h"
#include "Drivers/SSD1306/oled.h"


/*=====================================[macros and typedef]=====================================*/
#define IS_DATA_MMA8451_READY		mma8451_getDataReadyInterruptStatus()	// return bool
#define _UART_TRANSMISSION_DELAY	150 //ms

int16_t x = 0;
int16_t y = 0;
int16_t z = 0;

typedef enum{
	Est_Master,
	Est_3D
}mef_principal_estado_enum;

/*=====================================[Variables]=====================================*/
static mef_principal_estado_enum estado;
static volatile int timer;

/*=====================================[Functions]=====================================*/
void mef_principal_init(){
	estado = Est_Master;
	timer = _UART_TRANSMISSION_DELAY;
	oled_clearScreen(OLED_COLOR_BLACK);
	oled_putString(30, 29, (uint8_t*)"Estado Master" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void mef_principal(){

	uint8_t buffer_modo_3d[32];

	switch(estado){

		case Est_Master:
			mefRecTrama_task();
			/* Transición a ESTADO 3D */
			if(key_getPressEv(BOARD_SW_ID_1)){
				estado = Est_3D;
				oled_clearScreen(OLED_COLOR_BLACK);
				oled_putString(30, 29, (uint8_t*)"Estado 3D" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
				mma8451_dataReady_config(); /* Se habilitan interrupciones por dataReady */
			}
			break;

		case Est_3D:
			if(IS_DATA_MMA8451_READY){
				x = mma8451_getAcX();
				y = mma8451_getAcY();
				z = mma8451_getAcZ();
			}
			if(timer <= 0) // Para no sobrecargar el puerto UART, Ejecutamos cada un tiempo mayor a 100ms.
			{
				PRINTF("Eje X: %d | ",x);
				PRINTF("Eje Y: %d | ",y);
				PRINTF("Eje Z: %d\n",z);
				timer = _UART_TRANSMISSION_DELAY;

				//snprintf((char*)buffer_modo_3d, sizeof(buffer_modo_3d), ":%c%c21%+04d%+04d%+04d\n", NUM_GRUPO_A, NUM_GRUPO_B, x, y, z);
				snprintf((char*)buffer_modo_3d, sizeof(buffer_modo_3d), "%d %d %d\n", x, y, z);
				uart0_drv_envDatos(buffer_modo_3d, strlen((char*)buffer_modo_3d));
			}

			/* Transición a ESTADO MASTER */
			if(key_getPressEv(BOARD_SW_ID_1)) {
				mma8451_clearInterruptions_config(); /* Se deja de interrumpir por interrupciones */
				estado = Est_Master;
				oled_clearScreen(OLED_COLOR_BLACK);
				oled_putString(30, 29, (uint8_t*)"Estado Master" , OLED_COLOR_WHITE, OLED_COLOR_BLACK);
			}
	}
}
void mef_principal_task1ms(){
	if(timer && estado == Est_3D) timer--;
}
