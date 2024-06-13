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

/* ----------- Controls Lines for UART1 + RS485 ---------------------------------------*/
static const board_gpioInfo_type board_gpioUART_ControlLine[] =
{
    {PORTE, GPIOE, 29},    /* RE */
    {PORTE, GPIOE, 30},    /* DE */
};

static bool dataAvailable;

/*==================[internal functions declaration]=========================*/
static void LPUART_UserCallback(LPUART_Type *base, lpuart_dma_handle_t *handle, status_t status, void *userData);
static void init_transceiver_GPIO(void); /* Init control lines for UART1 + RS485 */
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

static void rs485_RE(bool est)
{
    if (est)
    	GPIO_PortSet(board_gpioUART_ControlLine[0].gpio, 1<<board_gpioUART_ControlLine[0].pin);
    else
    	GPIO_PortClear(board_gpioUART_ControlLine[0].gpio, 1<<board_gpioUART_ControlLine[0].pin);
}

static void rs485_DE(bool est)
{
    if (est)
    	GPIO_PortSet(board_gpioUART_ControlLine[1].gpio, 1<<board_gpioUART_ControlLine[1].pin);
    else
    	GPIO_PortClear(board_gpioUART_ControlLine[1].gpio, 1<<board_gpioUART_ControlLine[1].pin);
}

/* Initialize GPIO for transceiver control lines */
void init_transceiver_GPIO(void){
	/* ====================== [CONFIGURACION DE Y RE] ============================================ */
	int32_t i;
	const gpio_pin_config_t gpio_rs485_config = {
		.outputLogic = 1,
		.pinDirection = kGPIO_DigitalOutput,
	};
	const port_pin_config_t port_config = {
		/* Internal pull-up/down resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Slow slew rate is configured */
		.slewRate = kPORT_SlowSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as PTC3 */
		.mux = kPORT_MuxAsGpio,
	};
	/* inicialización de pines de control (RE y DE)*/
	for (i = 0 ; i < 2 ; i++)
	{
		PORT_SetPinConfig(board_gpioUART_ControlLine[i].port, board_gpioUART_ControlLine[i].pin, &port_config);
		GPIO_PinInit(board_gpioUART_ControlLine[i].gpio, board_gpioUART_ControlLine[i].pin, &gpio_rs485_config);
	}
	rs485_RE(false);
	rs485_DE(false);
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
	init_transceiver_GPIO();

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
        if (size > TX_BUFFER_DMA_SIZE)
            size = TX_BUFFER_DMA_SIZE;

        /* Habilitar la transmisión antes de enviar los datos */
        rs485_RE(true);
		rs485_DE(true);

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
		/* Desahabilita interrupciones de transmisión. Se vuelven a habilitar en el envio de datos por dma. */
		LPUART_DisableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
		LPUART_ClearStatusFlags(TR_UART, kLPUART_TransmissionCompleteFlag);

		/* Habilito recepción para esperar el dato de vuelta */
		rs485_RE(false);
		rs485_DE(false);
	}

	/* Chequeo de errores en UART leyendo banderas */
	checkUartErrors();
}

/*==================[end of file]============================================*/

