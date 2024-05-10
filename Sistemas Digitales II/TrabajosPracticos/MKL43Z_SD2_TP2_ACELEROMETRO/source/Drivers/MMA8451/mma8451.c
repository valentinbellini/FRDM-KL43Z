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

/*==================[inclusions]=============================================*/
#include "fsl_debug_console.h"
#include "fsl_smc.h"

#include <Drivers/MMA8451/mma8451.h>
#include "stdint.h"
#include "stdbool.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "Drivers/PowerMode/power_mode_switch.h"

/*==================[macros and definitions]=================================*/

#define MMA8451_I2C_ADDRESS     (0x1d)	// Direccón del acelerómetro (se la debo indicar al I2C)

typedef struct {
    int16_t X;
    int16_t Y;
    int16_t Z;
} Readings;

/*==================[internal data declaration]==============================*/

static Readings read;
static bool isInterruptionByFreeFall = false, isInterruptionByDR = false;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void config_port_int1(void){
/*
 * Brief: En resumen, esta función configura el pin INT1 para actuar como una entrada digital
 * y configura la interrupción asociada al mismo en un microcontrolador KL43.
 * */
	const port_pin_config_t port_int1_config = {
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
	const gpio_pin_config_t gpio_int1_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 0U
	};

	PORT_SetPinConfig(INT1_PORT, INT1_PIN, &port_int1_config);
	GPIO_PinInit(INT1_GPIO, INT1_PIN, &gpio_int1_config);

	/* Interrupt polarity active high, or active low. Default value: 0.
	   0: Active low; 1: Active high. VER REGISTRO CTRL_REG3 */
	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN, kPORT_InterruptLogicZero);

	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn, 0);
}

// Funciones de lectura y escritura del acelerómetro mediante I2C:
static uint8_t mma8451_read_reg(uint8_t addr){
	i2c_master_transfer_t masterXfer;
    uint8_t ret;

	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress = MMA8451_I2C_ADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = addr;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &ret;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	return ret;
}
static void mma8451_read_multiple_reg(uint8_t addr, uint8_t *pbuffer, uint8_t size){ // size_t size
	// La funcino carga los datos leidos en el buffer
	// Ej: Leer desde 0x01 a 0x06 (size 6 desde address inicial: 0x01)
	i2c_master_transfer_t masterXfer;

	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress = MMA8451_I2C_ADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = addr;
	masterXfer.subaddressSize = 1; 			// Direccion despues del registro.
	masterXfer.data = pbuffer;
	masterXfer.dataSize = size;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}
static void mma8451_write_reg(uint8_t addr, uint8_t data){
	i2c_master_transfer_t masterXfer;

    memset(&masterXfer, 0, sizeof(masterXfer));

    /* Estructura definida por el fabricante del micro para transmitir datos al i2c de manera sencilla: */
    masterXfer.slaveAddress = MMA8451_I2C_ADDRESS;	/*!< 7-bit slave address. */
	masterXfer.direction = kI2C_Write;				/*!< A transfer direction, read or write. */
	masterXfer.subaddress = addr;					/*!< Address del registro del acelerómetro para la acción necesaria. Ej: 0x2d es interrupt enable (Transferred MSB first) */
	masterXfer.subaddressSize = 1;					/*!< A size of the command buffer. */
	masterXfer.data = &data;						/*!< A transfer buffer. */
	masterXfer.dataSize = 1;						/*!< A transfer size. */
	masterXfer.flags = kI2C_TransferDefaultFlag;	/*!< A transfer flag which controls the transfer. Tipo de transferencia que envias (normalmente la estandar, podria decirse que no termine con stop u otras cosas) */

    I2C_MasterTransferBlocking(I2C0, &masterXfer); // Transferencia blocking: más fácil de usar pero mas ineficiente desde el punto de vista del procesador. Pregunta al i2c si termino todo el tiempo.
}

/*==================[external functions definition]==========================*/

