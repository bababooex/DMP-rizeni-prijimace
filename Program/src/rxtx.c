#include "rxtx.h"
static uint8_t Transmitter[8] = {0x0A,0x11,0x15,0x15,0x0E,0x04,0x0A,0x15};
static uint8_t Receiver[8] = {0x1F,0x15,0x0E,0x04,0x04,0x04,0x0A,0x15};

void rxtx_show(void) {
    lcd_select(LCD_CTRL1);  
    lcd_store_symbol(5, Transmitter);
    lcd_store_symbol(6, Receiver);
    lcd_gotoxy(0,1);
    lcd_data(6);
    lcd_gotoxy(2,1);
    lcd_puts("RX FREQ MHz");
    lcd_gotoxy(15,1);
    lcd_data(5);
    lcd_gotoxy(17,1);
    lcd_puts("TX TV CHAN.");
}
void rxtx_fix(void) {//fixes bug with receiver symbol changing to (
	  lcd_select(LCD_CTRL1);  
    lcd_store_symbol(6, Receiver);
    lcd_gotoxy(0,1);
    lcd_data(6);
}
	