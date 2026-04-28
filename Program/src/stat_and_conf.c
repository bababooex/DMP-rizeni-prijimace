#include "stat_and_conf.h"
bool M; //external mode to check for changing charge pump (handling FM/TV modes)
rfmod_sound_system sound;//sound system handling here
tuner_CP_settings chargepump;//charge pump current handling here
void s_a_c_init(void)
{
    lcd_gotoxy(29,1);
    lcd_puts("C:---------");
    lcd_gotoxy(29,2);
    lcd_puts("S:---------");
}

void s_update()
{

    if (currentMode == MODE_RX)
    {
			  uint8_t status = 0;
				int ADC, SS;
				char buf[10];
				bool FL;
				bool POR;
				status = tuner_getStatus();
        if (status==0xFF){
					lcd_gotoxy(31,2);
					lcd_puts("CommError");//cant read, throw error
				}
				else{
        FL  = (status >> 6) & 1;
        M  = (status >> 5) & 1;
				POR  = (status >> 7) & 1;
				ADC = status & 7;
        lcd_gotoxy(31,2);
			  if (M == 1){//check if mode is set to TV
					 lcd_puts("IF"); //IF strenght indicator
					 //reverse order to ADC
					 if (ADC == 0){
								SS=4;
					 }
					 else if (ADC == 1){
							  SS=3;
					 }
					 else if (ADC == 2){
							  SS=2;
					 }
					 else if (ADC == 3){
							  SS=1;
					 }
					 else if (ADC == 4){
							  SS=0;
					 }
				   sprintf(buf,"%i",SS); //can also be signal strenght
				   lcd_puts(buf);
			  }
				else {
					lcd_puts("XXX"); //if FM mode,doesnt work->print undefined
				}
        lcd_puts("FL");
        lcd_data(FL ? '+' : '-'); // PLL lock flag
				#ifdef FM_MODE // only for fm mode
        lcd_puts("M");
        lcd_puts(M ? "tv" : "fm"); // mode of operation
				#endif
				#ifndef FM_MODE
				lcd_puts("PR");
        lcd_data(POR ? '+' : '-'); // power on reset flag, because I dont have any other bits left display anything else
				#endif
			  }
    }
    else if (currentMode == MODE_TX)
    { 
		    uint8_t status = 0;
			  bool OOR,Y1,Y2;
        status = rfmod_getStatus();
        if (status==0xFF){
					lcd_gotoxy(31,2);
					lcd_puts("CommError");//cant read, throw error
				}
				else{
        OOR = (status >> 0) & 1;
        Y1  = (status >> 1) & 1;
        Y2  = (status >> 2) & 1;

        lcd_gotoxy(31,2);
        lcd_puts("OR"); //Out of range error
        lcd_data(OOR ? '+' : '-');
        lcd_puts("Y1"); //VCO out of range additional info
        lcd_data(Y1 ? '+' : '-');
        lcd_puts("Y2"); //High or low VCO (CH 51 and more)
        lcd_data(Y2 ? '+' : '-');
			  }
    }
}

// config display
void c_update()
{
    lcd_gotoxy(31,1);
		
    if (currentMode == MODE_TX)
    {
        lcd_puts("TP");
        lcd_data(tpen_enabled ? '+' : '-');

        switch(sound)//audio subcarrier
        {
            case MN_4_5MHZ: lcd_puts("AUDM/N"); break;
            case BG_5_5MHZ: lcd_puts("AUDB/G"); break;
            case I_6_0MHZ:  lcd_puts("AUD I "); break;
            case DK_6_5MHZ: lcd_puts("AUDD/K"); break;
        }
    }
    else if (currentMode == MODE_RX)
    {
        switch(chargepump)//charge pump -> sensitivity
        {
            case CP_OFF:    lcd_puts("CP OFF   "); break;//experimental - only for show off
            case CP_50uA:   lcd_puts("CP 50uA  "); break;
            case CP_250uA:  lcd_puts("CP 250uA "); break;
        }
    }
}

//configs
void c_toggleTPEN(void)
{
    if (currentMode != MODE_TX) return; // eventhough main code will handle incorrect mode, just to be sure
    tpen_enabled = !tpen_enabled;

    rfmod_changeChannel(channel, tpen_enabled);
    c_update();
}

void c_toggleAUDIO(void)
{
    if (currentMode != MODE_TX) return;

    sound++;
    if (sound > DK_6_5MHZ) sound = MN_4_5MHZ;

    rfmod_setSoundCarrier(sound);
    c_update();
}

void c_toggleCP(void)
{
	  if (currentMode != MODE_RX) return;
    chargepump++;
    if (chargepump > CP_250uA) chargepump = CP_OFF;

		tuner_setChargepump(chargepump);
    c_update();
}
