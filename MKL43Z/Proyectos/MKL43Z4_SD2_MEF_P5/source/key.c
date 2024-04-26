/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * Copyright 2023, Guido Cicconi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusions]=============================================*/
#include "key.h"

/*==================[macros and definitions]=================================*/
typedef enum
{
	ESPERANDO_ACTIVACION = 0, // Estado esperando presionar pulsador.
	ESPERANDO_DESACTIVACION	  // Estado esperando despresionar pulsador.
}estPul_enum;

/*==================[internal data declaration]==============================*/

static estPul_enum estSW[BOARD_SW_ID_TOTAL]; // Arreglo que contiene el estado actual de cada tecla
static int32_t timerSW[BOARD_SW_ID_TOTAL];   // Arreglo que contiene el tiempo transcurrido desde que se presionó cada tecla
static bool pressSW[BOARD_SW_ID_TOTAL];      // Arreglo que indica si se ha detectado una pulsación de tecla
static bool releaseSW[BOARD_SW_ID_TOTAL];    // Arreglo que indica si se ha detectado la liberación de una tecla

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


extern void key_init(void) // Inicialización de los switches
{
    for (int32_t i = 0 ; i < BOARD_SW_ID_TOTAL ; i++)
    {
    	estSW[i] = ESPERANDO_ACTIVACION;  // Inicializa el estado de todas los switches como "esperando activación"
    	pressSW[i] = 0;                    // Inicializa el indicador de pulsación de swtich como falso
    	releaseSW[i] = 0;                 // Inicializa el indicador de liberación de switch como falso
    	timerSW[i] = 0;                   // Inicializa el temporizador de cada switch en cero
    }
}

extern void key_periodicTask1ms(void) { // Tarea periódica de 1 ms para gestionar los switches

	for (int32_t i = 0; i < BOARD_SW_ID_TOTAL; i++) { // Recorremos el array para todos los switches definidos en SD2_board.h

		switch (estSW[i]) {

		case ESPERANDO_ACTIVACION:
			if (board_getSw(i)) {
				pressSW[i] = 1;						// Se detecta una pulsada
				estSW[i] = ESPERANDO_DESACTIVACION; // Cambiar estado a esperando despulsada
			}
			break;

		case ESPERANDO_DESACTIVACION:
			timerSW[i]++;							// Incrementa el temporizador mientras los tiene apretado
			if (!board_getSw(i)) {
				releaseSW[i] = 1;					// Se detecta liberación del switch
				estSW[i] = ESPERANDO_ACTIVACION;	// Cambia el estado del switch
				timerSW[i]=0;						// Setea timer/contador de dicho switch en cero nuevamente
			}
			break;

		default:
			estSW[i] = ESPERANDO_ACTIVACION; 		// Si se alcanza un estado desconocido, vuelve al estado de espera de activación
			break;
		}

	}

}

extern bool key_getPressEv(board_swId_enum id) 	// Obtiene el evento de pulsación del switch
{
    bool ret = false;

    if (pressSW[id])
    {
        pressSW[id] = 0;	// Reinicia el indicador de pulsado del switch
        ret = true;			// Devuelve verdadero indicando que se ha detectado una pulsación de switch
    }

    return ret;
}

extern bool key_getReleaseEv(board_swId_enum id) // Obtiene el evento de liberación del switch
{
    bool ret = false;

    if (releaseSW[id])
    {
    	releaseSW[id] = 0;  // Reinicia el indicador de liberación del switch
        ret = true;			// Devuelve verdadero indicando que se ha detectado una liberación del switch
    }

    return ret;
}

// Comprueba si el switch ha estado presionado durante un tiempo específico:
extern bool key_getPressed(board_swId_enum id, int32_t timeMs){
	if (timerSW[id] >= timeMs) return true; // Si el tiempo transcurrido es mayor o igual al tiempo especificado, devuelve verdadero
	else return false;
}

// Limpia los indicadores de eventos de un switch específico:
extern void key_clearFlags(board_swId_enum id)
{
	pressSW[id] = 0;    // Reinicia el indicador de pulsación del switch
	releaseSW[id] = 0;  // Reinicia el indicador de liberación del switch
}
