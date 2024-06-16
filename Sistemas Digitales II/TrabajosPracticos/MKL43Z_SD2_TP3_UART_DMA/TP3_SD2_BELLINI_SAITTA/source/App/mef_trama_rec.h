/**
 * @file    mef_trama_rec.h
 * @brief   MEF para recepción de trama.
 * @autor	Valentin Bellini & Iván Saitta
 */

#ifndef MEFRECTRAMA_H_
#define MEFRECTRAMA_H_

/* Numeros de grupo para recepción de mensaje */
#define NUM_GRUPO_A		'0'
#define NUM_GRUPO_B		'3'

/* MEF para preprocesamiento de la trama */
void mefRecTrama_task(void);

#endif /* MEFRECTRAMA_H_ */
