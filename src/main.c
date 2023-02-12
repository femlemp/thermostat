#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "global_define.h"
#include "setting.h"
#include "drive_seg.h"
#include "timer.h"
#include "adc.h"
#include "usart.h"

typedef enum{
	NOTHING = 0,
	ENTER_TEMP = 1,
	CALC_TEMP = 2
}eMode_t;
typedef struct{
	eMode_t state;
	u8 cur_temp_sensor;
	u8 gap_encoder;
	u8 error;
	u8 blink;
}sTemp_real_t;

sTemp_real_t temperature = {
	.state = CALC_TEMP,
	.cur_temp_sensor = 5,
	.gap_encoder = 0,
	.error = 104,
	.blink = 105
};

volatile u32 timer0_cnt = 0;
void pin_ctor(void), periph_ctor(void);

/* ENCODER
<-left
    ____   __
S1     |__|	<- check falling edge
  
   __   ____
S2   |__|   	<-0 value--
  
 -> right
  ____   ___ 
S1    |__|	<- check falling edge
  
   ____   __
S2     |__|   	<-1 value++  
 */
 
ISR(INT0_vect){
	EIMSK = 0;
	//if(PIN_S1){//best way used timer
		if(!(PIN_S2))
			temperature.gap_encoder--;
		if(PIN_S2)
			temperature.gap_encoder++;
	//}
	timer0_cnt = timer0_start();
	_delay_ms(60);
	EIMSK |= (1 << INT0);
	if(temperature.state == CALC_TEMP){
		temperature.state = ENTER_TEMP;
	}
}

int main(void){
	
	__asm__("cli");
	
	pin_ctor();
	periph_ctor();
	
	_EEGET(temperature.gap_encoder, TEMP_SAVE_ADR);
	   
	__asm__("sei");
	
	_delay_ms(50);
	
	EICRA |= (1<<ISC01)|(0<<ISC00); //falling edge
	EIMSK |= (1<<INT0);
	
	while(1){
		switch(temperature.state){	
				
			case ENTER_TEMP:
			
			if(timer0_start() - timer0_cnt > 4000UL){
				_EEPUT(TEMP_SAVE_ADR,temperature.gap_encoder);
				timer0_cnt = 0;
				timer0_reset();
				//digseg_print(&temperature.blink);
				_delay_ms(200);
				temperature.state = CALC_TEMP;
			}
			if(temperature.gap_encoder >= 100)
				temperature.gap_encoder = 0;
			//digseg_print(&temperature.gap_encoder);
			break;
			
			case CALC_TEMP:
			
			//temperature.cur_temp_sensor = (u8)adc_read();
			if(temperature.cur_temp_sensor < temperature.gap_encoder){
				if(RELAY_CHECK) continue;
				else{
					RELAY_ON;
					_delay_ms(10);
				}
			}
			if(temperature.cur_temp_sensor > (temperature.gap_encoder + 2) || temperature.cur_temp_sensor == temperature.gap_encoder){
				RELAY_OFF;
				_delay_ms(10);
			}
			//uart_putsymbol(temperature.gap_encoder);
			_delay_ms(1000);
			//digseg_print(&ccc);
			break;
			
			case NOTHING:
			default:
				digseg_print(&temperature.error);
			break;
		}
		uart_putsymbol(temperature.gap_encoder);
    }
}

void pin_ctor(void){
	DDRD = 0x00;
	DDRD |= (0<<S2)|(0<<S1);
	PORTD = 0x00;
	PORTD |= (1<<S2)|(1<<S1);
	
	DDRB = 0x00;
	DDRB |= (1<<RELAY_DIR);
	DDRC = 0x00;
	PORTC = 0x00;
}

void periph_ctor(void){
	digseg_ctor();
	adc_ctor();
	timer0_ctor();
	uart_ctor(103);
}
