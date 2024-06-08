/*
 * procTrama.c
 *
 *  Created on: 30 may. 2023
 *      Author: Usuario
 */
/*==================[inclusions]=============================================*/
#include "App/trama_process.h"
#include "Drivers/Board/SD2_board.h"
#include "Drivers/SSD1306/oled.h"
#include "Drivers/MMA8451/mma8451.h"
#include "Drivers/uart_ringBuffer.h"
#include <stdio.h>
#include "App/mef_trama_rec.h"

#include "fsl_debug_console.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void accionLed(uint8_t charIdLed, board_ledMsg_enum ledMsg)
{
	switch (charIdLed)
	{
		case '1':
			board_setLed(BOARD_LED_ID_ROJO, ledMsg);
			break;

		case '2':
			board_setLed(BOARD_LED_ID_VERDE, ledMsg);
			break;
	}
}
static bool accionSwitch(uint8_t charIdSwitch)
{
	switch (charIdSwitch)
	{
		case '1':
			return board_getSw(BOARD_SW_ID_1);
			break;

		case '3':
			return board_getSw(BOARD_SW_ID_3);
			break;
	}
	return false;
}

/*==================[external functions definition]==========================*/

void tramaProcess(char *buf, int length)
{
	PRINTF("Input buffer: %s\n", buf);
	uint8_t buffer[32];
	bool pulsador_presionado;
	buffer[0]='\0';
	switch (buf[0])
	{

	case '0'://Caso de los leds
		switch (buf[2])
		{
			case 'A':
				accionLed(buf[1], BOARD_LED_MSG_OFF);
				break;
			case 'E':
				accionLed(buf[1], BOARD_LED_MSG_ON);
				break;
			case 'T':
				accionLed(buf[1], BOARD_LED_MSG_TOGGLE);
				break;
		}
		//Se da formato al string a transmitir y se almacena en el arrelgo de char buffer

		sprintf((char*)buffer, ":%c%c0%c%c\n",NUM_GRUPO_A, NUM_GRUPO_B,buf[1], buf[2]);
		break;

    case '1'://Caso de los switchs

    	pulsador_presionado = accionSwitch(buf[1]);
    	if (pulsador_presionado)
    	{
            //Se da formato al string a transmitir y se almacena en el arrelgo de char buffer
            sprintf((char*)buffer, ":%c%c1%cP\n",NUM_GRUPO_A, NUM_GRUPO_B, buf[1]);
    	}
    	else
    	{
            //Se da formato al string a transmitir y se almacena en el arrelgo de char buffer
            sprintf((char*)buffer, ":%c%c1%cN\n", NUM_GRUPO_A, NUM_GRUPO_B,buf[1]);
    	}
        break;

	case '2'://Caso de los acelerometro

		if (buf[1]=='1')
		{
			/* Se configura interrupciones por DataReady y se espera que esté lista la conversión en No-Operation */
			mma8451_dataReady_config();
			while(!mma8451_getDataReadyInterruptStatus()){
				//__NOP();
			}
			int16_t x = mma8451_getAcX();
			int16_t y = mma8451_getAcY();
			int16_t z = mma8451_getAcZ();
			snprintf((char*)buffer, sizeof(buffer), ":%c%c21%+04d%+04d%+04d\n", NUM_GRUPO_A, NUM_GRUPO_B, x, y, z);
			mma8451_clearInterruptions_config();
		}
		break;
	default:
		break;
	}
	PRINTF("Output buffer: %s\n", buffer);
	//uart_ringBuffer_envDatos(buffer, sizeof(buffer));
	uart0_drv_envDatos(buffer, strlen((char*)buffer));
	//Envia datos por UART0 mediante DMA
	//transceptor_envDatosDMA(buffer, strlen((char*)buffer));
}
/*==================[end of file]============================================*/

















