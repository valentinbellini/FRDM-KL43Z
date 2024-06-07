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
#include "time.h"
#include "fsl_pit.h"
#include "clock_config.h"

/*==================[macros and definitions]=================================*/
#define PIT_IRQ_ID 			PIT_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK 	CLOCK_GetFreq(kCLOCK_BusClk)
#define PIT_CHNL			kPIT_Chnl_0



/*==================[internal data declaration]==============================*/

static bool overflowFlag = false;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void PIT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, PIT_CHNL, kPIT_TimerFlag);

	overflowFlag = true;
}

/*==================[external functions definition]==========================*/

void time_init()
{
	/* Structure of initialize PIT */
	pit_config_t pitConfig;

	/*
	 * pitConfig.enableRunInDebug = false;
	 */
	PIT_GetDefaultConfig(&pitConfig);

	pitConfig.enableRunInDebug = true;

	/* Init pit module */
	PIT_Init(PIT, &pitConfig);

	/* Set timer period for channel 0 */
	PIT_SetTimerPeriod(PIT, PIT_CHNL, UINT32_MAX);//USEC_TO_COUNT(UINT32_MAX, PIT_SOURCE_CLOCK));

	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(PIT, PIT_CHNL, kPIT_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQ_ID);

	/* Start channel 0 */
	PIT_StartTimer(PIT, PIT_CHNL);
}

void time_restart(void)
{
	PIT_StopTimer(PIT, PIT_CHNL);

	overflowFlag = false;

	PIT_SetTimerPeriod(PIT, PIT_CHNL, UINT32_MAX);
	PIT_StartTimer(PIT, PIT_CHNL);
}

uint64_t time_elapsed_us(void) //Tiene ~0.03ms de error por defecto
{
	if(overflowFlag)
		return UINT32_MAX;

	return COUNT_TO_USEC((UINT32_MAX - PIT_GetCurrentTimerCount(PIT, PIT_CHNL)), PIT_SOURCE_CLOCK);
}

/*
 * PRUEBA
 * ---------------------------------------------------------------------
 * Frecuencia de Systick 							| 20971520 		 Hz
 * ---------------------------------------------------------------------
 * Período de Systick 								|  	   999.92752 us
 * ---------------------------------------------------------------------
 * Para descontar desde 100U se toma (idealmente) 	|	 99992.7 	 us
 * Temporizado real (con el sistema actual)			|	 99965.0 	 us
 *----------------------------------------------------------------------
 *											  ERROR	|		27.0   	 us (~ 0.03ms) por defecto
 * */

bool time_overflow(void)
{
	bool of = overflowFlag;
	overflowFlag = false;
	return of;
}

/*==================[end of file]============================================*/

