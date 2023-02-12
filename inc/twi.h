#pragma once
#include "global_define.h"

#define F_SCL 100000UL
#define TWPS_VAL 1 //TWPS value set 1 or 4 or 16 or 64(div 4^TWPS) 

void i2c_init(void);
void i2c_write_data(u8, u8, u8*);

