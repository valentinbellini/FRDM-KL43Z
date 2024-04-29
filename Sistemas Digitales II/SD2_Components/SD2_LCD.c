/* Copyright 2019, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2019, Luciano Rumin
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
#include <math.h>

// Project Included Files

#include "SD2_board.h"
#include "clock_config.h"
#include "fsl_smc.h"
#include "Seg_LCD.h"

/*==================[macros and definitions]=================================*/
#define MAXIMO_MILISEG 1000
#define MAXIMA_CUENTA    1000

/*==================[internal data declaration]==============================*/
static uint16_t miliseg = 0;
static uint16_t contador = 0;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootClocks();

    // Se inicializan funciones de la placa
    board_init();
    SegLCD_Init();

    /* inicializa interrupción de systick cada 1 ms */
    SysTick_Config(SystemCoreClock / 1000U);

    SegLCD_DP2_Off();
    SegLCD_DisplayDecimal(0);
    while(1)
    {
        if(miliseg == 0)
        {
            miliseg = MAXIMO_MILISEG;

            contador++;
            if(contador == MAXIMA_CUENTA){
                contador = 0;
            }
            SegLCD_DisplayDecimal(contador);
        }
    }
}

/*==================[end of file]============================================*/

void SysTick_Handler(void)
{
   if (miliseg)
       miliseg--;
}