/* --------- Configuraciones de registros ------------------ */
void mma8451_freefall_config(void){
	// Registers definitions
	CTRL_REG1_t ctrl_reg1;
	CTRL_REG4_t ctrl_reg4;
	CTRL_REG5_t ctrl_reg5;
	FF_MT_CFG_t FF_MT_CFG_reg;
	FF_MT_THS_t FF_MT_THS_reg;
	FF_MT_COUNT_t FF_MT_COUNT_reg;
	XYZ_DATA_CFG_t data_cfg;

//	mma8451_write_reg(0x2B, 64); 		// RST the MMA8451
//	while(mma8451_read_reg(0x2B)){
//		 // Espero a que el reset termine
//	}

	/* Put the device in Standby Mode: Register 0x2A CTRL_REG1 */
	ctrl_reg1.ACTIVE = 0;
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	/* Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0, Enabling X, Y, Z and the Latch */
	FF_MT_CFG_reg.XEFE = 1 ; // Estos 3 registros X Y Z se ponen en 1 para que levanten una bandera cuando la acc supera el threshold
	FF_MT_CFG_reg.YEFE = 1 ;
	FF_MT_CFG_reg.ZEFE = 1 ;
	FF_MT_CFG_reg.OAE = 0 ; // Motion detect/freefall detect flag selection. Default 0 (Freefall flag)
	FF_MT_CFG_reg.ELE = 1 ; // Event latch enable. Hace clear a la bandera cuando se lee el source register
	mma8451_write_reg(FF_MT_CFG_ADDRESS,FF_MT_CFG_reg.data);

	// Format Scale (2g, 4g u 8g)
	//mma8451_write_reg(XYZ_DATA_CFG_ADDRESS, 0x01); // Write the 4g dynamic range value into register 0x0E --> a 8 bit 0.032g/LSB
	data_cfg.FormatScale = FS_4G;
	data_cfg.HPF_out = 0;
	mma8451_write_reg(XYZ_DATA_CFG_ADDRESS, data_cfg.data);


	// Threshold Setting Value for the resulting acceleration < 0.2g
	FF_MT_THS_reg.data = 0x06; // Note: The step count is 0.032g/count --  0.2g/0.032g = 6.25 counts //Round to 6 counts
	mma8451_write_reg(FF_MT_THS_ADDRESS,FF_MT_THS_reg.data);

	// Set the debounce counter to eliminate false positive readings for 200Hz sample rate with a
	// requirement of 120 ms timer.
	FF_MT_COUNT_reg.data = 24; // Note: 120 ms/5 ms (steps) = 24 counts (0x18)
	mma8451_write_reg(FF_MT_COUNT_ADRESS,FF_MT_COUNT_reg.data);

	//Enable Motion/Freefall Interrupt Function in the System (CTRL_REG4)
	ctrl_reg4.INT_EN_DRDY = 0;  //Desactivo la interrupcion por Data ready en caso de que venga activada
	ctrl_reg4.INT_EN_FF_MT = 1; //Activo la interrupcion por FreeFall
	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);

	// Route the Motion/Freefall Interrupt Function to INT1 hardware pin (CTRL_REG5)
	ctrl_reg5.INT_CFG_DRDY = 0;  // Interrupt del dready en el pin INT2 para evitar problemas
	ctrl_reg5.INT_CFG_FF_MT = 1; // Interrupt Freefall Pin 1
	mma8451_write_reg(CTRL_REG5_ADDRESS, ctrl_reg5.data);

	// Put the device in Active Mode, 200 Hz
	ctrl_reg1.ACTIVE = 1; // en 0 es stanby, 1 es active.
	ctrl_reg1.DR = DR_200hz; // Dr 010, 200HZ 5ms Hz output data rate
	ctrl_reg1.ASLP_RATE = 0B00;
	ctrl_reg1.F_READ = 1;	// Fast-read mode (8 bits) --> Menor resolución pero mayor velocidad
	ctrl_reg1.LNOISE = 0;
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	config_port_int1();
}
void mma8451_dataReady_config(void){
	CTRL_REG1_t ctrl_reg1;
	CTRL_REG4_t ctrl_reg4;
	CTRL_REG5_t ctrl_reg5;
	FF_MT_CFG_t FF_MT_CFG_reg;
	XYZ_DATA_CFG_t data_cfg;

	ctrl_reg1.ACTIVE = 0; // Lo pone en standby para configurar el  registro
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	// REGISTROS DE FREEFALL, todo en valores deafult
	FF_MT_CFG_reg.XEFE =0 ;
	FF_MT_CFG_reg.YEFE =0 ;
	FF_MT_CFG_reg.ZEFE =0 ;
	FF_MT_CFG_reg.OAE = 0 ;
	FF_MT_CFG_reg.ELE = 0 ;
	mma8451_write_reg(FF_MT_CFG_ADDRESS,FF_MT_CFG_reg.data);

	//REGISTROS DE CONTROL
	ctrl_reg4.INT_EN_DRDY = 1;  // Interrupciones activas por DataReady
	ctrl_reg4.INT_EN_FF_MT = 0; // Interrupciones desactivadas por FreeFall
	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);

	ctrl_reg5.INT_CFG_DRDY = 1;  // Data Ready para interrumpir en pin 1
	ctrl_reg5.INT_CFG_FF_MT = 0; // Interrupt del freefall en el pin INT2 para evitar problemas
	mma8451_write_reg(CTRL_REG5_ADDRESS, ctrl_reg5.data);

	// Test
	//mma8451_write_reg(0x0E, 0x01); // Write the 4g dynamic range value into register 0x0E
	data_cfg.FormatScale = FS_8G;   // Rango de 8G
	data_cfg.HPF_out = 0;
	mma8451_write_reg(XYZ_DATA_CFG_ADDRESS, data_cfg.data);

	ctrl_reg1.ACTIVE = 1; 		// En 0 es stanby, en 1 es active.
	ctrl_reg1.DR = DR_200hz; 	// Dr 010, 200HZ 5ms Hz output data rate
	ctrl_reg1.ASLP_RATE = 0B00;
	ctrl_reg1.F_READ = 0; 		//
	ctrl_reg1.LNOISE = 1; 		//
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	config_port_int1();
}
void mma8451_clearInterruptions_config(void){
	CTRL_REG1_t ctrl_reg1;
	CTRL_REG4_t ctrl_reg4;

	isInterruptionByFreeFall = false;
	isInterruptionByDR = false;

	ctrl_reg1.ACTIVE = 0; // Lo pone en standby
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	// Deshabilito ambas interrupciones
	ctrl_reg4.INT_EN_DRDY = 0;
	ctrl_reg4.INT_EN_FF_MT = 0;
	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);

	config_port_int1();
}

