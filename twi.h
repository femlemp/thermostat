#ifndef TWI_H_
#define TWI_H_

#include "global_define.h"
void I2C_Init(void);
void I2C_WriteData(uint8_t, uint8_t, uint8_t*);
#endif
