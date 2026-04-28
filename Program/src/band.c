#include "band.h"
//info from https://web.archive.org/web/20220102111350/https://www.delta-team.cz/ham-radio/282-frekvence-ruznych-sluzeb.html
//last aviable custom char for signal symbol
static uint8_t SignalSymbol[8]  = {0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x00};
// init band info
void Band_init(void) {
    lcd_select(LCD_CTRL1);  
    lcd_store_symbol(7, SignalSymbol);
    
    lcd_gotoxy(21, 0);  
    lcd_puts("Band");
    lcd_gotoxy(25, 0);  
    lcd_data(7);
		lcd_gotoxy(27,0);
		lcd_puts("-------------"); // also used for starting
    
}
void Band_update(void) {
    lcd_gotoxy(27,0);
		
    if (frec >= 49.8 && frec <= 50.0) {
        lcd_puts("BABY MONITORS");
    }
    else if (frec >= 50.0 && frec <= 54.0) {
        lcd_puts("6M HAM RADIO ");
    }
    else if (frec >= 54.0 && frec <= 68.0) {
        lcd_puts("VHF TV-LOW   ");
    }
	else if (frec >=67.6 && frec<= 71.3){
		lcd_puts("LOC.BROADCAST");
	}
	else if (frec >=73.4 && frec<= 85.0){
		lcd_puts("PUBLIC FREQ. ");
	}
    else if (frec >= 87.6 && frec <= 108.0) {
        lcd_puts("FM RADIO BAND");
    }
    else if (frec >= 108.0 && frec <= 137.0) {
        lcd_puts("AIRCRAFT COM ");
    }
    else if (frec >= 137.0 && frec <= 144.0) {
        lcd_puts("WX / SAT BAND");
    }
    else if (frec >= 144.0 && frec <= 148.0) {
        lcd_puts("2M HAM RADIO ");
    }
    else if (frec >= 150.0 && frec <= 174.0) {
        lcd_puts("VHF MAR/LAND ");
    }
    else if (frec >= 174.0 && frec <= 230.0) {
        lcd_puts("DAB / TV BAND");
    }
    else if (frec >= 230.0 && frec < 380.0) {
        lcd_puts("MIL/UTILITY  ");
    }
    else if (frec >= 380.0 && frec < 430.0) {
        lcd_puts("TETRA / GOV  ");
    }
    else if (frec >= 400.0 && frec <= 406.0) {
        lcd_puts("MET/WBL BAND ");
    }
	else if ((frec >= 433.0 && frec <= 434.9) || (frec >= 855.0 && frec <= 868.0)) {
        lcd_puts("ISM DEVICES  ");
    }
    else if (frec >= 430.0 && frec <= 440.0) {
        lcd_puts("70CM HAM BAND");
    }
	else if (frec >= 440.0 && frec <= 446.0) {
        lcd_puts("RADIOLOCATORS");
    }
    else if (frec >= 446.0 && frec <= 446.2) {
        lcd_puts("PMR446 RADIO ");
    }
	else if (frec >= 448.0 && frec <= 450.0) {
        lcd_puts("CONSTRUCTION ");
    }
	else if (frec >= 456.0 && frec <= 459.0) {
        lcd_puts("TRAIN PMR COM");
    }
	else if (frec >= 460.0 && frec <= 470.0) {
        lcd_puts("TRAIN/PUBLIC 	");
    }
    else if (frec >= 470.0 && frec <= 790.0) {
        lcd_puts("UHF TV BAND  ");
    }
    else if (frec >= 790.0 && frec <= 862.0) {
        lcd_puts("LTE / CELL   ");
    }
    else { // band not known or undefined
        lcd_puts("-------------");
    }
}

