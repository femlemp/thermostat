#include "timer.h"

static volatile uint32_t cnt;

ISR(TIMER0_COMPA_vect){
	cnt++;
}

void TIMER_init(void){
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS00)|(1<<CS01);//prescaler
	OCR0A = ((F_CPU/64)/1000)-1;
	TIMSK0 |= (1<<OCIE0A);
}
uint32_t TIMER_start(void){
	uint32_t timer;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		timer = cnt;
	}
	return timer;
}
void TIMER_reset(){
	cnt = 0;
}
