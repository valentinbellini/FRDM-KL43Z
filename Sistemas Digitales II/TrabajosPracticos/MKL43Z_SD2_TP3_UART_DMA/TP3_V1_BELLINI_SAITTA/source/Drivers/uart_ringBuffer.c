/* Copyright, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright, Gustavo Muro
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
#include "Drivers/Board/SD2_board.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "board.h"
#include "ringBuffer.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
static void* pRingBufferRx;
static void* pRingBufferTx;

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void uart_ringBuffer_init(void)
{
	lpuart_config_t config;

    pRingBufferRx = ringBuffer_init(16);
    pRingBufferTx = ringBuffer_init(16);

    CLOCK_SetLpuart0Clock(0x1U); //IRC48M clock

	/* PORTA1 (pin 35) is configured as LPUART0_RX */
	PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

	/* PORTA2 (pin 36) is configured as LPUART0_TX */
	PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

	/*   lpuartConfig->baudRate_Bps = 115200U;
	 *   lpuartConfig->parityMode = kLPUART_ParityDisabled;
	 *   lpuartConfig->dataBitsCount = kLPUART_EightDataBits;
	 *   lpuartConfig->isMsb = false;
	 *   lpuartConfig->stopBitCount = kLPUART_OneStopBit;
	 *   lpuartConfig->txFifoWatermark = 0;
	 *   lpuartConfig->rxFifoWatermark = 1;
	 *   lpuartConfig->rxIdleType = kLPUART_IdleTypeStartBit;
	 *   lpuartConfig->rxIdleConfig = kLPUART_IdleCharacter1;
	 *   lpuartConfig->enableTx = false;
	 *   lpuartConfig->enableRx = false;
	 */
	LPUART_GetDefaultConfig(&config);

	config.baudRate_Bps = 115200;
	config.parityMode = kLPUART_ParityDisabled;
	config.stopBitCount = kLPUART_OneStopBit;
	config.enableTx = true;
	config.enableRx = true;

	LPUART_Init(LPUART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));

	/* Habilitación de interrupciones */
	LPUART_EnableInterrupts(LPUART0, kLPUART_RxDataRegFullInterruptEnable);
	LPUART_EnableInterrupts(LPUART0, kLPUART_TxDataRegEmptyInterruptEnable);
	EnableIRQ(LPUART0_IRQn);
}

/** \brief recibe datos por puerto serie accediendo al RB
 **
 ** \param[inout] pBuf buffer a donde guardar los datos
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes recibidos
 **/
int32_t uart_ringBuffer_recDatos(uint8_t *pBuf, int32_t size)
{
    int32_t ret = 0;

    /* entra sección de código crítico */
    NVIC_DisableIRQ(LPUART0_IRQn);

    while (!ringBuffer_isEmpty(pRingBufferRx) && ret < size)
    {
        ringBuffer_getData(pRingBufferRx, &pBuf[ret]);
        ret++;
    }

    /* sale de sección de código crítico */
    NVIC_EnableIRQ(LPUART0_IRQn);

    return ret;
}

/** \brief envía datos por puerto serie accediendo al RB
 **
 ** \param[inout] pBuf buffer a donde estan los datos a enviar
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes enviados
 **/
int32_t uart_ringBuffer_envDatos(uint8_t *pBuf, int32_t size)
{
    int32_t ret = 0;

    /* entra sección de código crítico */
    NVIC_DisableIRQ(LPUART0_IRQn);

    /* si el buffer estaba vacío hay que habilitar la int TX */
    if (ringBuffer_isEmpty(pRingBufferTx))
    	LPUART_EnableInterrupts(LPUART0, kLPUART_TxDataRegEmptyInterruptEnable);

    while (!ringBuffer_isFull(pRingBufferTx) && ret < size)
    {
        ringBuffer_putData(pRingBufferTx, pBuf[ret]);
        ret++;
    }

    /* sale de sección de código crítico */
    NVIC_EnableIRQ(LPUART0_IRQn);

    return ret;
}


void LPUART0_IRQHandler(void)
{
	uint8_t data;

    if ( (kLPUART_RxDataRegFullFlag)            & LPUART_GetStatusFlags(LPUART0) &&
         (kLPUART_RxDataRegFullInterruptEnable) & LPUART_GetEnabledInterrupts(LPUART0) )
	{
        /* obtiene dato recibido por puerto serie */
	    data = LPUART_ReadByte(LPUART0);

		/* pone dato en ring buffer */
		ringBuffer_putData(pRingBufferRx, data);

	}

	if ( (kLPUART_TxDataRegEmptyFlag)            & LPUART_GetStatusFlags(LPUART0) &&
         (kLPUART_TxDataRegEmptyInterruptEnable) & LPUART_GetEnabledInterrupts(LPUART0) )
	{
		if (ringBuffer_getData(pRingBufferTx, &data))
		{
			/* envía dato extraído del RB al puerto serie */
			LPUART_WriteByte(LPUART0, data);
		}
		else
		{
			/* si el RB está vacío deshabilita interrupción TX */
			LPUART_DisableInterrupts(LPUART0, kLPUART_TxDataRegEmptyInterruptEnable);
		}
	}
}


/*==================[end of file]============================================*/

