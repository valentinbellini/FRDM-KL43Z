

#include "MacrosKL43Z.h"


void inicializar(void){
	// Configuramos los relojes
	SW1_CLOCK;
	SW3_CLOCK;
	LED_VERDE_CLOCK;
	LED_ROJO_CLOCK;


	// Configuramos los mux de los pines
	SW1_MODE;
	SW3_MODE;
	LED_VERDE_MODE;
	LED_ROJO_MODE;


	// Configuramos resistencias Pull Up de los Switches
	SW1_PORT->PCR[SW1_PIN]  |= (1 << PORT_PCR_PE_MASK) | (1 << PORT_PCR_PS_MASK);
	SW3_PORT->PCR[SW3_PIN]  |= (1 << PORT_PCR_PE_MASK) | (1 << PORT_PCR_PS_MASK);


	// Configuramos el GPIO
	LED_VERDE_ON;		//Prendemos
	LED_VERDE_OUTPUT; 	//Configuramos como Salida
	LED_VERDE_OFF;		//Apagamos por seguridad

	LED_ROJO_ON;
	LED_ROJO_OUTPUT;
	LED_ROJO_OFF;

	SW1_GPIO->PDDR &=~(1<<SW1_PIN);	//Configuramos como entrada
	SW3_GPIO->PDDR &=~(1<<SW3_PIN);

}

int8_t SW1_get(void){
	return ((SW1_GPIO->PDIR & (1<<SW1_PIN))?0:1);
}

int8_t SW3_get(void){
	return ((SW3_GPIO->PDIR & (1<<SW3_PIN))?0:1);
}

void switch_3_interrupt(){
	PORTC->PCR[3] |= PORT_PCR_IRQC(0x0A); 		// Interrupción por flanco creciente
	__NVIC_SetPriority(PORTC_PORTD_IRQn, 0); 	// Seteo de prioridad // NVIC->ISPR[7] |= (0<<31)
	__NVIC_EnableIRQ(PORTC_PORTD_IRQn); 		// Habilito interrupcion en el puerto -- PORTC_PROTD_IRQn = 31 // NVIC_ISER |= (1<<31)
}

void switch_1_interrupt(){
	PORTA->PCR[4] |= PORT_PCR_IRQC(0x0A); 	// Interrupción por flanco creciente
	__NVIC_SetPriority(PORTA_IRQn, 0); 		// Seteo de prioridad // NVIC->ISPR[7] |= (0<<31)
	__NVIC_EnableIRQ(PORTA_IRQn); 			// Habilito interrupcion en el puerto -- PORTC_PROTD_IRQn = 31 // NVIC_ISER |= (1<<31)
}


void Init_All(void){
	// Configuramos los relojes
	SIM->SCGC5              	|= SIM_SCGC5_PORTA_MASK; //SW1_CLOCK
	SIM->SCGC5               	|= SIM_SCGC5_PORTC_MASK //Asigno clock al puerto del SW3
	SIM->SCGC5 		 			|= SIM_SCGC5_PORTD_MASK; //LED_VERDE_CLOCK
	SIM->SCGC5          		|= SIM_SCGC5_PORTE_MASK; //LED_ROJO_CLOCK

	// Configuramos los mux de los pines
	PORTA->PCR[4]  	|= PORT_PCR_MUX(1); // Switch 1 
	PORTC->PCR[3]   |= PORT_PCR_MUX(1); // Switch 3
	PORTD->PCR[5] 	|= PORT_PCR_MUX(1); // Led Verde
	PORTE->PCR[31] 	|= PORT_PCR_MUX(1); // Led Rojo

	// Configuramos resistencias Pull Up de los Switches
	PORTA->PCR[4]  |= (1 << PORT_PCR_PE_MASK) | (1 << PORT_PCR_PS_MASK); // Switch 1 
	PORTC->PCR[3]  |= (1 << PORT_PCR_PE_MASK) | (1 << PORT_PCR_PS_MASK); // Switch 3

	// Configuramos el GPIO
	PTD->PDDR 	|= 1<<5; 				//Configuramos como Salida led verde
	PTD->PSOR 	|= 1<<5;				//Apagamos por seguridad
	PTE->PDDR   |= 1<<31;				//Configuramos como Salida led rojo	
	PTE->PSOR 	|= 1<<31;				//Apagamos por seguridad
	PTA->PDDR &=~(1<<4);				//Configuramos SW1 como entrada
	PTC->PDDR &=~(1<<3);     			//Configuramos SW3 como entrada
}