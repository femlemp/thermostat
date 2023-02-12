#include <avr/io.h>
#include "timer.h"
#include <util/atomic.h>

static volatile u32 cnt;

ISR(TIMER0_COMPA_vect){
	cnt++;
}

void timer0_ctor(void){
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00)|(1 << CS01);//prescaler
	OCR0A = (( F_CPU / 64) / 1000) - 1;
	TIMSK0 = (1 << OCIE0A);
}
u32 timer0_start(void){
	u32 timer;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		timer = cnt;
	}
	return timer;
}
void timer0_reset(void){
	cnt = 0;
}
