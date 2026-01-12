#include "libADC.hpp"


void ADC_Init(void){
	// ADCSRA � ADC Control and Status Register A
	ADCSRA = (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	// ADEN:7 - ADC Enable (writing this bit to one enables the ADC)
	// ADPS2:0 - ADC Prescaler sets to 128 (division factor between the system clock frequency and the input clock to the ADC)
	// ADMUX � ADC Multiplexer Selection Register
	ADMUX  =  (1<<REFS0);
	// REFS1:0 - Reference Selection Bits sets to "AVCC with external capacitor at AREF pin"
}


int  ADC_conversion(){
	ADCSRA |= (1<<ADSC);		//	ADC - Start Conversion
	while(ADCSRA & (1<<ADSC)); 	//	wait for finish of conversion

	return ADC;
}
