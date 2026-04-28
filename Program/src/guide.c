#include "guide.h"
void display_guide(void)
{
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_puts("UNIVERSAL ANALOG");
    lcd_gotoxy(0,1);
    lcd_puts("TELEVISION TRANSCEIVER");
    lcd_gotoxy(0,2);
    lcd_puts("WITH TV TUNER AND RF MODULATOR");
    lcd_gotoxy(0,3);
    lcd_puts("Programmed and designed by Adam Fucik");
    delay_ms(4000); //show time
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_puts("CP: set rx strenght, TPEN: set tx test");
    lcd_gotoxy(0,1);
    lcd_puts("FL: PLL lock, PR: power on, IF: rx power");
    lcd_gotoxy(0,2);
    lcd_puts("OR: tx out of range, Y1+Y2: VCO error");
    lcd_gotoxy(0,3);
    lcd_puts("PRESS BLUE BTN. TO CONTINUE...");
}