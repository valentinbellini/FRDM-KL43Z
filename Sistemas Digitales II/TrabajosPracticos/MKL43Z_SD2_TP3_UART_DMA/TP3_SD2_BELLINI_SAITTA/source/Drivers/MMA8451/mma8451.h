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

#ifndef MMA8451_H_
#define MMA8451_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "stdbool.h"
#include "Drivers/MMA8451/mma8451_registers.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define MMA8451_I2C     I2C0 	// brief: puerto I2C utilizado en el acelerómetro

//#define INT1_PORT       PORTC
//#define INT1_GPIO       GPIOC
//#define INT1_PIN        5

#define INT2_PORT       PORTD
#define INT2_GPIO       GPIOD
#define INT2_PIN        1

/*==================[typedef]================================================*/

typedef enum{ // DR_enum
    DR_800hz = 0b000,
    DR_400hz = 0b001,
    DR_200hz = 0b010,
    DR_100hz = 0b011,
    DR_50hz = 0b100,
    DR_12p5hz = 0b101,
    DR_6p25hz = 0b110,
    DR_1p56hz = 0b111,
}DR_enum;

typedef enum{
	// Set dynamic range en Format Scale (default 2G)
	FS_2G = 0b00,
	FS_4G = 0b01,
	FS_8G = 0b10,
}FS_enum;

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

/* ---------------- [FUNCIONES DE REGISTROS] ------------------------------ */

void readAccsFromRegisters(void);

/* ---------------- [FUNCIONES DE CONFIGURACIONES] ------------------------------ */
void mma8451_freefall_config(void);
void mma8451_dataReady_config(void);
void mma8451_clearInterruptions_config(void);

/* ---------------- [FUNCIONES GETTERS] ------------------------------ */
int16_t mma8451_getAcX(void);	// Lee lectura en eje X, devuelve lectura en centecimas de g
int16_t mma8451_getAcY(void);	// Lee lectura en eje Y, devuelve lectura en centecimas de g
int16_t mma8451_getAcZ(void);	// Lee lectura en eje Z, devuelve lectura en centecimas de g


bool mma8451_getFreefallInterruptStatus(void);
bool mma8451_getDataReadyInterruptStatus(void);

/*==================[end of file]============================================*/
#endif /* MMA8451_H_ */
