/**
 * @file    mef_trama_rec.c
 * @brief   MEF para recepción de trama.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/
#include <Drivers/Transceiver/transceiver_RS485_UART.h>
#include "App/mef_trama_rec.h"
#include "App/trama_process.h"

/*==================[macros and definitions]=================================*/
#define _BUFFER_SIZE		32		/* Buffer Size de RX */
#define CHAR_LF				0x0A

typedef enum{
	MEF_EST_ESPERANDO_INICIO = 0,
	MEF_EST_ESPERANDO_GRUPO_1,
	MEF_EST_ESPERANDO_GRUPO_2,
	MEF_EST_RECIBIENDO_TRAMA,
}mefRecTrama_estado_enum;


/*==================[internal data declaration]==============================*/
static char bufferRec[_BUFFER_SIZE];

/*==================[external functions definition]==========================*/

/* MEF para preprocesamiento de la trama recibida por ring buffer */
void mefRecTrama_task(void){

	static mefRecTrama_estado_enum estado = MEF_EST_ESPERANDO_INICIO;

	uint32_t flagRec; 			/* Indicará si hay dato disponible */
	uint8_t byteRec;			/* Variable donde se guarda el byte recibido */
	static uint8_t indexRec;	/* Indice del buffer */

	flagRec = uart_ringBuffer_recDatos(&byteRec, sizeof(byteRec)); /* Recibe dato por UART */

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
			/*
			 * En este estado ya se ha verificado que la trama es correcta y debe ser procesada.
			 * Se procesa el buffer con los siguientes caracteres que sean datos.
			 * Por último, cuando llega el CHAR_LF, se llama a una función para procesar la trama
			 * */

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

			if (flagRec != 0 && byteRec == CHAR_LF){ /* Llega el LF, se completó la trama y se envía buffer a procesar */
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
