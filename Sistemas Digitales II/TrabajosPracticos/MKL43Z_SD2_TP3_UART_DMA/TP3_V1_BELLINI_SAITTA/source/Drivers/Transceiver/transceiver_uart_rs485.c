///*
// *  tranceptor.c
// *
// *  Created on: 06/06/2024
// *      Author: Bellini y Saitta
// */
//
//#include <Drivers/Transceiver/transceiver_uart_rs485.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <string.h>
//#include <stdint.h>
//#include "fsl_clock.h"
//#include "fsl_lpuart.h"
//#include "fsl_port.h"
//#include "fsl_lpuart_dma.h"
//#include "fsl_dmamux.h"
//#include "board.h"
//#include "MKL43Z4.h"
//#include "Drivers/Board/SD2_board.h"
//#include "ringBuffer.h"
//
///*==================[macros and definitions]=================================*/
//#define TR_UART		        	LPUART1
//#define LPUART_TX_DMA_CHANNEL   0U
//#define TX_BUFFER_DMA_SIZE      32
//#define RX_RING_BUFFER_SIZE	    32
//
//#define TR_RX_PORT	        PORTE
//#define TR_RX_PIN	        1U
//#define TR_TX_PORT	        PORTE
//#define TR_TX_PIN	        0U
//#define TR_EN_PORT	        PORTB
//#define TR_EN_GPIO	        GPIOB
//#define TR_EN_PIN	        0
//
////#define RS485_DE_PORT       PORTC
////#define RS485_DE_GPIO       GPIOC
////#define RS485_DE_PIN        1
////#define RS485_RE_PORT       PORTC
////#define RS485_RE_GPIO       GPIOC
////#define RS485_RE_PIN        2
//
///*==================[internal data declaration]==============================*/
//static uint8_t txBuffer_dma[TX_BUFFER_DMA_SIZE];
//static lpuart_dma_handle_t LPUARTDmaHandle;
//static dma_handle_t LPUARTTxDmaHandle;
//static volatile bool txOnGoing = false;
////static volatile bool listoParaLectura = false;
////static volatile int32_t timer = ESPERA_PARA_LECTURA;
//static void* pRingBufferRx;
//
///*==================[internal data declaration]==============================*/
//static const board_gpioInfo_type board_gpioUART_ControlLine[] =
//{
//    {PORTE, GPIOE, 29},    /* RE */
//    {PORTE, GPIOE, 30},    /* DE */
//};
//
///*==================[internal functions definition]==========================*/
//
//static void LPUART_UserCallback(LPUART_Type *base, lpuart_dma_handle_t *handle, status_t status, void *userData)
//{
//    if (kStatus_LPUART_TxIdle == status)
//    {
//        txOnGoing = false;
//    }
//}
//
///* Funciones para habilitar la transmisión y recepción de UART/RS485 */
//void transceiver_habilitarTx() {
//    GPIO_PinWrite(board_gpioUART_ControlLine[1].gpio, board_gpioUART_ControlLine[1].pin, 1); // DE = 1
//    GPIO_PinWrite(board_gpioUART_ControlLine[0].gpio, board_gpioUART_ControlLine[0].pin, 0); // RE = 0
//}
//void transceiver_habilitarRx() {
//	GPIO_PinWrite(board_gpioUART_ControlLine[0].gpio, board_gpioUART_ControlLine[0].pin, 1); // DE = 0
//	GPIO_PinWrite(board_gpioUART_ControlLine[1].gpio, board_gpioUART_ControlLine[1].pin, 0); // RE = 1
//}
//
///* ==================================================================================================================== */
//
//// Inicialización del transceptor
//void transceiver_rs485_uart_init()
//{
//	lpuart_config_t lpuart_config;	/* UART Config */
//
//    txOnGoing=false;
//
//    pRingBufferRx = ringBuffer_init(RX_RING_BUFFER_SIZE);	/* Inicio buffer para recepción */
//
//    /* Inicializa los clocks de los puertos que se utilizan */
//    CLOCK_EnableClock(kCLOCK_Lpuart1);
//    CLOCK_EnableClock(kCLOCK_PortE);
//    CLOCK_EnableClock(kCLOCK_PortB);
//    CLOCK_EnableClock(kCLOCK_PortC);
//
//    /* ====================== [CONFIGURACION PIN EN] ============================================ */
//	const port_pin_config_t port_tranceptor_config =
//	{
//		/* Internal pull-up/down resistor is disabled */
//		.pullSelect = kPORT_PullDisable,
//		/* Fast slew rate is configured */
//		.slewRate = kPORT_SlowSlewRate,
//		/* Passive filter is disabled */
//		.passiveFilterEnable = kPORT_PassiveFilterDisable,
//		/* Low drive strength is configured */
//		.driveStrength = kPORT_HighDriveStrength,
//		/* Pin is configured as GPIO */
//		.mux = kPORT_MuxAsGpio,
//	};
//	const gpio_pin_config_t gpio_tranceptor_config =
//	{
//		.outputLogic = 0,
//		.pinDirection = kGPIO_DigitalOutput,
//	};
//	PORT_SetPinConfig(TR_EN_PORT, TR_EN_PIN, &port_tranceptor_config);
//	GPIO_PinInit(TR_EN_GPIO, TR_EN_PIN, &gpio_tranceptor_config);
//
//	// Habilita el pin EN para que funcione el tranceptor
//	GPIO_PinWrite(TR_EN_GPIO, TR_EN_PIN, 1);
//
//    /* ====================== [CONFIGURACION DE Y RE] ============================================ */
//	int32_t i;
//	const gpio_pin_config_t gpio_rs485_config =
//	{
//		.outputLogic = 1,
//		.pinDirection = kGPIO_DigitalOutput,
//	};
//	const port_pin_config_t port_config = {
//		/* Internal pull-up/down resistor is disabled */
//		.pullSelect = kPORT_PullDisable,
//		/* Slow slew rate is configured */
//		.slewRate = kPORT_SlowSlewRate,
//		/* Passive filter is disabled */
//		.passiveFilterEnable = kPORT_PassiveFilterDisable,
//		/* Low drive strength is configured */
//		.driveStrength = kPORT_LowDriveStrength,
//		/* Pin is configured as PTC3 */
//		.mux = kPORT_MuxAsGpio,
//	};
//	/* inicialización de pines de control (RE y DE)*/
//	for (i = 0 ; i < 2 ; i++)
//	{
//		PORT_SetPinConfig(board_gpioContLine[i].port, board_gpioContLine[i].pin, &port_config);
//		GPIO_PinInit(board_gpioContLine[i].gpio, board_gpioContLine[i].pin, &gpio_rs485_config);
//		GPIO_PinWrite(board_gpioUART_ControlLine[i].gpio, board_gpioUART_ControlLine[i].pin, 0); // Init en 0
//	}
//
//	/* ====================== [CONFIGURACION UART] ============================================ */
//    /* CONFIGURA RX UART1*/
//    PORT_SetPinMux(TR_RX_PORT, TR_RX_PIN, kPORT_MuxAlt3);
//    /* CONFIGURA TX UART1*/
//    PORT_SetPinMux(TR_TX_PORT, TR_TX_PIN, kPORT_MuxAlt3);
//
//	/*
//     * config.parityMode = kLPUART_ParityDisabled;
//     * config.stopBitCount = kLPUART_OneStopBit;
//     * config.enableTx = false;
//     * config.enableRx = false;
//     */
//    LPUART_GetDefaultConfig(&lpuart_config);
//    lpuart_config.baudRate_Bps = 9600;
//    lpuart_config.parityMode = kLPUART_ParityDisabled;
//    lpuart_config.stopBitCount = kLPUART_OneStopBit;
//    lpuart_config.enableTx = true;
//    lpuart_config.enableRx = true;
//
//    LPUART_Init(TR_UART, &lpuart_config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
//
//    /* Habilita interrupciones */
//    LPUART_EnableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
//    LPUART_EnableInterrupts(TR_UART, kLPUART_RxDataRegFullInterruptEnable);
//    LPUART_EnableInterrupts(TR_UART, kLPUART_RxOverrunInterruptEnable);
//    EnableIRQ(LPUART1_IRQn);
//
//    /* ====================== [CONFIGURACION DMA (sólo para TX)] ============================================ */
//    /* Init DMAMUX */
//    DMAMUX_Init(DMAMUX0);
//
//    /* Set channel for LPUART  */
//    DMAMUX_SetSource(DMAMUX0, LPUART_TX_DMA_CHANNEL, kDmaRequestMux0LPUART1Tx);
//    DMAMUX_EnableChannel(DMAMUX0, LPUART_TX_DMA_CHANNEL);
//
//    /* Init the DMA module */
//    DMA_Init(DMA0);
//    DMA_CreateHandle(&LPUARTTxDmaHandle, DMA0, LPUART_TX_DMA_CHANNEL);
//
//    /* Create LPUART DMA handle. */
//    LPUART_TransferCreateHandleDMA(
//            TR_UART,
//            &LPUARTDmaHandle,
//            LPUART_UserCallback,
//            NULL,
//            &LPUARTTxDmaHandle,
//            NULL);
//	}
//
///** \brief recibe datos por puerto serie accediendo al RB
// **
// ** \param[inout] pBuf buffer a donde guardar los datos
// ** \param[in] size tamaño del buffer
// ** \return cantidad de bytes recibidos
// **/
//int32_t transceiver_recDatos(uint8_t *pBuf, int32_t size)
//{
//    int32_t ret = 0;
//
//    // Habilitar la recepción antes de recibir los datos
//    transceiver_habilitarRx();
//
//    /* entra sección de código crítico */
//     NVIC_DisableIRQ(LPUART1_IRQn);
//
//    while (!ringBuffer_isEmpty(pRingBufferRx) && ret < size)
//    {
//        uint8_t dato;
//        ringBuffer_getData(pRingBufferRx, &dato);
//        pBuf[ret] = dato;
//        ret++;
//    }
//
//    /* sale de sección de código crítico */
//    NVIC_EnableIRQ(LPUART1_IRQn);
//
//    return ret;
//}
//
///** \brief envía datos por puerto serie accediendo a memoria RAM
// **
// ** \param[inout] pBuf buffer a donde estan los datos a enviar
// ** \param[in] size tamaño del buffer
// ** \return cantidad de bytes enviados
// **/
//int32_t transceiver_envDatosDMA(uint8_t *pBuf, int32_t size)
//{
//    lpuart_transfer_t xfer;	// Estructura de la transferencia
//
//    if (txOnGoing || size == 0){	// Si hay una transacción en curso, devuelve 0
//        size = 0;
//    }
//    else{
//        if (size > TX_BUFFER_DMA_SIZE)
//            size = TX_BUFFER_DMA_SIZE;
//
//        /* Habilitar la transmisión antes de enviar los datos */
//        transceiver_habilitarTx();
//
//        /* Copio los datos a una variable para no depender del buffer original */
//        memcpy(txBuffer_dma, pBuf, size);
//
//        /* Configuraciones de la transferencia */
//        xfer.data = txBuffer_dma;
//        xfer.dataSize = size;
//
//        /* Defino el estado de transferencia en proceso */
//        txOnGoing = true;
//
//        /* Realizo transferencia por DMA y habilito interrupción */
//        LPUART_TransferSendDMA(TR_UART, &LPUARTDmaHandle, &xfer);
//        LPUART_EnableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
//    }
//
//    return size;
//}
//
//
//// Manejo de interrupciones de la UART1
//void LPUART1_IRQHandler(void)
//{
//    uint8_t data;
//
//    // Verifica si la bandera de registro de datos RX está activa y si la interrupción está habilitada
//    if ((kLPUART_RxDataRegFullFlag) & LPUART_GetStatusFlags(TR_UART) &&
//        (kLPUART_RxDataRegFullInterruptEnable) & LPUART_GetEnabledInterrupts(TR_UART))
//    {
//        /* Lee el byte recibido */
//        data = LPUART_ReadByte(TR_UART);
//
//        /* Pone el byte recibido en el buffer */
//        ringBuffer_putData(pRingBufferRx, data);
//
//        /* Limpia la bandera de registro de datos RX */
//        LPUART_ClearStatusFlags(TR_UART, kLPUART_RxDataRegFullFlag);
//    }
//
//    // Verifica si la bandera de transmisión completa está activa y si la interrupción está habilitada
//    if ((kLPUART_TransmissionCompleteFlag) & LPUART_GetStatusFlags(TR_UART) &&
//        (kLPUART_TransmissionCompleteInterruptEnable) & LPUART_GetEnabledInterrupts(TR_UART))
//    {
//        /* Deshabilita las interrupciones de transmisión */
//        LPUART_DisableInterrupts(TR_UART, kLPUART_TransmissionCompleteInterruptEnable);
//
//        /* Limpia la bandera de transmisión completa */
//        LPUART_ClearStatusFlags(TR_UART, kLPUART_TransmissionCompleteFlag);
//
//        /* Habilito recepción para esperar el dato de vuelta */
//        transceiver_habilitarRx();
//    }
//
//    // Verifica si la bandera de OverRun está activa y si la interrupción está habilitada
//    if ((kLPUART_RxOverrunFlag) & LPUART_GetStatusFlags(TR_UART) &&
//        (kLPUART_RxOverrunInterruptEnable) & LPUART_GetEnabledInterrupts(TR_UART))
//    {
//        // Limpia la bandera de OverRun
//        LPUART_ClearStatusFlags(TR_UART, kLPUART_RxOverrunFlag);
//    }
//}
