/**
 * @file    main.c
 * @brief   Application entry point.
 * @autor	Valentin Bellini & Iván Saitta
 */

/*  =========== [PORT MAPPING] ====================================== *//*
    This application is using the following PORT/PINS:

    PORTA   4   BOARD_SW1

    PORTC   0   RST - OLED
    PORTC   3   BOARD_SW3
    PORTC   4   SPI1_SS
    PORTC   5   SPI1_SCK
    PORTC   6   SPI1_MOSI
    PORTC   7   DATA/CMD - OLED

    PORTD   1   INT2 - MMA8451
    PORTD   5   BOARD_LED_VERDE

	PORTE	0	UART1_TX
	PORTE	1	UART1_RX
    PORTE   24  I2C0_SCL
    PORTE   25  I2C0_SDA
    PORTE	29	UART1_ControlLine_RE
    PORTE	30	UART1_ControlLine_DE
    PORTE   31  BOARD_LED_ROJO

*/

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "fsl_smc.h"
#include "fsl_port.h"

#include "Drivers/PowerMode/power_mode_switch.h"
#include "Drivers/Board/SD2_board.h"
#include "Drivers/Key/key.h"
#include "Drivers/I2C/SD2_I2C.h"
#include "Drivers/SSD1306/oled.h"
#include "Drivers/MMA8451/mma8451.h"
#include <Drivers/Transceiver/transceiver_RS485_UART.h>
#include <App/mef_principal.h>

/*==================[ internal functions definition ]==========================*/
void init_clocks_and_power_mode(){
	uint32_t freq = 0;
	smc_power_state_t currentPowerState;

	DEBUG_PRINT("\r\n####################  TRABAJO PRACTICO 3 - SD2 - BELLINI Y SAITTA ####################\n\r");

	currentPowerState = SMC_GetPowerModeState(SMC);
	APP_ShowPowerMode(currentPowerState);
	DEBUG_PRINT("\n    Clock luego del Reset\n");
	freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
	DEBUG_PRINT("    Core Clock = %dMHz \r\r", freq/1000000);


	DEBUG_PRINT("    Configurando Clocks a máxima frecuencia...\n");
	/* Se configuran los clocks a máxima frecuencia */
    BOARD_InitBootClocks();
    /* Se habilita la posibilidad de operar con todos los modos de bajo consumo */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    DEBUG_PRINT("    Core Clock = %dMHz \r", freq/1000000);

    DEBUG_PRINT("\r\n#################### ========================= ####################\n\r\n");
}

/*==================[ MAIN ]==========================*/
int main(void){

	/* Inicialización de clocks a máxima frecuencia y micro en modo RUN a 48MHz */
	init_clocks_and_power_mode();

    /* Inicialización FSL debug console. */
    BOARD_InitDebugConsole();

    /* Inicialización de GPIOS (LED, SW, OLED, RS485) */
    board_init();

    /* Inicialización de SPI y display OLED */
    board_configSPI0();
    oled_init();
    oled_setContrast(16);

    /* Inicialización del I2C: release bus - config pins - master init*/
    SD2_I2C_init();

    /* Inicialización MEF de switches*/
    key_init();

    /* Se inicializa UART1, DMA y RingBufferRx */
    transceiver_init();

    /* Se configura interrupción de systick cada 1 ms */
    SysTick_Config(SystemCoreClock / 1000U);

    /* INIT de la APP */
    mef_principal_init();

    while(1) {
    	mef_principal();
    }

    return 0;
}

void SysTick_Handler(void){
    key_periodicTask1ms();
    mef_principal_task1ms();
}

