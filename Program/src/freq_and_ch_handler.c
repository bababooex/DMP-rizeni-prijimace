#include "freq_and_ch_handler.h"


void update_tuner_freq(void) {
	  int16_t diff;
	  Encoder1_Update();
    diff = Encoder1_GetDelta(); //compare differences
    if (diff != 0) {
			  frec2+=diff;
        frec = frec1 + (frec2 / 10.0);
				if (frec2 > 9)
				{
						frec2 = 0;
						frec1++;
				}
				else if (frec2 < 0)
				{
						frec2 = 9;
						frec1--;
				}
				//frequency handler - set boundaries, although not really designed to go this way, it can be pushed
				if (frec > 870.0)
				{
						frec = 870.0;
						frec1 = 870;
						frec2 = 0;
				}
				else if (frec < 44.0)
				{
						frec = 44.0;
						frec1 = 44;
						frec2 = 0;
				}
				frequency_changed=true;
		    tuner_setFrequency(frec);
				tuner_display_frequency(frec1, frec2, tun_off);
    }
}

void update_mod_chan(void) {
		uint8_t diff;
		Encoder1_Update();
		diff = Encoder1_GetDelta(); //compare differences
    if (diff != 0) {
        channel+=diff;

        // channel handler - set boundaries
        if (channel > 69) channel = 69;
        if (channel < 21) channel = 21;
				channel_changed=true;
        rfmod_changeChannel(channel, tpen_enabled);
        mod_display_chan(channel, mod_off);
    }

}

void tuner_display_frequency(uint16_t frec1, int8_t frec2, bool tun_off) {
    if (tun_off){
        int count1,count2;
        for (count1 = 0; count1 < 9 ; count1++) {
            lcd_gotoxy(count1,2);
            lcd_data(0x5F); 
            lcd_gotoxy(count1,3);                        
            lcd_data(0xBF); 
        for (count2 = 10; count2 < 13; count2++) {
            lcd_gotoxy(count2,2);
            lcd_data(0x5F); 
            lcd_gotoxy(count2,3);                        
            lcd_data(0xBF); 
					}
			}
	  }
    else{
        if (frec1 >= 100){
            bigfont_number(0,2,frec1);
					}
        else {
				    lcd_gotoxy(0,3);
						lcd_puts("   ");
            lcd_gotoxy(0,2);
						lcd_puts("   ");	
            bigfont_number(3,2,frec1);
			  }
        bigfont_number(10,2,frec2);
    }
}
void mod_display_chan(uint8_t channel, bool mod_off) {
    if (mod_off){ 
        int count;
        for (count = 22; count < 28 ; count++) {
            lcd_gotoxy(count,2);
            lcd_data(0x5F); 
            lcd_gotoxy(count,3);                        
            lcd_data(0xBF);          
			}
		}
    else{
        bigfont_number(22,2,channel);
	}
}

