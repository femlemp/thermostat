#include <avr/io.h>
#include <util/twi.h>
#include "twi.h"

void i2c_init(void){
	#define F_TWI ((F_CPU / F_SCL) - 16)/(2*TWPS_VAL)

	TWBR = F_TWI; //speed bus (8 mhz = 100 khz)
	TWSR = (TWPS1<<0)|(TWPS0<<0); //TWI Prescaler
	#undef F_TWI
}
static void i2c_start(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT))) continue;
	/*if((TWSR & 0xF8) != TW_START)
		exeption();*/
}
static void i2c_stop(void){
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}
static void i2c_send_byte(u8 byte){
	TWDR = byte;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT))) continue;
		/*if((TWSR & 0xF8) != MT_DATA_ACK)
		exeption();*/
}
void i2c_write_data(u8 addr, u8 sz, u8* value){
	u8 i;
	i2c_start();
	i2c_send_byte(addr);
	for(i = 0; i < sz; i++)
		i2c_send_byte(value[i]);
	i2c_stop();
}

