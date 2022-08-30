#ifndef SETTING_H_
#define SETTING_H_

//pin where encoder
#define S1 2
#define S2 3
#define PIN_S1 PIND&0b00000100
#define PIN_S2 PIND&0b00001000

#define RELAY_PORT PORTB //where port relay
#define RELAY_PIN PORTB4 //where port relay
#define RELAY_DIR DDB4 //where pin relay

#define RELAY_ON RELAY_PORT|=(1<<RELAY_PIN)
#define RELAY_OFF RELAY_PORT&=~(1<<RELAY_PIN)
#define RELAY_CHECK PINB&(1<<RELAY_DIR)
	
#define TEMP_SAVE_ADR 0x01//where saving enter temp

#endif 
