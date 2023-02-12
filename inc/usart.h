#pragma once
#include "global_define.h"

void uart_ctor(u16);
void uart_putbyte(u8);
void uart_putstring(u8*);
void uart_putsymbol(u32);

