/**
 * @file    transceiver_RS485_UART.h
 * @brief   transceiver driver for UART - RS485 com. Rx w/ ringBuffer - Tx w/ DMA.
 * @autor	Valentin Bellini & Iván Saitta
 */

#ifndef TRANSCEIVER_RS485_UART_H_
#define TRANSCEIVER_RS485_UART_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "stdbool.h"
#include <string.h>
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "board.h"
#include "fsl_lpuart_dma.h"
#include "fsl_dmamux.h"
#include "MKL43Z4.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

void transceiver_uart_rs485_init(void);

bool board_rs485_isDataAvailable(void);

/** \brief recibe datos por puerto serie accediendo al RB **/
int32_t uart_ringBuffer_recDatos(uint8_t *pBuf, int32_t size);

/** \brief envia datos por puerto serie a traves de DMA **/
int32_t uart_drv_envDatos_DMA(uint8_t *pBuf, int32_t size);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* TRANSCEIVER_RS485_UART_H_ */
