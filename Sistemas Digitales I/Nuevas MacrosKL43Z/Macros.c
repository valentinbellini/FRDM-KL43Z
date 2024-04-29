#include "Macros.h"

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


	// Configuramos redistencias Pull Up de los Switches
	SW1_PORT->PCR[SW1_PIN]  |= (1 << PORT_PCR_PE_MASK) | (1 << PORT_PCR_PS_MASK);
	SW3_PORT->PCR[SW3_PIN]  |= (1 << PORT_PCR_PE_MASK) | (1 << PORT_PCR_PS_MASK);


	// Configuramos el GPIO
	LED_VERDE_ON;		//Prendemos
	LED_VERDE_OUTPUT; 	//Configuramos como Salida
	LED_VERDE_OFF;		//Apagamos por seguridad

	LED_ROJO_ON;
	LED_ROJO_OUTPUT;
	LED_ROJO_OFF;

	SW1_GPIO->PDDR &=~(1<<SW1_PIN);	//Configuramos Switch como entrada
	SW3_GPIO->PDDR &=~(1<<SW3_PIN);

}

void led_rojo(int x){
    if (x)
        LED_ROJO_ON;
    else
        LED_ROJO_OFF;
}
void led_verde(int x){
    if (x)
        LED_VERDE_ON;
    else
        LED_VERDE_OFF;
}

void SW3_interrupt(){
	PORTC->PCR[3] |= PORT_PCR_IRQC(0x0A); 		// Interrupción por flanco creciente
	__NVIC_SetPriority(PORTC_PORTD_IRQn, 0); 	// Seteo de prioridad // NVIC->ISPR[7] |= (0<<31)
	__NVIC_EnableIRQ(PORTC_PORTD_IRQn); 		// Habilito interrupcion en el puerto -- PORTC_PROTD_IRQn = 31 // NVIC_ISER |= (1<<31)
}

void SW1_interrupt(){
	PORTA->PCR[4] |= PORT_PCR_IRQC(0x0A); 	// Interrupción por flanco creciente
	__NVIC_SetPriority(PORTA_IRQn, 0); 		// Seteo de prioridad // NVIC->ISPR[7] |= (0<<31)
	__NVIC_EnableIRQ(PORTA_IRQn); 			// Habilito interrupcion en el puerto -- PORTC_PROTD_IRQn = 31 // NVIC_ISER |= (1<<31)
}
