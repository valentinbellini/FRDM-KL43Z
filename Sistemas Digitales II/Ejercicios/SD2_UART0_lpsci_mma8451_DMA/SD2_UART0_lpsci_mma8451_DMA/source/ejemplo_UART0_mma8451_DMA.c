/* Copyright 2022, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
  * Copyright 2017-2019, Gustavo Muro - Daniel Márquez
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

// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Project Included Files
#include "SD2_board.h"
#include "fsl_lpsci.h"
#include "fsl_port.h"
#include "board.h"
#include "MKL46Z4.h"
#include "pin_mux.h"
#include "uart0_drv.h"
#include "mma8451.h"


#define _UMBRAL_EJE_X_POSITIVO 50
#define _UMBRAL_EJE_X_NEGATIVO -50
#define _RETARDO_MSEG 100

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

long cuenta;

int main(void)
{
    uint8_t buffer[32];
    int16_t acc; // Variable de lectura de un eje del acelerometro

	BOARD_BootClockRUN();

	// Se inicializan funciones de la placa
	board_init();

	mma8451_setDataRate(DR_12p5hz);

	//Inicializa driver de UART0
	uart0_drv_init();

	//inicializa interrupción de systick cada 1 ms
	SysTick_Config(SystemCoreClock / 1000U);

	//Lazo infinito
	while(1)
	{
		// Led testigo de transmisión por UART0
		board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
		//Desde aquí se genera un retardo de X mSeg con el Systick Timer
		cuenta = _RETARDO_MSEG;
		while(cuenta!=0);

		//Lee valor de aceleración del eje X
	    acc = mma8451_getAcX();

	    //Enciende led rojo si la lectura supera el umbral positivo (depura funcioanemitno del mma8451)
	    if (acc > _UMBRAL_EJE_X_POSITIVO)
	    	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
	    else
	    	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);


	    //Se da formato al string a transmitir y se almacena en el arrelgo de char buffer
	    sprintf((char*)buffer, "EJE X:%d \r\n", acc);

	    //Envia datos por UART0 mediante DMA
	    uart0_drv_envDatos(buffer, strlen((char*)buffer));


	    //Desde aquí se genera un retardo de X mSeg con el Systick Timer
	    cuenta = _RETARDO_MSEG;
	    while(cuenta!=0);

	}
}


void SysTick_Handler(void)
{
   if(cuenta!=0)
	   cuenta--;
}


/*==================[end of file]============================================*/