/* --------- Getters ------------------ */
bool mma8451_getFreefallInterruptStatus(void) {
	bool ret = false;
	if(isInterruptionByFreeFall){
	    isInterruptionByFreeFall = false;
		ret = true;
	}
	return ret;
}
bool mma8451_getDataReadyInterruptStatus(void) {
	bool ret = false;
	if(isInterruptionByDR){
	    isInterruptionByDR = false;
		ret = true;
	}
	return ret;
}


int16_t mma8451_getAcX(void){
	return (int16_t)(((int32_t)read.X * 100) / (int32_t)4096); // Para 14 bits con 2g (por defecto). Divido por la mitad del maximo para que esté entre -2g y 2g. Luego la multiplicacion por 100 es para tener la representacion de g en centésimas.
}
int16_t mma8451_getAcY(void){
	return (int16_t)(((int32_t)read.Y * 100) / (int32_t)4096); // Para 14 bits con 2g (por defecto). Divido por la mitad del maximo para que esté entre -2g y 2g. Luego la multiplicacion por 100 es para tener la representacion de g en centésimas.
}
int16_t mma8451_getAcZ(void){
	return (int16_t)(((int32_t)read.Z * 100) / (int32_t)4096); // Para 14 bits con 2g (por defecto). Divido por la mitad del maximo para que esté entre -2g y 2g. Luego la multiplicacion por 100 es para tener la representacion de g en centésimas.
}

/* Brief: Leer los 6 registros de las 3 aceleraciones y cargarlas en un buffer */
void readAccsFromRegisters(void){
	uint8_t bufAcc[6]; // Se puede hacer un buffer de 7 y tambien guardar el Status
	int16_t readGravity;
	mma8451_read_multiple_reg(OUT_X_MSB_ADDRESS, bufAcc, sizeof(bufAcc));
	readGravity	=  (int16_t)bufAcc[0] << 8;
	readGravity	|= bufAcc[1];
	read.X = readGravity >> 2;

	readGravity   = (int16_t)bufAcc[2] << 8;
	readGravity  |= bufAcc[3];
	read.Y = readGravity >> 2;

	readGravity   = (int16_t)bufAcc[4] << 8;
	readGravity  |= bufAcc[5];
	read.Z = readGravity >> 2;
}

/* --------- IRQ HANDLER ------------------ */
void PORTC_PORTD_IRQHandler(void){
    INT_SOURCE_t intSource;
    STATUS_t status;
    intSource.data = mma8451_read_reg(INT_SOURCE_ADDRESS);	/* The bits are set by a low to high transition
    															and are cleared by reading the
																appropriate interrupt source register */

    /* Chequeamos de donde viene la interrupción */
    if (intSource.SRC_DRDY){
    	status.data = mma8451_read_reg(STATUS_ADDRESS);
    	 if (status.ZYXDR){
    		 readAccsFromRegisters();
    		 isInterruptionByDR = true;		// Set in false by reading
    	 }
    } else if (intSource.SRC_FF_MT) {
    	isInterruptionByFreeFall = true;	// Set in false by reading
    	APP_PowerModeSwitch(kSMC_PowerStateVlpr, kAPP_PowerModeRun);
    }

    mma8451_read_reg(FF_MT_SRC_ADDRESS);
    mma8451_read_reg(INT_SOURCE_ADDRESS);

    PORT_ClearPinsInterruptFlags(INT1_PORT, 1<<INT1_PIN);
}

/*==================[end of file]============================================*/





