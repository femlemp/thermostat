#include <avr/io.h>
#include "usart.h"

void uart_ctor(u16 ubrr){ 
	UBRR0H = (u8)(ubrr>>8);
	UBRR0L = (u8)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	 
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
	UCSR0A = (1<<RXC0);
	/*
	UMSEL01,UMSEL00=0 -> Asynch
	UPM01 = 0, UPM00=0 -> Parity none
	USBS=0 -> 1 stop bit
	UCSZ01 = 1, UCSZ00 = 1 -> 8 bit data 
	*/
}
void uart_putbyte(u8 byte){
	while (!(UCSR0A & (1<<UDRE0))) continue;
	UDR0 = byte;
}
void uart_putstring(u8* str){
	while(*str != '\0') uart_putbyte(*(str++));
}
void uart_putsymbol(uint32_t data){
	u8 temp[34] = {0};
	u8 i,end;
	u32 _temp;
	
	uart_putbyte(temp[33]);
	for(i = sizeof(data)*8, end = 0; data != 0; data /= 10, i--){
		if(i > 32) return;
		if(data / 10 == 0){
			end=1;
			_temp=data;
		}
		else _temp=data-((data / 10) * 10);
		temp[i] = _temp + '0';
		if(end) break;
	}
	uart_putstring(&temp[i]);
}

