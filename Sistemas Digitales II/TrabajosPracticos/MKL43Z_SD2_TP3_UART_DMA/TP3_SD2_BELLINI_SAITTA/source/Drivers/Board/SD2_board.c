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
#include "SD2_board.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "fsl_spi.h"

/*==================[macros and definitions]=================================*/
/*------------------ SPI CONFIGS --------------------------------------------*/
#define SPI0_MASTER              	SPI0
#define SPI0_MASTER_SOURCE_CLOCK 	kCLOCK_BusClk
#define SPI0_MASTER_CLK_FREQ     	CLOCK_GetFreq(kCLOCK_BusClk)

#define SPI0_SS_PORT				PORTC
#define SPI0_SS_PIN					4
#define SPI0_SCK_PORT				PORTC
#define SPI0_SCK_PIN				5
#define SPI0_MOSI_PORT				PORTC
#define SPI0_MOSI_PIN				6

/*==================[internal data declaration]==============================*/
static const board_gpioInfo_type board_gpioLeds[] =
{
    {PORTE, GPIOE, 31},     /* LED ROJO */
    {PORTD, GPIOD, 5},      /* LED VERDE */
};
static const board_gpioInfo_type board_gpioSw[] =
{
    {PORTA, GPIOA, 4},      /* SW1 */
    {PORTC, GPIOC, 3},      /* SW3 */
};
static const board_gpioInfo_type board_gpioOled[] =
{
    {PORTC, GPIOC, 0},      /* RST */
    {PORTC, GPIOC, 7},      /* DATA/CMD */

};


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void board_gpio_led_init(void){
	int32_t i;
	gpio_pin_config_t gpio_led_config =
	{
		.outputLogic = 1,
		.pinDirection = kGPIO_DigitalOutput,
	};
	const port_pin_config_t port_led_config = {
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
	/* inicialización de leds */
	for (i = 0 ; i < BOARD_LED_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioLeds[i].port, board_gpioLeds[i].pin, &port_led_config);
		GPIO_PinInit(board_gpioLeds[i].gpio, board_gpioLeds[i].pin, &gpio_led_config);
	}
}
static void board_gpio_switch_init(void){
	int32_t i;
	gpio_pin_config_t gpio_sw_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 0U
	};
	const port_pin_config_t port_sw_config = {
		/* Internal pull-up resistor is enabled */
		.pullSelect = kPORT_PullUp,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as PTC3 */
		.mux = kPORT_MuxAsGpio,
	};

	/* Desactivación de NMI en SW1 */
	PORTA->PCR[4] &= ~(7 << 8);

	/* inicialización de SWs */
	for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioSw[i].port, board_gpioSw[i].pin, &port_sw_config);
		GPIO_PinInit(board_gpioSw[i].gpio, board_gpioSw[i].pin, &gpio_sw_config);
	}
}
static void board_gpio_oled_init(void){
	int32_t i;
	gpio_pin_config_t gpio_oled_config =
	{
		.outputLogic = 0,
		.pinDirection = kGPIO_DigitalOutput,
	};
	const port_pin_config_t port_oled_config = {
		/* Internal pull-up/down resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as GPIO */
		.mux = kPORT_MuxAsGpio,
	};
	/*Inicialización de los pines GPIO necesarios para manejar el display OLED*/
	for (i = 0 ; i < OLED_TOTAL ; i++){
		PORT_SetPinConfig(board_gpioOled[i].port, board_gpioOled[i].pin, &port_oled_config);
		GPIO_PinInit(board_gpioOled[i].gpio, board_gpioOled[i].pin, &gpio_oled_config);
	}
}
/*==================[external functions definition]==========================*/
void board_init(void)
{
	/* Clocks enables */
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	board_gpio_led_init();		/* Board LEDS init */
	board_gpio_switch_init();	/* Board Switches init */
	board_gpio_oled_init();		/* external OLED SSD1306 GPIO init */

}

