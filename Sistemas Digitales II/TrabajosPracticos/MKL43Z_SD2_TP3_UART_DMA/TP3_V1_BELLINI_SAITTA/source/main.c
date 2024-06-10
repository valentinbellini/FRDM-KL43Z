/**
 * @file    main.c
 * @brief   Application entry point.
 * @autor	Valentin Bellini & Iván Saitta
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

/*==================[internal functions definition]==========================*/
void init_clocks_and_power_mode(){
	uint32_t freq = 0;
	smc_power_state_t currentPowerState;

	PRINTF("\r\n####################  TRABAJO PRACTICO 3 - SD2 - BELLINI Y SAITTA ####################\n\r");

	currentPowerState = SMC_GetPowerModeState(SMC);
	APP_ShowPowerMode(currentPowerState);
	PRINTF("\n    Clock luego del Reset\n");
	freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
	PRINTF("    Core Clock = %dMHz \r\r", freq/1000000);


	PRINTF("    Configurando Clocks a máxima frecuencia...\n");
	/* Se configuran los clocks a máxima frecuencia */
    BOARD_InitBootClocks();
    /* Se habilita la posibilidad de operar con todos los modos de bajo consumo */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    PRINTF("    Core Clock = %dMHz \r", freq/1000000);

    PRINTF("\r\n#################### ========================= ####################\n\r\n");
}


int main(void) {

	/* Inicialización de clocks a máxima frecuencia y micro en modo RUN a 48MHz */
	init_clocks_and_power_mode();

    /* Inicialización FSL debug console. */
    BOARD_InitDebugConsole();

    /* Inicialización de GPIOS (LED, SW, OLED) */
    board_init();

    /* Inicialización de SPI y display OLED */
    board_configSPI0();
    oled_init();
    oled_setContrast(16);
	oled_clearScreen(OLED_COLOR_BLACK);

    /* Inicialización del I2C */
    SD2_I2C_init();

    /* Inicialización MEF de pulsadores*/
    key_init();

    /* Se inicializa UART1 y DMA */
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

