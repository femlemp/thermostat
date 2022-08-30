#ifndef USART_H_
#define USART_H_

#include "global_define.h"
void USART_init(uint16_t);
void USART_putchar(uint8_t);
void USART_putstring(uint8_t*);
void USART_putsymbol(uint32_t);
#endif
