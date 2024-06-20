/* Copyright, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright, Diego Alegrechi
 * Copyright, Gustavo Muro
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
#include <Drivers/RingBuffer/ringBuffer.h>
#include "stdlib.h"

/*==================[macros and definitions]=================================*/

typedef struct
{
    int32_t indexRead;		// Índice de lectura
    int32_t indexWrite;		// Ïndice de escritura
    int32_t count;			// Número de elementos actualmente en el buffer
    int32_t size;			// Tamaño total del buffer
    uint8_t *pBuf;			// Puntero al buffer de datos
}ringBuferData_struct;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* Inicializa un buffer circular con el tamaño especificado
 * Devuelve un puntero a la estructura del buffer inicializado
 */
void *ringBuffer_init(int32_t size)
{
    ringBuferData_struct *rb;

    /* TODO: verificar puntero null*/
    rb = malloc(sizeof(ringBuferData_struct));
    if (rb == NULL) {
		return NULL; // Error en la asignación de memoria
	}

    /* TODO: verificar puntero null*/
    rb->pBuf = malloc(size);
    if (rb->pBuf == NULL) {
		free(rb);
		return NULL; // Error en la asignación de memoria
	}

    /* Inidialización de la estructura */
    rb->indexRead = 0;
    rb->indexWrite = 0;
    rb->count = 0;
    rb->size = size;

    return rb;
}

/* Libera la memoria asignada al buffer circular
 * Toma un puntero a la estructura del buffer
 */
void ringBuffer_deInit(void *rb)
{
	ringBuferData_struct *prb = rb;
	free(prb->pBuf);	// Libera la memoria del buffer de datos
	free(prb);			// Libera la memoria de la estructura
}

/* Inserta un dato en el buffer circular
 * Devuelve true si la operación fue exitosa, false si se perdió un dato
 */
bool ringBuffer_putData(void *pRb, uint8_t data)
{
    ringBuferData_struct *rb = pRb;
    bool ret = true;

    rb->pBuf[rb->indexWrite] = data;	// Escribe el dato en el índice de escritura

    rb->indexWrite++;					// Incrementa el índice de escritura

    if (rb->indexWrite >= rb->size)	rb->indexWrite = 0;	// Si se llega al final del buffer, vuelve al principio

    if (rb->count < rb->size) {			// Si el buffer no está lleno, incrementa el contador
    	rb->count++;
    }
    else{
        /* si el buffer está lleno incrementa en uno indexRead
         * haciendo que se pierda el dato más viejo y devuelve
         * false para indicar que se estan perdiendo datos */
        rb->indexRead++;
        if (rb->indexRead >= rb->size)	// Si se llega al final del buffer, vuelve al principio
            rb->indexRead = 0;
        ret = false; // Indica que se perdió un dato
    }

    return ret;
}

/* Extrae un dato del buffer circular
 * Almacena el dato extraído en la dirección proporcionada
 * Devuelve true si la operación fue exitosa, false si el buffer estaba vacío
 */
bool ringBuffer_getData(void *pRb, uint8_t *data)
{
    ringBuferData_struct *rb = pRb;
    bool ret = true;

    if (rb->count){	// Si hay datos en el buffer

        *data = rb->pBuf[rb->indexRead]; 	// Lee el dato en el índice de lectura

        rb->indexRead++;					// Incrementa el índice de lectura
        if (rb->indexRead >= rb->size)		// Si se llega al final del buffer, vuelve al principio
            rb->indexRead = 0;
        rb->count--;						// Decrementa el contador de elementos
    }
    else {
        ret = false;		// Indica que el buffer estaba vacío
    }

    return ret;
}

/* Verifica si el buffer circular está lleno
 * Devuelve true si está lleno, false en caso contrario
 */
bool ringBuffer_isFull(void *pRb){
    ringBuferData_struct *rb = pRb;

    return rb->count == rb->size; // El buffer está lleno si el contador es igual al tamaño
}

/* Verifica si el buffer circular está vacío
 * Devuelve true si está vacío, false en caso contrario
 */
bool ringBuffer_isEmpty(void *pRb){
    ringBuferData_struct *rb = pRb;

    return rb->count == 0; // El buffer está vacío si el contador es cero
}

/*==================[end of file]============================================*/
