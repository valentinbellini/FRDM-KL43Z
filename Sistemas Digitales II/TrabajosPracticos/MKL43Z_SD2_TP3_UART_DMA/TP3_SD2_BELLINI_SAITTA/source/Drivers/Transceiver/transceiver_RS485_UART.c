/**
 * @file    transceiver_RS485_UART.c
 * @brief   transceiver driver for UART - RS485 com. Rx w/ ringBuffer - Tx w/ DMA.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/

/* Project Included Files */
#include <Drivers/Transceiver/transceiver_RS485_UART.h>
#include "Drivers/Board/SD2_board.h"
#include "Drivers/RingBuffer/ringBuffer.h"
#include "debug.h"

/*==================[macros and definitions]=================================*/

#define TR_UART              	LPUART1
#define RS485_UART_IRQn      	LPUART1_IRQn  /* LPUART1 status and error */
#define DMA_REQUEST_SRC      	kDmaRequestMux0LPUART1Tx
#define TR_UART_BAUD_RATE		9600


/* ----------- Communication Configs ---------------------------------------*/
#define TX_BUFFER_DMA_SIZE  	32
#define RX_RING_BUFFER_SIZE		32
#define LPUART_TX_DMA_CHANNEL 	0U	/* DMA Channel for Tx */


/*==================[internal data declaration]==============================*/
static uint8_t txBuffer_dma[TX_BUFFER_DMA_SIZE]; /* Buffer for storing data to be sent via DMA. */
static lpuart_dma_handle_t LPUARTDmaHandle; /* Handle for managing UART data Tx and Rx using DMA. */
static dma_handle_t LPUARTTxDmaHandle; /* Handle for managing UART Tx DMA transfers. */
static void* pRingBufferRx; /* Pointer to the ring buffer for storing received data. */
volatile bool txOnGoing = false; /* Flag indicating if a transmission is in progress. */


static bool dataAvailable;

/*==================[internal functions declaration]=========================*/

static void LPUART_UserCallback(LPUART_Type *base, lpuart_dma_handle_t *handle, status_t status, void *userData);
static void checkUartErrors(void);

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

	/* ============= [Interrupción de OverRun] =================================================== */
    if ((kLPUART_RxOverrunFlag) & LPUART_GetStatusFlags(TR_UART) &&
    	(kLPUART_RxOverrunInterruptEnable)	& LPUART_GetEnabledInterrupts(TR_UART))
    {
        LPUART_ClearStatusFlags(TR_UART, kLPUART_RxOverrunFlag);
        DEBUG_PRINT("UART Rx Overrun Error\n");

    }
    if ((kLPUART_FramingErrorFlag) & LPUART_GetStatusFlags(TR_UART) &&
        (kLPUART_FramingErrorInterruptEnable)	& LPUART_GetEnabledInterrupts(TR_UART))
    {
        LPUART_ClearStatusFlags(TR_UART, kLPUART_FramingErrorFlag);
        DEBUG_PRINT("UART Framing Error\n");
    }
    if ((kLPUART_ParityErrorFlag) & LPUART_GetStatusFlags(TR_UART) &&
       (kLPUART_ParityErrorInterruptEnable)	& LPUART_GetEnabledInterrupts(TR_UART))
    {
        LPUART_ClearStatusFlags(TR_UART, kLPUART_ParityErrorFlag);
        DEBUG_PRINT("UART Parity Error\n");
    }
}

/*==================[external functions definition]==========================*/

