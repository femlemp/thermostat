#include <util/delay.h>
#include "drive_seg.h"
#include "twi.h"

static const u8 adr_r = 0b01000001;
static const u8 adr_w = 0b01000000;
/* indicator SEG:TOD-5263BMB-N

	__A__			DIG1			DIG2
	|    |			5V-14 			5V-13
       F|    |B			A-16(p0)		A-11(p10)
	|_G__|			B-15			B-10
	|    |  		C-3			C-8   
       E|    |C   		D-2			D-6 
	|_D__|.DP     		E-1			E-5
				F-18			F-12
				G-17(p6)		G-7(p16)
				DP-4(p7)		DP-9(p17)
*/ 
/* interface provided by the chip pcf8575ts
The first data byte in every pair refers to Port 0
(P07 to P00), where as the second data byte in every pair refers to Port 1 (P17 to P10)
 First		Second
 p07...p0 ACK   P17...p10 ACK
*/
static const u8 dig[13]={
   	// A......G
	0b10000001,//0
	0b11001111,
	0b10010010,
	0b10000110,
	0b11001100,
	0b10100100,//5
	0b10100000,
	0b10001111,
	0b10000000,
	0b10000100,//9	
	0b01111111,//DP
	0b11111110,//-
	0b11111111,//nothing
};
void digseg_ctor(void){
	i2c_init();
}
void digseg_print(u8* symb){
	u8 temp[2], dig1;
	dig1 = 0x0F & (*symb / 10);
	if(dig1 >= 10){
		if(*symb == 104){//error
			temp[0] = dig[11];
			temp[1] = dig[11];
		}
		if(*symb == 105){//save
			temp[0] = dig[12];
			temp[1] = dig[12];
		}
	}
	else {
		temp[0] = dig[dig1];
		temp[1] = dig[0x0F&(*symb - dig1*10)];
	}
	i2c_write_data(adr_w, sizeof(temp), temp);
	_delay_ms(50);
}
