#include "off_handler.h"

bool tun_off = false;
bool mod_off = false;

//Power for tuner
void tuner_toggle_off(void) {
    if (tun_off) {
        // Turn tuner back ON
        tun_off = false;
        AUD_init();
        tuner_setFrequency(frec);
        tuner_display_frequency(frec1, frec2, tun_off);
				c_update();
				s_update();//update status
				Band_update();
    } else {
        // Turn tuner OFF
				lcd_gotoxy(10,2);
				lcd_puts("   ");
				lcd_gotoxy(10,3);
				lcd_puts("   ");
				lcd_gotoxy(0,2);
				lcd_puts("         ");
				lcd_gotoxy(0,2);
				lcd_puts("TUNER IS");
				lcd_gotoxy(0,3);
				lcd_puts("         ");
				lcd_gotoxy(0,3);
				lcd_puts("OFF");
				delay_ms(500);
        tuner_sleep();
        AUD_mute();
        tun_off = true;
        tuner_display_off();
				Band_init(); //serves as off mode
				s_a_c_init(); //serves as off mode also
    }
}

//Power for modualtor
void mod_toggle_off(void) {
    if (mod_off) {
        mod_off = false;
        rfmod_start(channel, sound, tpen_enabled);
        mod_display_chan(channel, mod_off);
				c_update();
				s_update();//update status
    } else {
			  lcd_gotoxy(22,2);
				lcd_puts("MODUL.");
				lcd_gotoxy(22,3);
				lcd_puts("IS OFF");
				delay_ms(500);
        rfmod_sleep();
        mod_off = true;
        mod_display_off();
				s_a_c_init(); //serves as off mode also
    }
}

// OFF Display for Tuner
void tuner_display_off(void) {
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


// OFF Display for Modulator
void mod_display_off(void) {
	  int count;
    for (count = 22; count < 28 ; count++) {
				lcd_gotoxy(count,2);
				lcd_data(0x5F); 
				lcd_gotoxy(count,3);                        
				lcd_data(0xBF); 
		}
}