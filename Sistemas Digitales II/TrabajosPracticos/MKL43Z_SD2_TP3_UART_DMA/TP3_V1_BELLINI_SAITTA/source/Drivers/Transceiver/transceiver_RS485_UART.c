/**
 * @file    transceiver_RS485_UART.c
 * @brief   transceiver driver for UART - RS485 com. Rx w/ ringBuffer - Tx w/ DMA.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*==================[inclusions]=============================================*/

/* Project Included Files */
#include <Drivers/Transceiver/transceiver_RS485_UART.h>
#include "Drivers/Board/SD2_board.h"
#include "ringBuffer.h"
#include "debug.h"


/*==================[macros and definitions]=================================*/

/* ----------- UART Configs ---------------------------------------*/
#define TR_UART            		LPUART1
#define TR_UART_IRQn      		LPUART1_IRQn  /* LPUART1 status and error */
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

/* ----------- Controls Lines for UART1 + RS485 ---------------------------------------*/

static bool dataAvailable;

/*==================[internal functions definition]==========================*/

/* UART user callback */
static void LPUART_UserCallback(LPUART_Type *base, lpuart_dma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_TxIdle == status)
    {
        txOnGoing = false;
    }
}

static void uart_init(void){
	lpuart_config_t lpuart_config;

	CLOCK_SetLpuart1Clock(0x1U);

	/* Configura los pines RX y TX de la UART1 */
	PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);	/* TX: PORTE PIN 0 */
	PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3); 	/* RX: PORTE PIN 1 --- pag 116 ref*/

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

	lpuart_config.baudRate_Bps = TR_UART_BAUD_RATE;
	lpuart_config.parityMode = kLPUART_ParityDisabled;
	lpuart_config.stopBitCount = kLPUART_OneStopBit;
	lpuart_config.enableTx = true;
	lpuart_config.enableRx = true;

	LPUART_Init(LPUART1, &lpuart_config, CLOCK_GetFreq(kCLOCK_CoreSysClk));

	/* Habilitación de interrupciones */
	LPUART_EnableInterrupts(LPUART1, kLPUART_RxDataRegFullInterruptEnable);
	LPUART_EnableInterrupts(LPUART1, kLPUART_TransmissionCompleteInterruptEnable);
	LPUART_EnableInterrupts(LPUART1, kLPUART_RxOverrunInterruptEnable);
	EnableIRQ(LPUART1_IRQn);
}

static void dma_init(void){
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
			LPUART1,
			&LPUARTDmaHandle,
			LPUART_UserCallback,
			NULL,
			&LPUARTTxDmaHandle,
			NULL);
}

/** \brief Lee las status flags de UART y chequea de donde viene el error. **/
static void checkUartErrors(void) {
    uint32_t status = LPUART_GetStatusFlags(LPUART1);
    if (status & kLPUART_RxOverrunFlag) {
        LPUART_ClearStatusFlags(LPUART1, kLPUART_RxOverrunFlag);
        DEBUG_PRINT("UART Rx Overrun Error\n");
        //logError("UART Rx Overrun Error\n");
    }
    if (status & kLPUART_FramingErrorFlag) {
        LPUART_ClearStatusFlags(LPUART1, kLPUART_FramingErrorFlag);
        DEBUG_PRINT("UART Framing Error\n");
        //logError("UART Framing Error\n");
    }
    if (status & kLPUART_ParityErrorFlag) {
        LPUART_ClearStatusFlags(LPUART1, kLPUART_ParityErrorFlag);
        DEBUG_PRINT("UART Parity Error\n");
        //logError("UART Parity Error\n");
    }
}

/*==================[external functions definition]==========================*/

void transceiver_uart_rs485_init(void){
	/* RS485 Control Lines (RE y DE) tienen su init de GPIO en SD2_board.c  */
	txOnGoing = false;
	dataAvailable = false;
    pRingBufferRx = ringBuffer_init(RX_RING_BUFFER_SIZE);
    uart_init();
	dma_init();
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
        if (size > TX_BUFFER_DMA_SIZE)
            size = TX_BUFFER_DMA_SIZE;

        /* Habilitar la transmisión antes de enviar los datos */
		board_setRS485_controlLine(RS485_RE_PIN, true);
		board_setRS485_controlLine(RS485_DE_PIN, true);

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

		/* pone dato en ring buffer */
		ringBuffer_putData(pRingBufferRx, data);

		dataAvailable = true;

		LPUART_ClearStatusFlags(TR_UART,kLPUART_RxDataRegFullFlag);
	}

	/* ============= [Interrupción de TX] =================================================== */
	if ( (kLPUART_TransmissionCompleteFlag) & LPUART_GetStatusFlags(TR_UART) &&
		 (kLPUART_TransmissionCompleteInterruptEnable) & LPUART_GetEnabledInterrupts(TR_UART) )
	{
		/* Desahabilita interrupciones de transmisión. Se vuelven a habilitar en el envio de datos por dma. */
		LPUART_DisableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
		LPUART_ClearStatusFlags(TR_UART, kLPUART_TransmissionCompleteFlag);

		/* Habilito recepción para esperar el dato de vuelta */
		board_setRS485_controlLine(RS485_RE_PIN, false);
		board_setRS485_controlLine(RS485_DE_PIN, false);
	}

	/* Chequeo de errores en UART leyendo banderas */
	checkUartErrors();
}

/*==================[end of file]============================================*/

