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
#include <SD2_board.h>
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "pin_mux.h"

/*==================[macros and definitions]=================================*/

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

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void board_init(void)
{
	int32_t i;
	gpio_pin_config_t gpio_led_config =
	{
		.outputLogic = 1,
		.pinDirection = kGPIO_DigitalOutput,
	};
	gpio_pin_config_t gpio_sw_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 0U
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

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	/* inicialización de leds */
	for (i = 0 ; i < BOARD_LED_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioLeds[i].port, board_gpioLeds[i].pin, &port_led_config);
		GPIO_PinInit(board_gpioLeds[i].gpio, board_gpioLeds[i].pin, &gpio_led_config);
	}

	PORTA->PCR[4] &= ~(7 << 8); //Desactivo NMI en SW1

	/* inicialización de SWs */
	for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioSw[i].port, board_gpioSw[i].pin, &port_sw_config);
		GPIO_PinInit(board_gpioSw[i].gpio, board_gpioSw[i].pin, &gpio_sw_config);
	}
}

void adc_init(void){

	SIM->SCGC6 |= 1<<27; //Le doy clock al ADC

	ADC0->CFG1 |= (0b10)<<2; //Modo 10-bit
	//ADC0->SC3 |= 1<<2; //Promedio por hardware (4 muestras por defecto)
	//ADC0->SC3 |= (0b01); //Promedio 8 muestras
	//ADC0->SCE |= 1<<3;  //Conversión continua

	PORTE->PCR[22] |= PORT_PCR_MUX(0); //Pin 22E como entrada analógica (sensor de luz)
	NVIC_EnableIRQ(ADC0_IRQn); //Habilito la interrupción en NVIC

	ADC0->SC1[0] = 0b1000011; //Habilito interrupción, configuro lectura en canal 3 (sensor de luz)

}

void redPWM_init(void) {

#ifndef PWM_INIT
#define PWM_INIT

	CLOCK_EnableClock(kCLOCK_Tpm0); //Le doy clock al TPM0
	CLOCK_SetTpmClock(3); //Fuente de clock: MCGIRCLK

	TPM0->SC |= 0b111; //Prescaler por 128
	TPM0->SC |= 1 << 3; //Habilito el contador

	// Quiero un PWM de frecuencia 1KHz, por lo que necesito que el contador llegue a MOD 1000 veces por segundo
	// Por eso MOD debe ser CLOCK_GetFreq(kCLOCK_McgInternalRefClk)/128/1000
	TPM0->MOD = CLOCK_GetFreq(kCLOCK_McgInternalRefClk) / 128 / 1000;

#endif

	TPM0->CONTROLS[4].CnSC |= (0b1011) << 2; // Configuro el canal 4 (LED Rojo) como PWM Edge-Aligned Low-true pulses

	TPM0->CONTROLS[4].CnV = 0; //Arrancamos en duty 0%

	TPM0->CNT = 0; //Reseteo la cuenta

	PORTE->PCR[31] &= ~PORT_PCR_MUX(15); //Limpio
	PORTE->PCR[31] |= PORT_PCR_MUX(3); //PTE31 (LED Rojo) configurado para TPM0_CH4

}

void greenPWM_init(void) {

#ifndef PWM_INIT
#define PWM_INIT

	CLOCK_EnableClock(kCLOCK_Tpm0); //Le doy clock al TPM0
	CLOCK_SetTpmClock(3); //Fuente de clock: MCGIRCLK

	TPM0->SC |= 0b111; //Prescaler por 128
	TPM0->SC |= 1<<3; //Habilito el contador

	// Quiero un PWM de frecuencia 1KHz, por lo que necesito que el contador llegue a MOD 1000 veces por segundo
	// Por eso MOD debe ser CLOCK_GetFreq(kCLOCK_McgInternalRefClk)/128/1000
	TPM0->MOD = CLOCK_GetFreq(kCLOCK_McgInternalRefClk)/128/1000;

#endif

	TPM0->CONTROLS[5].CnSC |= (0b1011) << 2; // Configuro el canal 5 (LED Verde) como PWM Edge-Aligned Low-true pulses

	TPM0->CONTROLS[5].CnV = 0; //Arrancamos en duty 0%

	TPM0->CNT = 0; //Reseteo la cuenta

	PORTD->PCR[5] &= ~PORT_PCR_MUX(15); //Limpio
	PORTD->PCR[5] |= PORT_PCR_MUX(4); //PTD5 (LED Verde) configurado para TPM0_CH5

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

bool board_getSw(board_swId_enum id)
{
    return !GPIO_PinRead(board_gpioSw[id].gpio, board_gpioSw[id].pin);
}

bool board_getLed(board_ledId_enum id)
{
    return !GPIO_PinRead(board_gpioLeds[id].gpio, board_gpioLeds[id].pin);
}
/* ----------------------------------------------------------------------------------------------------- */
static void config_TPM_PWM(void)
{
	tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;

    int i;

    /* Configure tpm params with frequency 24kHZ */
	tpmParam.level = kTPM_LowTrue;
	tpmParam.dutyCyclePercent = 0;

	/* Select the clock source for the TPM counter as kCLOCK_McgIrc48MClk */
	CLOCK_SetTpmClock(1U);

	TPM_GetDefaultConfig(&tpmInfo);
	/* Initialize TPM module */
	TPM_Init(TPM0, &tpmInfo);

	for (i = 0 ; i < BOARD_LED_ID_TOTAL ; i++)
	{
		tpmParam.chnlNumber = tpm_chnl_leds[i];
		TPM_SetupPwm(TPM0, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, CLOCK_GetFreq(kCLOCK_McgIrc48MClk));

		PORT_SetPinMux(board_gpioLeds[i].port, board_gpioLeds[i].pin, pinMuxLeds[i]);

		dutyCiclePWM[i] = 100;
		ledStatus[i] = false;
		TPM_UpdatePwmDutycycle(TPM0, tpm_chnl_leds[i], kTPM_CenterAlignedPwm, 0);
	}

	TPM_StartTimer(TPM0, kTPM_SystemClock);
}

void board_setLedBrightness(board_ledId_enum id, uint8_t brightness)
{
	if (brightness > 100)
		brightness = 100;

	dutyCiclePWM[id] = brightness;
	if (ledStatus[id])
		TPM_UpdatePwmDutycycle(TPM0, tpm_chnl_leds[id], kTPM_CenterAlignedPwm, dutyCiclePWM[id]);
}

bool board_getSw(board_swId_enum id)
{
    return !GPIO_PinRead(board_gpioSw[id].gpio, board_gpioSw[id].pin);
}
/*==================[end of file]============================================*/