uint16_t getResolutionInCountsXG(void){
	XYZ_DATA_CFG_t data_cfg;
	CTRL_REG1_t ctrl_reg1;
	uint16_t countsxg = 0;
	data_cfg.data = mma8451_read_reg(XYZ_DATA_CFG_ADDRESS);
	ctrl_reg1.data = mma8451_read_reg(CTRL_REG1_ADDRESS);
	bool modoConv = ctrl_reg1.F_READ; // 1: 8 bits ; 0:14 bits ?
	switch(data_cfg.FormatScale){
		case FS_2G:
			if(modoConv) countsxg = 64; // 64 counts/g --> 15.6 mg/LSB
			else countsxg = 4096;		// 4096 count/g --> 0.25 mg/LSB
		break;
		case FS_4G:
			if(modoConv) countsxg = 32; // 32 counts/g --> 31.25 mg/LSB
			else countsxg = 2048;		// 2048 count/g --> 0.5 mg/LSB
		break;
		case FS_8G:
			if(modoConv) countsxg = 16; // 16 counts/g --> 62.5 mg/LSB
			else countsxg = 1024;		// 4096 count/g --> 1 mg/LSB
		break;
	}
	return countsxg;
}

void setThreshold(int mg_threshold){
	FF_MT_THS_t FF_MT_THS_reg;
	uint16_t resolutionxg = getResolutionInCountsXG();
	int counts_in_threshold = (int) (mg_threshold + resolutionxg / 2) / resolutionxg;
	FF_MT_THS_reg.data = counts_in_threshold;
	mma8451_write_reg(FF_MT_THS_ADDRESS,FF_MT_THS_reg.data);
}

void setDebounceCounter(int debounceCountermS){
	CTRL_REG1_t ctrl_reg1;
	FF_MT_COUNT_t FF_MT_COUNT_reg;
	int debounceCount = 0;
	ctrl_reg1.DR = mma8451_read_reg(FF_MT_COUNT_ADRESS);
	switch(ctrl_reg1.DR){
		case DR_800hz:
			debounceCount = debounceCountermS/1.25;
		break;
		case DR_400hz:
			debounceCount = debounceCountermS/2.5;
		break;
		case DR_200hz:
			debounceCount = debounceCountermS/5;
		break;
		case DR_100hz:
			debounceCount = debounceCountermS/10;
		break;
		case DR_50hz:
			debounceCount = debounceCountermS/20;
		break;
		case DR_12p5hz:
			debounceCount = debounceCountermS/80;
		break;
		case DR_6p25hz:
			debounceCount = debounceCountermS/160;
		break;
		case DR_1p56hz:
			debounceCount = debounceCountermS/640;
		break;
		FF_MT_COUNT_reg.data = debounceCount;
		mma8451_write_reg(FF_MT_COUNT_ADRESS,FF_MT_COUNT_reg.data);
	}

}








void mma8451_init(void)
{
	// Se definen variables del tipo de los registros a modificar.
    CTRL_REG1_t ctrl_reg1;
    CTRL_REG4_t ctrl_reg4;
    CTRL_REG5_t ctrl_reg5;

    // Se escribe en la variable que simula el registro 4.
	ctrl_reg4.INT_EN_DRDY = 1;
	ctrl_reg4.INT_EN_FF_MT = 0; /// Habilitar pedido de interrupción cuando se da Free Fall (poner en 1) - Antes hay que deshabilitar la interrupción de GPIO
	ctrl_reg4.INT_EN_PULSE = 0;
	ctrl_reg4.INT_EN_LNDPRT = 0;
	ctrl_reg4.INT_EN_TRANS = 0;
	ctrl_reg4.INT_EN_FIFO = 0;
	ctrl_reg4.INT_EN_ASLP = 0;

	// Escribe la estructura anterior en el registro
	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);
	/// ej: mma8451_write_reg(0X2D, 0x04) -->  Freefall/motion interrupt enabled

	/* verificación para fines didacticos (en debugging) */
	ctrl_reg4.data = mma8451_read_reg(CTRL_REG4_ADDRESS);

	// Se escribe en la estructura del registro 5
	ctrl_reg5.INT_CFG_DRDY = 1;
	ctrl_reg5.INT_CFG_FF_MT = 0;
	ctrl_reg5.INT_CFG_PULSE = 0;
	ctrl_reg5.INT_CFG_LNDPRT = 0;
	ctrl_reg5.INT_CFG_TRANS = 0;
	ctrl_reg5.INT_CFG_FIFO = 0;
	ctrl_reg5.INT_CFG_ASLP = 0;

	mma8451_write_reg(CTRL_REG5_ADDRESS, ctrl_reg5.data);


	ctrl_reg1.ACTIVE = 1;
	ctrl_reg1.F_READ = 0;
	ctrl_reg1.LNOISE = 1;
	ctrl_reg1.DR = 0B101;		// Ver que valor ponerle para saber desde que medida toma la caida libre. Habria que saber las cuentas y el tiempo por conversión.
	ctrl_reg1.ASLP_RATE = 0B00;

    mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);


    config_port_int1();
}
