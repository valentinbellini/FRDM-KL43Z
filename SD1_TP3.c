#include "board.h"
#include "MKL43Z4.h"

int Flag = 0;
int Nivel_Luz;
int Nivel_Pote;
int L = 0, Reset = 0;
int Check1 = 0, Check2 = 0;
int CT1 = 0, CT2 = 0, FT1 = 0, FT2 = 0;
int On_S1 = 0, Off_S1 = 0, On_S2 = 0, Off_S2 = 0;

void Init_PIT();			// Inicia PIT para los tiempos entre pulsadas
void Init_Switch();			// Inicializa a los SW como GPIO
void Init_Reloj_TPM();		// Selecciona el reloj para los TPM
void Init_TPM_Led_Rojo();	// TPM1 para el parpadeo del Led Rojo
void Init_PWM_Led_Verde();	// TPM0 para el PWM del Led Verde
void Init_ADC_Luz_y_Pote();	// Conversión para el sensor de Luz y el Pote

void Sistema_1();			// Red de Petri (controla el SW1)
void Sistema_2();			// Red de Petri (controla el SW2)
void Sistema_3();			// Red de Petri (controla el encendido de los Leds)
void Establecer_LEDS();		// Controla el modo de los Leds


int main() {

	Init_PIT();
	Init_Switch();
	Init_Reloj_TPM();
	Init_TPM_Led_Rojo();
	Init_PWM_Led_Verde();
	Init_ADC_Luz_y_Pote();

	for(;;) {
		Sistema_1();
		Sistema_2();
		Sistema_3();
		Establecer_LEDS();
	}
	return 0;
}


void PIT_IRQHandler(){
	if((PIT->CHANNEL[0].TFLG & 1) == 1) {
		PIT->CHANNEL[0].TFLG=0x1;	//Limpio la flag del TIMER 1
		if(CT1) FT1 = 1;
		else FT1 = 0;
	}
	else {
		PIT->CHANNEL[1].TFLG=0x1;	//Limpio la flag del TIMER 1
		if(CT2) FT2 = 1;
		else FT2 = 0;
	}
}

void ADC0_IRQHandler(){

	if(Flag == 0) {
		Nivel_Luz = ADC0->R[0];
		ADC0->SC1[0]=0x40;
		Flag = 1;
	}
	else {
		Nivel_Pote = ADC0->R[0];
		ADC0->SC1[0]=0x43;
		Flag = 0;
	}
}

void TPM1_IRQHandler(){
	TPM1->CONTROLS[0].CnSC |= (1<<7); 	//Borro la flag del LED ROJO
	if(L) PTE->PTOR |= (1<<31); // Toggle led rojo
}


void Init_PIT(){
	SIM->SCGC6 |= (1 << 23);		// Habilito el clock para el PIT
	MCG->C1 |= 0x42;				// Selecciono LIRC (low-frecuency internal
									// reference clock) para el TPM.
	PIT->MCR = 0x00;				// Habilito el timer, y que funcione en debug
	PIT->CHANNEL[0].LDVAL=	1000000;	// Cargo el valor para el TIMER 1
	PIT->CHANNEL[1].LDVAL=	1000000;	// Cargo el valor para el TIMER 2
	//PIT->CHANNEL[0].TCTRL= 0x03;	//Habilito su interrupcion
	NVIC_EnableIRQ(PIT_IRQn);		//Habilito las interrupciones
}

void Init_Switch() {
	// Configuramos los relojes
	SIM->SCGC5	|= SIM_SCGC5_PORTA_MASK; //Clock al SW1
	SIM->SCGC5	|= SIM_SCGC5_PORTC_MASK; //Clock al SW3
//	SIM->SCGC5 	|= SIM_SCGC5_PORTD_MASK; //Clock al Led Verde
	SIM->SCGC5	|= SIM_SCGC5_PORTE_MASK; //Clock al Led Rojo

	// Configuramos los MUX de los pines
//	PORTA->PCR[4]  	|= PORT_PCR_MUX(0); //Deshabilito NMI
	PORTA->PCR[4]  	|= PORT_PCR_MUX(1); //GPIO SW1
	PORTC->PCR[3]   |= PORT_PCR_MUX(1); //GPIO SW3
//	PORTD->PCR[5] 	|= PORT_PCR_MUX(1); //GPIO Led Verde
	PORTE->PCR[31] 	|= PORT_PCR_MUX(1); //GPIOLed Rojo

	// Configuramos el GPIO
//	PTD->PDDR 	|= 1<<5;	//Configuramos como salida Led Verde
//	PTD->PSOR 	|= 1<<5;	//Apagamos por seguridad

	PTE->PDDR   |= 1<<31;	//Configuramos como salida Led Rojo
	PTE->PSOR 	|= 1<<31;	//Apagamos por seguridad

	PTA->PDDR &=~(1<<4);	//Configuramos SW1 como entrada
	PTC->PDDR &=~(1<<3);    //Configuramos SW3 como entrada

	// Configuramos resistencias Pull Up de los Switches
	PORTC->PCR[3]  |= (1 << PORT_PCR_PE_SHIFT) | (1 << PORT_PCR_PS_SHIFT); //SW1
	PORTA->PCR[4]  |= (1 << PORT_PCR_PE_SHIFT) | (1 << PORT_PCR_PS_SHIFT); //SW2
}

void Init_Reloj_TPM(){
	MCG->C1 |= 0x42;			// Selecciono LIRC (low-frecuency internal
								// reference clock) para el TPM.
	MCG->C2 = 0x00;				// Selecciono el clock LIRC de 2 MHz.
	MCG->SC |= 0x02; 			// Selecciono el Clock Divider en un factor de 2
								// para obtener 1 MHz (64 -> 31250Hz)
	SIM->SOPT2 |= (0x03 << 24);	//Habilito el MCGIRCLK para el TPM
}

void Init_TPM_Led_Rojo(){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // Doy clock al PTA
	SIM->SCGC6 |= (1 << 25);			// Habilito el clock para el TPM1
	PORTA->PCR[12] |= PORT_PCR_MUX(3);	// Selecciono el modo TPM1 para el PTA12
	TPM1->SC |= 0x0E;					// Pre-Scaler en 64 y modo de clock (CMOD)
	TPM1->CONTROLS[0].CnSC |= 0x54; 	// Interrupciones con output compare
	NVIC_EnableIRQ(TPM1_IRQn);			// Habilito el vector de interrupción
}

void Init_PWM_Led_Verde(){
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; // Clock al PTD (Led Verde)
	SIM->SCGC6 |= (1 << 24);			// Habilito el clock para el TPM0
	PORTD->PCR[5] |= PORT_PCR_MUX(4);	// Selecciono TPM0 en canal del LED VERDE
	TPM0->CONTROLS[5].CnSC |= 0x24; 	// Habilito PWM alineado al borde (28)
	TPM0->MOD = 255;						// Defino el tamaño del modulo
	TPM0->SC |= 0x09;					// Pre-Scaler en 2 y modo de clock (CMOD)
}

void Init_ADC_Luz_y_Pote(){
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;	// Clock al PTE (sensor de luz y pote)
	PORTE->PCR[20] |= PORT_PCR_MUX(0);	// Función analógica para el Pote
	PORTE->PCR[22] |= PORT_PCR_MUX(0);	// Función analógica para el sensor de luz
	SIM->SCGC6 |= 1<<27;				// Clock al modulo del CAD
	ADC0->CFG1 = 0x71;					// bus_clock/2, resolucion de 8 bits,
										// consumo normal, muestra largo
	ADC0->SC2 = ADC_SC2_REFSEL(1); 		// Tensiones de referencia ALTH y ALTL
	ADC0->SC1[0]=0x43; 					// Habilito interrupcion de ADC y
										// lanzo conversión en canal 3 (luz)
	NVIC_SetPriority(ADC0_IRQn,0);
	NVIC_EnableIRQ(ADC0_IRQn);
}


void Sistema_1() {
	// MARCADO INICIAL
	static int L1_S1 = 1, L2_S1 = 0, L3_S1 = 0, L4_S1 = 0, L5_S1 = 0, L6_S1 = 0;
	// ADQUISICIÓN DE ENTRADAS
	int P1 = ((PTC->PDIR & (1<<3))?0:1);
	// ESTADO DE LAS TRANSICIONES
	int TR1_S1 = L1_S1 && (!P1);
	int TR2_S1 = L2_S1 && P1;
	int TR3_S1 = L3_S1 && (!P1);
	int TR4_S1 = L3_S1 && Reset;
	int TR5_S1 = L4_S1 && P1;
	int TR6_S1 = L4_S1 && (Reset || FT1);
	int TR7_S1 = L5_S1 && (!P1);
	int TR8_S1 = L5_S1 && (Reset || FT1);
	int TR9_S1 = L6_S1 && Reset;
	// EVOLUCIÓN DE LA RED
	if(TR1_S1) {L1_S1 = 0; L2_S1 = 1;}
	if(TR2_S1) {L2_S1 = 0; L3_S1 = 1;}
	if(TR3_S1) {L3_S1 = 0; L4_S1 = 1;}
	if(TR4_S1) {L3_S1 = 0; L1_S1 = 1;}
	if(TR5_S1) {L4_S1 = 0; L5_S1 = 1;}
	if(TR6_S1) {L4_S1 = 0; L2_S1 = 1;}
	if(TR7_S1) {L5_S1 = 0; L6_S1 = 1;}
	if(TR8_S1) {L5_S1 = 0; L1_S1 = 1;}
	if(TR9_S1) {L6_S1 = 0; L2_S1 = 1;}
	// SETEO DE SALIDAS
	Check1 = L1_S1 || L2_S1;
	CT1 = L4_S1 || L5_S1;
	Off_S1 = L4_S1;
	On_S1 = L6_S1;
	if(CT1) PIT->CHANNEL[0].TCTRL |= 0x03;
	else {
		PIT->CHANNEL[0].TCTRL = 0;
		FT1 = 0;
	}
}

void Sistema_2() {
	// MARCADO INICIAL
	static int L1_S2 = 1, L2_S2 = 0, L3_S2 = 0, L4_S2 = 0, L5_S2 = 0, L6_S2 = 0;
	// ADQUISICIÓN DE ENTRADAS
	int P2 = ((PTA->PDIR & (1<<4))?0:1);
	// ESTADO DE LAS TRANSICIONES
	int TR1_S2 = L1_S2 && (!P2);
	int TR2_S2 = L2_S2 && P2;
	int TR3_S2 = L3_S2 && (!P2);
	int TR4_S2 = L3_S2 && Reset;
	int TR5_S2 = L4_S2 && P2;
	int TR6_S2 = L4_S2 && (Reset || FT2);
	int TR7_S2 = L5_S2 && (!P2);
	int TR8_S2 = L5_S2 && (Reset || FT2);
	int TR9_S2 = L6_S2 && Reset;
	// EVOLUCIÓN DE LA RED
	if(TR1_S2) {L1_S2 = 0; L2_S2 = 1;}
	if(TR2_S2) {L2_S2 = 0; L3_S2 = 1;}
	if(TR3_S2) {L3_S2 = 0; L4_S2 = 1;}
	if(TR4_S2) {L3_S2 = 0; L1_S2 = 1;}
	if(TR5_S2) {L4_S2 = 0; L5_S2 = 1;}
	if(TR6_S2) {L4_S2 = 0; L2_S2 = 1;}
	if(TR7_S2) {L5_S2 = 0; L6_S2 = 1;}
	if(TR8_S2) {L5_S2 = 0; L1_S2 = 1;}
	if(TR9_S2) {L6_S2 = 0; L2_S2 = 1;}
	// SETEO DE SALIDAS
	Check2 = L1_S2 || L2_S2;
	CT2 = L4_S2 || L5_S2;
	Off_S2 = L4_S2;
	On_S2 = L6_S2;
	if(CT2) PIT->CHANNEL[1].TCTRL |= 0x03;
	else {
		PIT->CHANNEL[1].TCTRL = 0;
		FT2 = 0;
	}
}

void Sistema_3() {
	// MARCADO INICIAL
	static int L1_S3 = 1, L2_S3 = 0, L3_S3 = 0, L4_S3 = 0;
	// ADQUISICIÓN DE LAS ENTRADAS
	int On = On_S1 || On_S2;
	int Off = Off_S1 || Off_S2;
	// ESTADO DE LAS TRANSICIONES
	int TR1_S3 = L1_S3 && On;
	int TR2_S3 = L2_S3 && Check1 && Check2;
	int TR3_S3 = L3_S3 && Off;
	int TR4_S3 = L4_S3 && Check1 && Check2;
	// EVOLUCIÓN DE LA RED
	if(TR1_S3) {L1_S3 = 0; L2_S3 = 1;}
	if(TR2_S3) {L2_S3 = 0; L3_S3 = 1;}
	if(TR3_S3) {L3_S3 = 0; L4_S3 = 1;}
	if(TR4_S3) {L4_S3 = 0; L1_S3 = 1;}
	// SETEO DE SALIDAS
	L = L2_S3 || L3_S3;
	Reset = L2_S3 || L4_S3;
}

void Establecer_LEDS() {

	if(L) {
		if(Nivel_Luz <= 1){TPM0->CONTROLS[5].CnV = 0;}
		TPM0->CONTROLS[5].CnV = Nivel_Luz;
		TPM1->MOD = 50*Nivel_Pote+50;
		TPM1->CONTROLS[0].CnV= 25*Nivel_Pote+50;	//Programo los valores de cuentas para el toggle del LED VERDE (cada 1 min)
	}
	else {
		PTE->PSOR 	|= 1<<31; 		// Apago Led Rojo
		TPM0->CONTROLS[5].CnV = 0;
	}
}
