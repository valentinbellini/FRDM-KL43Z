/**
 * @file    transceiver_RS485_UART.h
 * @brief   transceiver driver for UART - RS485 com. Rx w/ ringBuffer - Tx w/ DMA.
 * @autor	Valentin Bellini & Iván Saitta
 */

#ifndef UART_RING_BUFFER_H_
#define UART_RING_BUFFER_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

void transceiver_init(void);

/** \brief recibe datos por puerto serie accediendo al RB
 **
 ** \param[inout] pBuf buffer a donde guardar los datos
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes recibidos
 **/
int32_t uart_ringBuffer_recDatos(uint8_t *pBuf, int32_t size);

/** \brief envia datos por puerto serie a traves de DMA **/
int32_t uart_drv_envDatos_DMA(uint8_t *pBuf, int32_t size);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* UART_RING_BUFFER_H_ */
