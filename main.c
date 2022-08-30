#include "global_define.h"
#include <stdlib.h>
#include <limits.h>
#include <avr/eeprom.h>

#include "setting.h"
#include "drive_seg.h"
#include "timer.h"
#include "adc.h"
#include "usart.h"

typedef enum{
	NOTHING=0,
	ENTER_TEMP=1,
	CALC_TEMP=2	
}mode_t;
typedef struct{
	mode_t mode;
	uint8_t cur_temp_sensor;
	uint8_t gap_encoder;
	uint8_t error;
	uint8_t blink;
}temp_real_t;

temp_real_t temp_real={
	.mode=CALC_TEMP,
	.cur_temp_sensor=5,
	.gap_encoder=0,
	.error=104,
	.blink=105
};

volatile uint32_t timer_cnt=0;
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
	EIMSK= 0;
	//if(PIN_S1){//best way used timer
		if(!(PIN_S2))
			temp_real.gap_encoder--;
		if(PIN_S2)
			temp_real.gap_encoder++;
	//}
	timer_cnt=TIMER_start();
	_delay_ms(60);
	EIMSK|= (1<<INT0);
	if(temp_real.mode==CALC_TEMP){
		temp_real.mode = ENTER_TEMP;
	}
}

void PIN_init(void){
	DDRD = 0x00;
	DDRD |= (0<<S2)|(0<<S1);
	PORTD =0x00;
	PORTD |= (1<<S2)|(1<<S1);
	
	DDRB = 0x00;
	DDRB |= (1<<RELAY_DIR);
	DDRC = 0x00;
	PORTC =0x00;
}

int main(void){
	
	__asm__("cli");
	
	PIN_init();
	DIGSEG_init();
	ADC_init();
	TIMER_init();
	USART_init(103);
	
	_EEGET(temp_real.gap_encoder,TEMP_SAVE_ADR);
	   
	__asm__("sei");
	
	_delay_ms(50);
	
	EICRA|=	(1<<ISC01)|(0<<ISC00); //falling edge
	EIMSK|= (1<<INT0);
	
    while(1){
		switch(temp_real.mode){	
				
			case ENTER_TEMP:
			
			if(TIMER_start()-timer_cnt>4000){
				_EEPUT(TEMP_SAVE_ADR,temp_real.gap_encoder);
				timer_cnt=0;
				TIMER_reset();
				DIGSEG_print(&temp_real.blink);
				_delay_ms(200);
				temp_real.mode=CALC_TEMP;
			}
			if(temp_real.gap_encoder>=100)
				temp_real.gap_encoder=0;
			DIGSEG_print(&temp_real.gap_encoder);
			break;
			
			case CALC_TEMP:
			
			temp_real.cur_temp_sensor=(uint8_t)ADC_read();
			if(temp_real.cur_temp_sensor<temp_real.gap_encoder){
				if(RELAY_CHECK) continue;
				else{
					RELAY_ON;
					_delay_ms(10);
				}
			}
			if(temp_real.cur_temp_sensor>(temp_real.gap_encoder+2) || temp_real.cur_temp_sensor==temp_real.gap_encoder){
				RELAY_OFF;
				_delay_ms(10);
			}
			USART_putsymbol((uint32_t)ADC_read());
			_delay_ms(1000);
			DIGSEG_print(&temp_real.cur_temp_sensor);
			break;
			
			case NOTHING:
			default:
				DIGSEG_print(&temp_real.error);
			break;
		}
    }
}
