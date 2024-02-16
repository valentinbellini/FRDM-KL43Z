/*MACROS PARA EL KIT KL43Z*/


#ifndef MACROSKL43Z_H_
#define MACROSKL43Z_H_

#include "MKL43Z4.h"


//LED ROJO
#define LED_ROJO_GPIO 		PTE
#define LED_ROJO_PIN 		31	//Pin del LED ROJO
#define LED_ROJO_PORT 		PORTE  //Puerto del LED ROJO
#define LED_ROJO_ON 		PTE->PCOR 							|= 1<<LED_ROJO_PIN
#define LED_ROJO_OFF 		PTE->PSOR 							|= 1<<LED_ROJO_PIN
#define LED_ROJO_TOGGLE 	PTE->PTOR 							|= 1<<LED_ROJO_PIN
#define LED_ROJO_CLOCK 		SIM->SCGC5          				|= SIM_SCGC5_PORTE_MASK
#define LED_ROJO_MODE 		LED_ROJO_PORT->PCR[LED_ROJO_PIN] 	|= PORT_PCR_MUX(1)
#define LED_ROJO_OUTPUT		PTE->PDDR      						|= 1<<LED_ROJO_PIN

//LED VERDE
#define LED_VERDE_GPIO 		PTD
#define LED_VERDE_PIN 		5   //Pin del LED VERDE
#define LED_VERDE_PORT 		PORTD   //Puerto del LED VERDE
#define LED_VERDE_ON 		PTD->PCOR 							|= 1<<LED_VERDE_PIN     //PTD->PCOR |= 1<<5
#define LED_VERDE_OFF 		PTD->PSOR 							|= 1<<LED_VERDE_PIN 
#define LED_VERDE_TOGGLE	PTD->PTOR 							|= 1<<LED_VERDE_PIN
#define LED_VERDE_CLOCK 	SIM->SCGC5 		 					|= SIM_SCGC5_PORTD_MASK
#define LED_VERDE_MODE 		LED_VERDE_PORT->PCR[LED_VERDE_PIN] 	|= PORT_PCR_MUX(1)
#define LED_VERDE_OUTPUT 	PTD->PDDR 	 						|= 1<<LED_VERDE_PIN     //PTD->PDDR |= 1<<5

//SW1
#define SW1_GPIO 	PTA
#define SW1_PIN 	4   //Pin del Pulsador 1
#define SW1_PORT 	PORTA   //Puerto del Pulsador 1
#define SW1_CLOCK 	SIM->SCGC5              	|= SIM_SCGC5_PORTA_MASK
#define	SW1_MODE 	SW1_PORT->PCR[SW1_PIN]  	|= PORT_PCR_MUX(1)      // Se elige alternativa 1 : GPIO
#define SW1_INPUT	PTA->PDDR 					&= ~( 1<<SW1_PIN )


//SW3
#define SW3_GPIO 	PTC
#define SW3_PIN 	3   //Pin del Pulsador 3
#define SW3_PORT 	PORTC   //Puerto del Pulsador 3
#define SW3_CLOCK 	SIM->SCGC5               	|= SIM_SCGC5_PORTC_MASK //Asigno clock al puerto  
#define	SW3_MODE 	SW3_PORT->PCR[SW3_PIN]     	|= PORT_PCR_MUX(1)      //Configuro el mux para GPIO
#define SW3_INPUT 	PTA->PDDR 		 		  	&= ~( 1<<SW3_PIN )      //Pulsador como entrada


void inicializar(void);

int8_t SW1_get(void);

int8_t SW3_get(void);

void switch_3_interrupt(void);
void switch_1_interrupt(void);

#endif /* MACROSKL43Z_H_ */
