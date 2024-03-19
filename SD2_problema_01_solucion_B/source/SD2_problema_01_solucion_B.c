/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
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
#include "key.h"

/*==================[macros and definitions]=================================*/
typedef enum
{
	EST_MEF_LED_INICIALIZACION = 0,
	EST_MEF_LED_ESPERANDO_EV,
	EST_MEF_LED_BLINKY,

}estMefLed_enum;

/*==================[internal data declaration]==============================*/

static int16_t timLed;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void mefLed(void)
{
    static estMefLed_enum estMefLed = EST_MEF_LED_INICIALIZACION;

    switch (estMefLed)
    {
        case EST_MEF_LED_INICIALIZACION:
            board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
            estMefLed = EST_MEF_LED_ESPERANDO_EV;
            break;

        case EST_MEF_LED_ESPERANDO_EV:
            if (key_getPressEv(BOARD_SW_ID_1))
                board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
            if (key_getPressEv(BOARD_SW_ID_3))
            	estMefLed = EST_MEF_LED_BLINKY;
            break;

        case EST_MEF_LED_BLINKY:
            if (timLed <= 0)
            {
            	timLed = 500;
            	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
            }
            if (key_getPressEv(BOARD_SW_ID_3))
            {
            	estMefLed = EST_MEF_LED_ESPERANDO_EV;
            	key_getPressEv(BOARD_SW_ID_1);
            }
            break;


    }
}

/*==================[external functions definition]==========================*/

int main(void)
{
	// Se inicializan funciones de la placa
	board_init();

	// Inicializa keyboard
	key_init();

	/* inicializa interrupción de systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);

    while(1)
    {
    	mefLed();
    }
}

void SysTick_Handler(void)
{
    key_periodicTask1ms();

    if (timLed)
    	timLed--;
}

/*==================[end of file]============================================*/


