#include "stm8s.h" 
//not my library, for timing works well

#define F_CPU 				16000000UL 
#define dly_const			(F_CPU / 16000000.0F) 


void delay_us(unsigned int  value);
void delay_ms(unsigned int value);