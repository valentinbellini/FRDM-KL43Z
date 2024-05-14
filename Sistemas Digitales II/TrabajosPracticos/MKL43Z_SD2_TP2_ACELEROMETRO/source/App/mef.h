/**
 * @file    mef.h
 * @brief   Application work flow
 * @autor	Valentin Bellini & Iván Saitta
 */

/*  =========== [IN USE] ===========
    This application is using the following PORT/PINS:

    PORTA   4   SW1

    PORTC   3   SW3
    PORTC   0   RST - OLED
    PORTC   7   DATA/CMD - OLED
    PORTC   5   INT1 - MMA8451

    PORTD   4   SPI1_SS
    PORTD   5   SPI1_SCK
    PORTD   6   SPI1_MOSI

    PORTE   24  I2C0_SCL
    PORTE   25  I2C0_SDA
    PORTE   32  LED_ROJO
    PORTD   5   LED_VERDE
*/

#ifndef MEF_H
#define MEF_H

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void mef_init(void);
extern void mef_Tr_To_Bajo_Consumo(void);
extern void mef(void);
extern void mef_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif
