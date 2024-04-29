/******************************************************************/
//Seg_LCD.c	Rev 1.0	8/20/2014 EPH Initial version
//Author: Ethan Hettwer
//Modified by: Juan Tinazzo 2022
//Purpose:  Allow for easy control of Seg_LCD
/******************************************************************/

#include "Seg_LCD.h"

const static uint8_t LCD_Frontplane_Pin[LCD_NUM_FRONTPLANE_PINS] = {LCD_FRONTPLANE0, LCD_FRONTPLANE1, LCD_FRONTPLANE2, LCD_FRONTPLANE3, LCD_FRONTPLANE4, LCD_FRONTPLANE5, LCD_FRONTPLANE6, LCD_FRONTPLANE7};
//const static uint8_t LCD_Backplane_Pin[LCD_NUM_BACKPLANE_PINS] = {LCD_BACKPLANE0, LCD_BACKPLANE1, LCD_BACKPLANE2, LCD_BACKPLANE3};

/******************************************************************/
//						Function for Initialization of LCD
/******************************************************************/

void SegLCD_Init(void){ //Initializes all components of SLCD on the FRDM-KL46Z
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_SLCD_MASK; //Enable Clock to ports B, C, D and E, and SegLCD Peripheral
	LCD->GCR |= LCD_GCR_PADSAFE_MASK; //Set PADSAFE to disable LCD while configuring
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK;  //Clear LCDEN (LCD Enable) while configuring

	PORTB->PCR[18]  = PORT_PCR_MUX(0u);					//Set PTB18 to LCD_P14
	PORTB->PCR[19]  = PORT_PCR_MUX(0u);					//Set PTB19 to LCD_P15
	PORTC->PCR[0]  = PORT_PCR_MUX(0u);					//Set PTC0 to LCD_P20
	PORTC->PCR[4]  = PORT_PCR_MUX(0u);					//Set PTC4 to LCD_P20
	PORTC->PCR[6]  = PORT_PCR_MUX(0u);					//Set PTC6 to LCD_P26
	PORTC->PCR[7]  = PORT_PCR_MUX(0u);					//Set PTC7 to LCD_P27
	PORTD->PCR[0]  = PORT_PCR_MUX(0u);					//Set PTD0 to LCD_P40
	PORTD->PCR[2]  = PORT_PCR_MUX(0u);					//Set PTD2 to LCD_P42
	PORTD->PCR[3]  = PORT_PCR_MUX(0u);					//Set PTD3 to LCD_P43
	PORTD->PCR[4]  = PORT_PCR_MUX(0u);					//Set PTD4 to LCD_P43
	PORTE->PCR[20]  = PORT_PCR_MUX(0u);					//Set PTE20 to LCD_P59
	PORTE->PCR[21]  = PORT_PCR_MUX(0u);					//Set PTE21 to LCD_P60

	
	//Configure LCD Registers
	//Configure LCD_GCR - General Control Register, controls most options in LCD Peripheral
	LCD->GCR =//LCD_GCR_RVEN_MASK |							//Clear LCD_GCR_RVEN, disable voltage regulator.
							LCD_GCR_RVTRIM(0x00) | 					//Set RVTRIM to 0, irrelevant as voltage regulator is disabled, but setting it to a known state.
							LCD_GCR_CPSEL_MASK |						//Set LCD_GCR_CPSEL to use capacitor charge pump.
							LCD_GCR_LADJ(0x03) |						//Set LCD_GCR_LADJ to 11, slow clock rate = lower power, but higher load capacitance on the LCD requires higher clock speed.
						//LCD_GCR_VSUPPLY |								//Clear LCD_GCR_VSUPPLY, drive VLL3 externally.
							LCD_GCR_PADSAFE_MASK |					//Set LCD_GCR_PADSAFE, leave enabled during configuration process.
						//LCD_GCR_FDCIEN_MASK | 					//Clear LCD_GCR_FDCIEN, No interrupt from fault dection.
							LCD_GCR_ALTDIV(0x00) |					//Set LCD_GCR_ALTDIV to 11, divide alternate clock by 512.  This is assuming an 8MHz External Crystal is used.
						//LCD_GCR_ALTSOURCE_MASK |				//Set LCD_GCR_ALTSOURCE, Part of setting clock source to OSCERCLK, or external oscillator.
							LCD_GCR_FFR_MASK |							//Set LCD_GCR_FFR, allow an LCD Frame Frequency of 46.6Hz to 146.2Hz.  Disable to change range to 23.3Hz to 73.1Hz.
						//LCD_GCR_LCDDOZE_MASK |					//Clear LCD_GCR_LCDDOZE, allows LCD peripheral to run even in doze mode.  Set to disable LCD in doze mode.
						//LCD_GCR_LCDSTP_MASK |						//Clear LCD_GCR_LCDSTP, allows LCD peripheral to run even in stop mode.  Set to disable LCD in stop mode.
						//LCD_GCR_LCDEN_MASK |						//Clear LCD_GCR_LCDEN, Disables all front and backplane pins.  Leave disabled during configuration process.
							LCD_GCR_SOURCE_MASK |						//Set LCD_GCR_SOURCE, Part of setting clock source to OSCERCLK, or external oscillator.
							LCD_GCR_LCLK(0x04) | 						//Set LCD_GCR_LCLK to 111, LCD Clock prescaler where LCD controller frame frequency = LCD clock/((DUTY  |  1) x 8 x (4 | LCLK[2:0]) x Y), where Y = 2, 2, 3, 3, 4, 5, 8, 16 chosen by module duty cycle config
							LCD_GCR_DUTY(0x03);  						//Set LCD_GCR_DUTY to 011, Have 4 backplane pins, so need a 1/4 duty cycle.
	
	//Configure LCD_SEG_AR  - Auxiliary Register, controls blinking of LCD
	LCD->AR = //LCD_AR_BLINK_MASK |							//Clear LCD_SEG_AR_BLINK, Disable SLCD blinking.  Enable to make LCD Blink.
						//LCD_AR_ALT_MASK |								//Clear LCD_SEG_AR_ALT, if enabled LCD back plane sequencer changes to an alternate display.  Only functional if DUTY[2:0] is less than 100(binary).  This allows a blink screen that is not blank.
						//LCD_AR_BLANK_MASK |							//Clear LCD_SEG_AR_BLANK, asserting bit clears all segments in LCD. 
						//LCD_AR_BMODE_MASK |							//Clear LCD_SEG_AR_BMODE, if enabled displays alternate display during blink period instead of blank.
						LCD_AR_BRATE(0x00);								//Set LCD_SEG_AR_BRATE to 000.  Frequency of blink is determined by LCD clock/(2^(12 + BRATE))
						
	//Configure LCD_SEG_FDCR - Fault Detect Control Register, controls use of Fault Detect features of SLCD.
	LCD->FDCR = 0x00000000; 										//Clear all bits in FDCR.  As this will not be covering use of fault detect, this register is cleared.
	
	//Configure LCD_PENn - Pin Enable Register, controls which of the possible LCD pins are used
	//LCD->PEN[0] contains bits 0-31, while LCD->PEN[1] contains bits 32-63.  For pins >= 32, set in LCD->PEN[1] as LCD_PEN_PEN(Pin#-32)
	LCD->PEN[0] =  LCD_PEN_PEN(1u<<7u) |				//LCD_P7
								 LCD_PEN_PEN(1u<<14u) |				//LCD_P14
								 LCD_PEN_PEN(1u<<15u)|				//LCD_P15
								 LCD_PEN_PEN(1u<<20u)|				//LCD_P20
								 LCD_PEN_PEN(1u<<24u)|				//LCD_P24
								 LCD_PEN_PEN(1u<<26u)|				//LCD_P26
								 LCD_PEN_PEN(1u<<27u);				//LCD_P27
	LCD->PEN[1] =  LCD_PEN_PEN(1u<<8u) |				//LCD_P40
								 LCD_PEN_PEN(1u<<10u) |				//LCD_P42
								 LCD_PEN_PEN(1u<<11u) |				//LCD_P43
								 LCD_PEN_PEN(1u<<12u) |				//LCD_P43
								 LCD_PEN_PEN(1u<<27u)|				//LCD_P59
								 LCD_PEN_PEN(1u<<28u);				//LCD_P60
	
	//Configure LCD_SEG_BPENn - Back Plane Enable Register, controls which pins in LCD->PEN are Back Plane (commons)
	//LCD->BPEN[0] contains bits 0-31, while LCD->BPEN[1] contains bits 32-63.  For pins >= 32, set in LCD->BPEN[1] as LCD_BPEN_BPEN(Pin#-32)
	LCD->BPEN[0] = LCD_BPEN_BPEN(1u<<15u)|			//LCD_P15  COM3
								 LCD_BPEN_BPEN(1u<<14u);			//LCD_P14, COM2
	LCD->BPEN[1] = LCD_BPEN_BPEN(1u<<27u) |			//LCD_P59, COM0
								 LCD_BPEN_BPEN(1u<<28u);			//LCD_P60, COM1
	
	//Configure LCD_WFyTOx - Configures 4 Waveform signals, LCD_WF[z] is defined such that x = (z*4) and y = 3 | (z*4)
	//Where x is the n index value of WFn on the least significant byte (bits 7-0) and y is the n index value of WFn on the most significant byte (bits 31-24)

	//Note that "Disabled" is used for pins that are not set as LCD pins, where "Off" is used for pins that are set as LCD, but are just inactive.
	LCD->WF[0] = LCD_WF_WF0(0x00) |
	             LCD_WF_WF1(0x00) |
	             LCD_WF_WF2(0x00) |
	             LCD_WF_WF3(0x00);
	LCD->WF[1] = LCD_WF_WF4(0x00) |
	             LCD_WF_WF5(0x00) |
	             LCD_WF_WF6(0x00) |
	             LCD_WF_WF7(0x00);
	LCD->WF[2] = LCD_WF_WF8(0x00) |
	             LCD_WF_WF9(0x00) |
	             LCD_WF_WF10(0x00) |
	             LCD_WF_WF11(0x00);
	LCD->WF[3] = LCD_WF_WF12(0x00) |
	             LCD_WF_WF13(0x00) |
	             LCD_WF_WF14(0x44) | // WF Pin 14 (COM2) is enabled on Phases C and G
	             LCD_WF_WF15(0x88);  // WF Pin 15 (COM3) is enabled on Phases D and H
	LCD->WF[4] = LCD_WF_WF16(0x00) |
	             LCD_WF_WF17(0x00) |
	             LCD_WF_WF18(0x00) |
	             LCD_WF_WF19(0x00);
	LCD->WF[5] = LCD_WF_WF20(0x00) |
	             LCD_WF_WF21(0x00) |
	             LCD_WF_WF22(0x00) |
	             LCD_WF_WF23(0x00);
	LCD->WF[6] = LCD_WF_WF24(0x00) |
	             LCD_WF_WF25(0x00) |
	             LCD_WF_WF26(0x00) |
	             LCD_WF_WF27(0x00);
	LCD->WF[7] = LCD_WF_WF28(0x00) |
	             LCD_WF_WF29(0x00) |
	             LCD_WF_WF30(0x00) |
	             LCD_WF_WF31(0x00);
	LCD->WF[8] = LCD_WF_WF32(0x00) |
	             LCD_WF_WF33(0x00) |
	             LCD_WF_WF34(0x00) |
	             LCD_WF_WF35(0x00);
	LCD->WF[9] = LCD_WF_WF36(0x00) |
	             LCD_WF_WF37(0x00) |
	             LCD_WF_WF38(0x00) |
	             LCD_WF_WF39(0x00);
	LCD->WF[10] = LCD_WF_WF40(0x00) |
	              LCD_WF_WF41(0x00) |
	              LCD_WF_WF42(0x00) |
	              LCD_WF_WF43(0x00);
	LCD->WF[11] = LCD_WF_WF44(0x00) |
	              LCD_WF_WF45(0x00) |
	              LCD_WF_WF46(0x00) |
	              LCD_WF_WF47(0x00);
	LCD->WF[12] = LCD_WF_WF48(0x00) |
	              LCD_WF_WF49(0x00) |
	              LCD_WF_WF50(0x00) |
	              LCD_WF_WF51(0x00);
	LCD->WF[13] = LCD_WF_WF52(0x00) |
	              LCD_WF_WF53(0x00) |
	              LCD_WF_WF54(0x00) |
	              LCD_WF_WF55(0x00);
	LCD->WF[14] = LCD_WF_WF56(0x00) |
	              LCD_WF_WF57(0x00) |
	              LCD_WF_WF58(0x00) |
	              LCD_WF_WF59(0x11);  // WF Pin 59 (COM0) is enabled on Phases A and E
	LCD->WF[15] = LCD_WF_WF60(0x22) | // WF Pin 60 (COM1) is enabled on Phases B and F
	              LCD_WF_WF61(0x00) |
	              LCD_WF_WF62(0x00) |
	              LCD_WF_WF63(0x00);

	//Disable GCR_PADSAFE and Enable GCR_LCDEN
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; 					//Clear PADSAFE to unlock LCD pins
	LCD->GCR |= LCD_GCR_LCDEN_MASK;							//Set LCDEN to enable operation of LCD
}//End SegLCD_Init

/******************************************************************/
//						End Function for Initialization of LCD
/******************************************************************/

/******************************************************************/
//						Functions for Manipulation of LCD
/******************************************************************/

void SegLCD_Set(uint8_t Value,uint8_t Digit){//Sets a value from 0-F to a specified Digit, with 1 being the leftmost, 4 being the rightmost.  Will not display error is Value is outside of 0-F, but display will not update
	//int k;
	if(Digit > 4){
		SegLCD_DisplayError(0x01);
	}		//Display "Err" if trying to access a digit that does not exist
	else{
		if(Value==0x00)			{LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E |LCD_SEG_F); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x01){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_CLEAR); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x02){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B);}
		else if(Value==0x03){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x04){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x05){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_C);}
		else if(Value==0x06){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_C);}
		else if(Value==0x07){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_CLEAR); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x08){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x09){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x0A){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C);}
		else if(Value==0x0B){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = LCD_SEG_C;}
		else if(Value==0x0C){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = LCD_SEG_A;}
		else if(Value==0x0D){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = LCD_SEG_B | LCD_SEG_C;}
		else if(Value==0x0E){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = LCD_SEG_A;}
		else if(Value==0x0F){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_SEG_E | LCD_SEG_F | LCD_SEG_G); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = LCD_SEG_A;}
		else if(Value==0x10){LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-2)]] = (LCD_CLEAR); LCD->WF8B[LCD_Frontplane_Pin[((2*Digit)-1)]] = LCD_CLEAR;}
	}
}//End SegLCD_Set

void SegLCD_DisplayDecimal(uint16_t Value){//Displays a 4 Digit number in decimal
	if(Value > 9999){
		SegLCD_DisplayError(0x10); //Display "Err" if value is greater than 4 digits
	}
	else{
		SegLCD_Set(Value/1000,1);
		SegLCD_Set((Value - (Value/1000)*1000)/100,2);
		SegLCD_Set((Value - (Value/100)*100)/10,3);
		SegLCD_Set(Value - (Value/10)*10,4);
	}
}//End SegLCD_Display4Digit

void SegLCD_DisplayHex(uint16_t Value){ //Displays a 16 bit number in Hex Format
	SegLCD_Set((Value & 0xF000)>>12, 1);
	SegLCD_Set((Value & 0x0F00)>>8 , 2);
	SegLCD_Set((Value & 0x00F0)>>4 , 3);
	SegLCD_Set((Value & 0x000F)>>0 , 4);
}//End SegLCD_DisplayHex

void SegLCD_DisplayTime(uint8_t Value1, uint8_t Value2){//Displays 2 values separated by a colon
	if((Value1 > 99) | (Value2 > 99)){
		SegLCD_DisplayError(0x10); //Display "Err" if either value is greater than 2 digits
	}
	else{
		SegLCD_Set(Value1/10, 1);
		SegLCD_Set(Value1 % 10, 2);
		SegLCD_Set(Value2/10, 3);
		SegLCD_Set(Value2 % 10, 4);
		SegLCD_Col_On();
	}
}//End SegLCD_DisplayTime

void SegLCD_DisplayError(uint8_t ErrorNum){//Displays Err# on screen, where # is a value 0-F.  If ErrorNum is outside of that range, just displays Err
	LCD->WF8B[LCD_FRONTPLANE0] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G);
	LCD->WF8B[LCD_FRONTPLANE1] = (LCD_SEG_A);
	LCD->WF8B[LCD_FRONTPLANE2] = (LCD_SEG_E | LCD_SEG_G);
	LCD->WF8B[LCD_FRONTPLANE3] = (LCD_CLEAR);
	LCD->WF8B[LCD_FRONTPLANE4] = (LCD_SEG_E | LCD_SEG_G);
	LCD->WF8B[LCD_FRONTPLANE5] = (LCD_CLEAR);
	if(ErrorNum < 0x10){
		SegLCD_Set(ErrorNum,4); //Display ErrorNum in digit 4 if within valid range.  If not, leave blank.
	}
	else{
		LCD->WF8B[LCD_FRONTPLANE6] = (LCD_CLEAR);
		LCD->WF8B[LCD_FRONTPLANE7] = (LCD_CLEAR);
	}
}//End SegLCD_DisplayError

//SegLCD_DP1_On() defined as macro in Seg_LCD.h, Turns on leftmost decimal without disturbing rest of display
//SegLCD_DP1_Off() defined as macro in Seg_LCD.h, Turns off leftmost decimal without disturbing rest of display
//SegLCD_DP2_On() defined as macro in Seg_LCD.h, Turns on center decimal without disturbing rest of display
//SegLCD_DP2_Off() defined as macro in Seg_LCD.h, Turns off center decimal without disturbing rest of display
//SegLCD_DP3_On() defined as macro in Seg_LCD.h, Turns on rightmost decimal without disturbing rest of display
//SegLCD_DP3_Off() defined as macro in Seg_LCD.h, Turns off rightmost decimal without disturbing rest of display
//SegLCD_Col_On() defined as macro in Seg_LCD.h, Turns on colon without disturbing rest of display
//SegLCD_Col_Off() defined as macro in Seg_LCD.h, Turns off colon without disturbing rest of display

/******************************************************************/
//						End Functions for Manipulation of LCD
/******************************************************************/
