/*
 * mef_recep_tramas.c
 *
 *  Created on: 30 may. 2023
 *      Author: Usuario
 */


/*==================[inclusions]=============================================*/
#include <Drivers/Transceiver/transceiver_uart_rs485.h>
#include "App/mef_trama_rec.h"
#include "App/trama_process.h"
#include "Drivers/uart_ringBuffer.h"
//#include "uart_ringBuffer.h"

/*==================[macros and definitions]=================================*/
#define _BUFFER_SIZE		32
#define CHAR_LF				0x0A


typedef enum{
	MEF_EST_ESPERANDO_INICIO = 0,
	MEF_EST_ESPERANDO_GRUPO_1,
	MEF_EST_ESPERANDO_GRUPO_2,
	MEF_EST_RECIBIENDO_TRAMA,
}mefRecTrama_estado_enum;


/*==================[internal data declaration]==============================*/
static char bufferRec[_BUFFER_SIZE];

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void mefRecTrama_task(void){

	static mefRecTrama_estado_enum estado = MEF_EST_ESPERANDO_INICIO;

	uint32_t flagRec; /* Indicará si hay dato disponible */
	uint8_t byteRec;
	static uint8_t indexRec;

	//flagRec = transceiver_recDatos(&byteRec, sizeof(byteRec));
	flagRec = uart_ringBuffer_recDatos(&byteRec, sizeof(byteRec));

	switch (estado){

		case MEF_EST_ESPERANDO_INICIO:
			if (flagRec != 0 && byteRec == ':'){
				estado = MEF_EST_ESPERANDO_GRUPO_1;
			}
			break;

		case MEF_EST_ESPERANDO_GRUPO_1:
			if (flagRec != 0 && byteRec == NUM_GRUPO_A){
				estado = MEF_EST_ESPERANDO_GRUPO_2;
			}
			if (flagRec != 0 && byteRec == ':'){
				estado = MEF_EST_ESPERANDO_GRUPO_1;
			}
			else if (flagRec != 0 && byteRec != NUM_GRUPO_A){
				estado = MEF_EST_ESPERANDO_INICIO;
			}
			break;

		case MEF_EST_ESPERANDO_GRUPO_2:
			if (flagRec != 0 && byteRec == NUM_GRUPO_B){
				estado = MEF_EST_RECIBIENDO_TRAMA;
				indexRec = 0;
			}
			if (flagRec != 0 && byteRec == ':'){
				estado = MEF_EST_ESPERANDO_GRUPO_1;
			}
			else if (flagRec != 0 && byteRec != NUM_GRUPO_B){
				estado = MEF_EST_ESPERANDO_INICIO;
			}
			break;

		case MEF_EST_RECIBIENDO_TRAMA:

			if (flagRec != 0 && byteRec != CHAR_LF){ /* Si ocurre esto -> El byte recibido es parte del dato */
				if (indexRec < _BUFFER_SIZE){
					bufferRec[indexRec] = byteRec;
					indexRec++;
				}
			}

			if (flagRec != 0 && byteRec == ':'){
				indexRec = 0;
				estado = MEF_EST_ESPERANDO_GRUPO_1;
			}

			if (flagRec != 0 && byteRec == CHAR_LF){ /* Llega el LF, se completó la trama y se envía para procesar */
				tramaProcess(bufferRec, indexRec);
				estado = MEF_EST_ESPERANDO_INICIO;
			}

			if (indexRec >= _BUFFER_SIZE){
				estado = MEF_EST_ESPERANDO_INICIO;
			}

			break;
	}
}


/*==================[end of file]============================================*/