void transceiver_init(void){

	lpuart_config_t lpuart_config;
	txOnGoing = false;
    pRingBufferRx = ringBuffer_init(RX_RING_BUFFER_SIZE);

	/* ====================== [CONFIGURACION UART1] ============================================ */

	CLOCK_SetLpuart1Clock(0x1U);
	PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);	/* RX: PORTE PIN 1 --- pag 116 ref*/
	PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);	/* TX: PORTE PIN 0 */

	/* DEFAULT CONFIG:
	 *	 lpuartConfig->baudRate_Bps = 115200U;
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
	LPUART_GetDefaultConfig(&lpuart_config);

	lpuart_config.baudRate_Bps = TR_UART_BAUD_RATE; 	// 9600
	lpuart_config.parityMode = kLPUART_ParityDisabled;
	lpuart_config.stopBitCount = kLPUART_OneStopBit;
	lpuart_config.enableTx = true;
	lpuart_config.enableRx = true;

	LPUART_Init(TR_UART, &lpuart_config, CLOCK_GetFreq(kCLOCK_CoreSysClk));

	dataAvailable = false;

	/* Habilitación de interrupciones */
	LPUART_EnableInterrupts(TR_UART, kLPUART_RxDataRegFullInterruptEnable);
	LPUART_EnableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
	LPUART_EnableInterrupts(TR_UART, kLPUART_RxOverrunInterruptEnable);
	LPUART_EnableInterrupts(TR_UART, kLPUART_FramingErrorInterruptEnable);
	LPUART_EnableInterrupts(TR_UART, kLPUART_ParityErrorInterruptEnable);

	EnableIRQ(RS485_UART_IRQn);

	/* ====================== [CONFIGURACION DMA (TX)] ============================================ */
	/* Init DMAMUX */
	DMAMUX_Init(DMAMUX0);

	/* Set channel for LPUART  */
	DMAMUX_SetSource(DMAMUX0, LPUART_TX_DMA_CHANNEL, DMA_REQUEST_SRC);
	DMAMUX_EnableChannel(DMAMUX0, LPUART_TX_DMA_CHANNEL);

	/* Init the DMA module */
	DMA_Init(DMA0);
	DMA_CreateHandle(&LPUARTTxDmaHandle, DMA0, LPUART_TX_DMA_CHANNEL);

	/* Create LPUART DMA handle. */
	LPUART_TransferCreateHandleDMA(
			TR_UART,
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
    dataAvailable = false;

    /* Entra sección de código crítico --> deshabilitación de interrupciones */
    __disable_irq();

    while (!ringBuffer_isEmpty(pRingBufferRx) && ret < size)
    {
        ringBuffer_getData(pRingBufferRx, &pBuf[ret]);
        ret++;
    }

    /* Sale de sección de código crítico --> habilitación de interrupciones */
    __enable_irq();

    return ret;
}

/** \brief envía datos por puerto serie accediendo a memoria RAM
 **
 ** \param[inout] pBuf buffer a donde estan los datos a enviar
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes enviados
 **/
int32_t uart_drv_envDatos_DMA(uint8_t *pBuf, int32_t size)
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
        if (size > TX_BUFFER_DMA_SIZE) size = TX_BUFFER_DMA_SIZE;

        rs485_RE(true); /* RE = 1: Deshabilitado y el módulo no puede recibir datos desde el bus RS485 */
		rs485_DE(true); /* Habilitar la transmisión hacia el bus RS485 antes de enviar los datos */

        /* Copio los datos a una variable para no depender del buffer original */
		memcpy(txBuffer_dma, pBuf, size);

		/* Configuraciones de la transferencia */
		xfer.data = txBuffer_dma;
		xfer.dataSize = size;

		/* Defino el estado de transferencia en proceso */
		txOnGoing = true;

		/* Realizo transferencia por DMA y habilito interrupción */
        LPUART_TransferSendDMA(TR_UART, &LPUARTDmaHandle, &xfer);
        LPUART_EnableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
    }

    return size;
}

bool board_rs485_isDataAvailable(void)
{
    return dataAvailable;
}

void LPUART1_IRQHandler(void){

	uint8_t data; /* variable para guardar dato de lectura de Rx */

	/* ============= [Interrupción de RX] =================================================== */

	if ( (kLPUART_RxDataRegFullFlag) & LPUART_GetStatusFlags(TR_UART) &&
		 (kLPUART_RxDataRegFullInterruptEnable) & LPUART_GetEnabledInterrupts(TR_UART) )
	{
		/* obtiene dato recibido por puerto serie */
		data = LPUART_ReadByte(TR_UART);
		dataAvailable = true;

		/* pone dato en ring buffer */
		ringBuffer_putData(pRingBufferRx, data);

		LPUART_ClearStatusFlags(TR_UART,kLPUART_RxDataRegFullFlag);
	}

	/* ============= [Interrupción de TX] =================================================== */

	if ( (kLPUART_TransmissionCompleteFlag) & LPUART_GetStatusFlags(TR_UART) &&
		 (kLPUART_TransmissionCompleteInterruptEnable) & LPUART_GetEnabledInterrupts(TR_UART) )
	{
		/* Desahabilita interrupción de transmisión completa.
		 * Se vuelve a habilitar en el envio de datos por DMA. */
		LPUART_DisableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
		LPUART_ClearStatusFlags(TR_UART, kLPUART_TransmissionCompleteFlag);

		/* Habilito recepción para esperar el dato de vuelta */
		rs485_RE(false); /* Driver Enable en 0: deshabilitado y no se transmiten datos hacia el bus RS485 */
		rs485_DE(false); /* Receiver Enable en 0: habilitado y puede recibir datos desde el busRS485 */
	}

	/* Chequeo de errores en UART leyendo banderas */
	checkUartErrors();
}

/*==================[end of file]============================================*/