void board_setLed(board_ledId_enum id, board_ledMsg_enum msg)
{
    switch (msg)
    {
        case BOARD_LED_MSG_OFF:
        	GPIO_PortSet(board_gpioLeds[id].gpio, 1<<board_gpioLeds[id].pin);
            break;

        case BOARD_LED_MSG_ON:
        	GPIO_PortClear(board_gpioLeds[id].gpio, 1<<board_gpioLeds[id].pin);
            break;

        case BOARD_LED_MSG_TOGGLE:
        	GPIO_PortToggle(board_gpioLeds[id].gpio, 1<<board_gpioLeds[id].pin);
            break;

        default:
            break;
    }
}

//void board_setRS485_controlLine(board_RS485_ControlLines_enum id, bool est){
//	if (est)
//		GPIO_PortSet(board_gpioUART_ControlLine[id].gpio, 1<<board_gpioUART_ControlLine[id].pin);
//	else
//		GPIO_PortClear(board_gpioUART_ControlLine[id].gpio, 1<<board_gpioUART_ControlLine[id].pin);
//}

bool board_getSw(board_swId_enum id)
{
    return !GPIO_PinRead(board_gpioSw[id].gpio, board_gpioSw[id].pin);
}

bool board_getLed(board_ledId_enum id)
{
    return !GPIO_PinRead(board_gpioLeds[id].gpio, board_gpioLeds[id].pin);
}

void board_setOledPin(board_oledPin_enum oledPin, uint8_t state)
{
	GPIO_PinWrite(board_gpioOled[oledPin].gpio, board_gpioOled[oledPin].pin, state);
}

void board_configSPI0(){
	const port_pin_config_t port_spi_config = {
		/* Internal pull-up resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as SPI0_x */
		.mux = kPORT_MuxAlt2,
	};
	PORT_SetPinConfig(SPI0_SS_PORT, SPI0_SS_PIN, &port_spi_config); 	//SPI0_SS	 C4
	PORT_SetPinConfig(SPI0_SCK_PORT, SPI0_SCK_PIN, &port_spi_config); 	//SPI0_SCK	 C5
	PORT_SetPinConfig(SPI0_MOSI_PORT, SPI0_MOSI_PIN, &port_spi_config);	//SPI0_MOSI	 C6

	CLOCK_EnableClock(kCLOCK_Spi1);

	spi_master_config_t userConfig;

	SPI_MasterGetDefaultConfig(&userConfig);

	/*
	userConfig.enableMaster         	= true;
	userConfig.enableStopInWaitMode 	= false;
	userConfig.polarity             	= kSPI_ClockPolarityActiveHigh;
	userConfig.phase                	= kSPI_ClockPhaseFirstEdge;
	userConfig.direction            	= kSPI_MsbFirst;
	userConfig.dataMode 			 	= kSPI_8BitMode;
	userConfig.txWatermark 		 		= kSPI_TxFifoOneHalfEmpty;
	userConfig.rxWatermark 		 		= kSPI_RxFifoOneHalfFull;
	userConfig.pinMode      		 	= kSPI_PinModeNormal;
	userConfig.outputMode   			= kSPI_SlaveSelectAutomaticOutput;
	userConfig.baudRate_Bps 		 	= 500000U;
	*/

	userConfig.polarity             = kSPI_ClockPolarityActiveLow;
	userConfig.phase                = kSPI_ClockPhaseSecondEdge;
	userConfig.baudRate_Bps 		= 4000000U;

	SPI_MasterInit(SPI0_MASTER, &userConfig, SPI0_MASTER_CLK_FREQ);
}

void board_SPI0Send(uint8_t* buf, size_t len){
	spi_transfer_t xfer;

	xfer.txData = buf;
	xfer.rxData = NULL;
	xfer.dataSize  = len;

	SPI_MasterTransferBlocking(SPI0_MASTER, &xfer);
}


/* ----------------------------------------------------------------------------------------------------- */



/*==================[end of file]============================================*/
