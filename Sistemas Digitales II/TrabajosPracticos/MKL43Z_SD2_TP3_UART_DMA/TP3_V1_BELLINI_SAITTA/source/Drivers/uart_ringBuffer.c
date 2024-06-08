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
// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Project Included Files
#include "Drivers/Board/SD2_board.h"
#include "Drivers/uart_ringBuffer.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "board.h"
#include "ringBuffer.h"
#include "fsl_lpuart_dma.h"
#include "fsl_dmamux.h"
#include "MKL43Z4.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"


/*==================[macros and definitions]=================================*/
#define TR_UART					LPUART0
#define LPUART_TX_DMA_CHANNEL 	0U
#define TX_BUFFER_DMA_SIZE  	32

/*==================[internal data declaration]==============================*/
static uint8_t txBuffer_dma[TX_BUFFER_DMA_SIZE]; /* Buffer para almacenar datos que se enviarán por DMA. */
static lpuart_dma_handle_t LPUARTDmaHandle; /* Estructura de manejo para la Tx y Rx de datos UART utilizando DMA. */
static dma_handle_t LPUARTTxDmaHandle; /* Estructura de manejo específica para las transfer DMA de transmisión UART. */
static void* pRingBufferRx; /* Puntero al buffer circular utilizado para almacenar datos recibidos. */
volatile bool txOnGoing = false; /* Bandera que indica si una operación de transmisión está en curso. */

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
/* UART user callback */
static void LPUART_UserCallback(LPUART_Type *base, lpuart_dma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_TxIdle == status)
    {
        txOnGoing = false;
    }
}

/** \brief Lee las status flags de UART y chequea de donde viene el error. **/
void checkUartErrors(void) {
    uint32_t status = LPUART_GetStatusFlags(TR_UART);
    if (status & kLPUART_RxOverrunFlag) {
        LPUART_ClearStatusFlags(TR_UART, kLPUART_RxOverrunFlag);
        PRINTF("UART Rx Overrun Error\n");
    }
    if (status & kLPUART_FramingErrorFlag) {
        LPUART_ClearStatusFlags(TR_UART, kLPUART_FramingErrorFlag);
        PRINTF("UART Framing Error\n");
    }
    if (status & kLPUART_ParityErrorFlag) {
        LPUART_ClearStatusFlags(TR_UART, kLPUART_ParityErrorFlag);
        PRINTF("UART Parity Error\n");
    }
}
/*==================[external functions definition]==========================*/

void uart_ringBuffer_init(void){


	lpuart_config_t config;

    pRingBufferRx = ringBuffer_init(16);

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

	config.baudRate_Bps = 9600;//115200;
	config.parityMode = kLPUART_ParityDisabled;
	config.stopBitCount = kLPUART_OneStopBit;
	config.enableTx = true;
	config.enableRx = true;

	LPUART_Init(LPUART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));

	/* Habilitación de interrupciones */
	LPUART_EnableInterrupts(TR_UART, kLPUART_RxDataRegFullInterruptEnable);
	LPUART_EnableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
	LPUART_EnableInterrupts(TR_UART, kLPUART_RxOverrunInterruptEnable);
	EnableIRQ(LPUART0_IRQn);

	/* CONFIGURACIÓN DMA (sólo para TX) */
	/* Init DMAMUX */
	DMAMUX_Init(DMAMUX0);

	/* Set channel for LPUART  */
	DMAMUX_SetSource(DMAMUX0, LPUART_TX_DMA_CHANNEL, kDmaRequestMux0LPUART0Tx);
	DMAMUX_EnableChannel(DMAMUX0, LPUART_TX_DMA_CHANNEL);

	/* Init the DMA module */
	DMA_Init(DMA0);
	DMA_CreateHandle(&LPUARTTxDmaHandle, DMA0, LPUART_TX_DMA_CHANNEL);

	/* Create LPUART DMA handle. */
	LPUART_TransferCreateHandleDMA(
			LPUART0,
			&LPUARTDmaHandle,
			LPUART_UserCallback,
			NULL,
			&LPUARTTxDmaHandle,
			NULL);
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
    __disable_irq();  // Deshabilita todas las interrupciones

    while (!ringBuffer_isEmpty(pRingBufferRx) && ret < size)
    {
        ringBuffer_getData(pRingBufferRx, &pBuf[ret]);
        ret++;
    }

    /* sale de sección de código crítico */
    __enable_irq();   // Habilita todas las interrupciones

    return ret;
}

/** \brief envía datos por puerto serie accediendo a memoria RAM
 **
 ** \param[inout] pBuf buffer a donde estan los datos a enviar
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes enviados
 **/
int32_t uart0_drv_envDatos(uint8_t *pBuf, int32_t size)
{
    lpuart_transfer_t xfer;

    // Modificamos la condición para que la función pueda aceptar
    // size = 0. En ese caso la fución no hace nada y devuelve 0.
    if (txOnGoing || size == 0 )
    {
        size = 0;
    }
    else
    {
        /* limita size */
        if (size > TX_BUFFER_DMA_SIZE)
            size = TX_BUFFER_DMA_SIZE;

        // Hace copia del buffer a transmitir en otro arreglo
        memcpy(txBuffer_dma, pBuf, size);

        xfer.data = txBuffer_dma;
        xfer.dataSize = size;

        txOnGoing = true;
        LPUART_TransferSendDMA(LPUART0, &LPUARTDmaHandle, &xfer);

        LPUART_EnableInterrupts(LPUART0, kLPUART_TransmissionCompleteInterruptEnable);
    }

    return size;
}

void LPUART0_IRQHandler(void)
{
	uint8_t data;

	/* Interrupción de RX */
    if ( (kLPUART_RxDataRegFullFlag)            & LPUART_GetStatusFlags(LPUART0) &&
         (kLPUART_RxDataRegFullInterruptEnable) & LPUART_GetEnabledInterrupts(LPUART0) )
	{
        /* obtiene dato recibido por puerto serie */
	    data = LPUART_ReadByte(LPUART0);

		/* pone dato en ring buffer */
		ringBuffer_putData(pRingBufferRx, data);

	}

    /* Interrupción de TX */
	if ( (kLPUART_TransmissionCompleteFlag)            & LPUART_GetStatusFlags(LPUART0) &&
	         (kLPUART_TransmissionCompleteInterruptEnable) & LPUART_GetEnabledInterrupts(LPUART0) )
	{
		// Desahabilita interrupciones de transmisión. Se vuelven a
		// habilitar en transceptor_envDatosDMA()
		LPUART_DisableInterrupts(LPUART0, kLPUART_TransmissionCompleteInterruptEnable);
		LPUART_ClearStatusFlags(LPUART0, kLPUART_TransmissionCompleteFlag);

	}

	/* Interrupción de OverRun */
	if ((kLPUART_RxOverrunFlag) 					   & LPUART_GetStatusFlags(TR_UART) &&
		(kLPUART_RxOverrunInterruptEnable)			   & LPUART_GetEnabledInterrupts(TR_UART))
	{
		// Se limpia la bandera de OverRun. La interfaz de UART se detinen
		// mientras esta bandera esta setea, aunque no esté habilitada
		// la interrupción de OverRun. Por eso hay que limpiarla manualmente.
		LPUART_ClearStatusFlags(TR_UART, kLPUART_RxOverrunFlag);
	}

	checkUartErrors();
}


/*==================[end of file]============================================*/