///*
// * procTrama.c
// *
// *  Created on: 06/06/2024
// *  Author: Valen
// */
///*==================[inclusions]=============================================*/
//#include <Drivers/Transceiver/transceiver_uart_rs485.h>
//#include <stdio.h>
//#include <stdbool.h>
//
//#include "App/trama_process.h"
//#include "Drivers/Board/SD2_board.h"
//#include "Drivers/MMA8451/mma8451.h"
//#include "App/mef_trama_rec.h"
//
///*==================[macros and definitions]=================================*/
//#define _ERROR_LOG_FILE 		"error.log"
//
///*==================[internal data declaration]==============================*/
//static bool wrongTrama = false;
//
///*==================[internal functions definition]==========================*/
//static board_ledMsg_enum convertToBoardLedMsg(char c) {
//    switch (c) {
//        case 'A':
//            return BOARD_LED_MSG_OFF;
//        case 'E':
//            return BOARD_LED_MSG_ON;
//        case 'T':
//            return BOARD_LED_MSG_TOGGLE;
//        default:
//            // En caso de que se pase un carácter no válido, regresamos un valor por defecto
//            return BOARD_LED_MSG_OFF;
//    }
//}
//void setErrorAndLog(const char *errorMessage) {
//    wrongTrama = true;
//    PRINTF("Error: Trama incorrecta\n");
//    //logError(errorMessage);
//}
//static void handleLedAction(char IdLed, char ledMsgChar){
//	board_ledMsg_enum ledMsg = convertToBoardLedMsg(ledMsgChar);
//
//	switch (IdLed){
//		case '1':
//			board_setLed(BOARD_LED_ID_ROJO, ledMsg);
//			break;
//		case '2':
//			board_setLed(BOARD_LED_ID_VERDE, ledMsg);
//			break;
//		default:
//			setErrorAndLog("Trama Incorrecta. Led Invalido");
//			break;
//	}
//}
//static bool isSwitchPressed(char IdSwitch)
//{
//	switch (IdSwitch){
//		case '1':
//			return board_getSw(BOARD_SW_ID_1);
//		case '3':
//			return board_getSw(BOARD_SW_ID_3);
//		default:
//			setErrorAndLog("Trama incorrecta. Switch Invalido.");
//	}
//	return false;
//}
//void logError(const char *errorMessage) {
//    FILE *errorLogFile = fopen(_ERROR_LOG_FILE, "a"); /* apertura en modo apéndice */
//
//    if (errorLogFile != NULL) {
//        fprintf(errorLogFile, "%s\n", errorMessage);
//        fclose(errorLogFile);
//    } else {
//        // Intenta crear el archivo de registro de errores
//        errorLogFile = fopen(_ERROR_LOG_FILE, "w"); /* apertura en modo escritura */
//        if (errorLogFile != NULL) {
//            fprintf(errorLogFile, "%s\n", errorMessage);
//            fclose(errorLogFile);
//        } else {
//            PRINTF("No se pudo abrir ni crear el archivo de registro de errores\n");
//        }
//    }
//}
//
///*==================[external functions definition]==========================*/
//void tramaProcess(char *buf, int length) {
//    bool swPressed;
//    uint8_t buffer[BUFFER_SIZE];
//    buffer[0] = '\0';
//
//    switch (buf[0]) {
//        case '0': // Caso de los leds
//            switch (buf[2]) {
//                case 'A':
//                case 'E':
//                case 'T':
//                    handleLedAction(buf[1],buf[2]);
//                    /* formatea una cadena de caracteres y la coloca en el búfer teniendo en cuenta desbordamiento. */
//                    snprintf((char*)buffer, sizeof(buffer), ":%c%c0%c%c\n", NUM_GRUPO_A, NUM_GRUPO_B, buf[1], buf[2]);
//                    break;
//                default:
//                    setErrorAndLog("Trama incorrecta. Control de LED invalido.");
//                    break;
//            }
//            break;
//
//        case '1': // Caso de los switchs
//            swPressed = isSwitchPressed(buf[1]);
//            snprintf((char*)buffer, sizeof(buffer), ":%c%c1%c%c\n", NUM_GRUPO_A, NUM_GRUPO_B, buf[1], (swPressed ? 'P' : 'N'));
//            break;
//
//        case '2': // Caso de los acelerómetro
//            if (buf[1] == '1') {
//            	/* Se configura interrupciones por DataReady y se espera que esté lista la conversión en No-Operation */
//                mma8451_dataReady_config();
//                while(!isInterruptionByDR){
//                	__NOP();
//                }
//                int16_t x = mma8451_getAcX();
//                int16_t y = mma8451_getAcY();
//                int16_t z = mma8451_getAcZ();
//                snprintf((char*)buffer, sizeof(buffer), ":%c%c21%+04d%+04d%+04d\n", NUM_GRUPO_A, NUM_GRUPO_B, x, y, z);
//            } else {
//                setErrorAndLog("Trama incorrecta para lectura de acelerómetro");
//            }
//            break;
//
//        default:
//            setErrorAndLog("Trama Incorrecta. buf[0] no encuentra coincidencias.");
//            break;
//    }
//
//    if (wrongTrama) {
//    	/* Si la trama no cumple con lo esperado, devuelve todas F */
//        memset(buffer, 'F', sizeof(buffer));
//    }
//
//    // Envía datos por UART0 mediante DMA
//    transceiver_envDatosDMA(buffer, strlen((char*)buffer));
//}
///*==================[end of file]============================================*/
//
