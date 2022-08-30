#include "twi.h"
#include <util/twi.h>

void I2C_Init(void){
	#define F_SCL 100000UL
	#define TWPS_VAL 1 //TWPS value set 1 or 4 or 16 or 64(div 4^TWPS) 
	#define F_TWI ((F_CPU / F_SCL) - 16)/(2*TWPS_VAL)

	TWBR = F_TWI; //speed bus (8 mhz = 100 khz)
	TWSR |= (TWPS1<<0)|(TWPS0<<0); //TWI Prescaler
}
static void I2C_Start(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT))) continue;
	/*if((TWSR & 0xF8) != TW_START)
		exeption();*/
}
static void I2C_Stop(void){
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}
static void I2C_SendByte(uint8_t c){
	TWDR = c;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT))) continue;
		/*if((TWSR & 0xF8) != MT_DATA_ACK)
		exeption();*/
}
void I2C_WriteData(uint8_t addr, uint8_t sz, uint8_t* value){
	uint8_t i;
	I2C_Start();
	I2C_SendByte(addr);
	for(i=0;i<sz;i++)
		I2C_SendByte(value[i]);
	I2C_Stop();
}

