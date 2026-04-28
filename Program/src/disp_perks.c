#include "disp_perks.h"

void perks_show(void) {
    //decimal dot
    lcd_gotoxy(9,4);
	lcd_data(0xCD);
    //letter c
    lcd_gotoxy(15, 2);   
    lcd_data(0);          
    lcd_data(1);       
    lcd_data(1);  
    lcd_gotoxy(15, 3);   
    lcd_data(3);         
    lcd_data(4); 
    lcd_data(4); 		
    //letter h
    lcd_gotoxy(18, 2);    
    lcd_data(0);       
    lcd_data(0x5F);         
    lcd_data(2);          

    lcd_gotoxy(18, 3);    
    lcd_data(3);          
    lcd_data(0xBF);          
    lcd_data(5);         
}