#include "global_define.h"
#include <avr/io.h>
#include "adc.h"

void adc_ctor(void){
	ADMUX = 0x00;
	ADMUX |= (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0); //supplay avcc|adc in PC0
	ADCSRA = 0x00;
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);//ADC enable|prescaler 64
	ADCSRB = 0x00;
	ADCSRB |= (0<<ADTS0)|(0<<ADTS1)|(0<<ADTS2);
}

double adc_read(void){
	u32 res = 0;
	u8 i,sample;
	sample = 10;
	for(i = 0; i < sample; i++){
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA&(1<<ADSC))) continue;
		res += ADCW;
	}
	res /= sample;
	return res;
}

double adc_conversion_volt(void){
	//adc=(vin*1024)/vref
	double res1 = adc_read();
	const double res2 = 10000.0, vref = 5.0;
	double buf;
	res1 *= vref;
	buf = res1 / (vref - res1);
	res1 = buf * res2;
	return res1;
}

