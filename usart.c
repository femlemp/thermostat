#include "usart.h"
volatile uint8_t temp[34]={0};

//if ckdiv8 prog-set ubrr=103,else unprog ubrr=set51
void USART_init(uint16_t ubrr){ 
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	 
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
	UCSR0A |= (1<<RXC0);
	/*
	UMSEL01,UMSEL00=0 -> Asynch
	UPM01 = 0, UPM00=0 -> Parity none
	USBS=0 -> 1 stop bit
	UCSZ01 = 1, UCSZ00 = 1 -> 8 bit data 
	*/
}
void USART_putchar(uint8_t data){
	while (!(UCSR0A & (1<<UDRE0))) continue;
	UDR0 = data;
}
void USART_putstring(uint8_t *str1){
	while(*str1!='\0') USART_putchar(*(str1++));
}
void USART_putsymbol(uint32_t data){
	uint8_t i,end;
	uint32_t _temp;
	USART_putchar(temp[33]);
	for(i=sizeof(data)*8,end=0;data!=0;data/=10,i--){
		if(i>32) return;
		if(data/10==0){
			end=1;
			_temp=data;
		}
		else _temp=data-((data/10)*10);
		temp[i]=_temp+'0';
		if(end) break;
	}
	USART_putstring(&temp[i]);
}

