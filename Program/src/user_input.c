#include "user_input.h"
bool tpen_enabled; //modified tpen state
//only work when defined, otherwise just floating bools
bool experimental_mode = false;
bool exp_start = false;

//timing hardcoded sequences to stop bad behavior
static void keypad_guard(void)
{
		uint16_t settle, timeout;
		timeout = system_ms + 300; 

    while (keypad_get_key() != 0) {
        flag_handler();
        if (TIME_PASSED(system_ms, timeout) > 0) break;
    }
		
    settle = system_ms;
    while (TIME_PASSED(system_ms, settle) < 450) {
        if (keypad_get_key() != 0) {
            settle = system_ms; 
        }
        flag_handler();
    }
		
} 

//for handling all inputs
void input_handler(void)
{
	  float freq;
	  char key;
    uint8_t col;
		int count,count1,count2;
		char new_frec1[4] = {0};
    char new_frec2 = '0';
		char new_chan[3] = {0}; 
		uint8_t input_position;
		uint8_t input_stage;
		bool input_error = false;	
	  key = keypad_get_key();
		if (!experimental_mode){
				memory_save(key); //save function 
		}
    if (currentMode == MODE_RX)
    {
        if (tuner_start == 1) {
            if (FLASH_ReadByte(TUNER_FLAG_ADDR) == 1) {
							  lcd_gotoxy(0,2);
                lcd_puts("MEM.SET?");
								lcd_gotoxy(0,3);
                lcd_puts("#YES*NO");
								lcd_gotoxy(13,3);
                lcd_puts("M");
								keypad_guard();
                while (1) {
									  flag_handler();
                    key = keypad_get_key();
                    if (key == '#') {
												uint16_t f1;
												uint8_t f2;
												f1 = FLASH_ReadByte(TUNER_FREC1_ADDR) |
                              (FLASH_ReadByte(TUNER_FREC1_ADDR + 1) << 8);
												f2 = FLASH_ReadByte(TUNER_FREC2_ADDR);

											if (f1 >= 44 && f1 <= 870 && f2 <= 9)//validity check
											{
													frec1 = f1;
													frec2 = f2;
													frec = frec1 + frec2 / 10.0;
			
													tuner_setFrequency(frec);
													if (fm_band_flag)//only for fm mode
															chargepump = CP_50uA;
													else
															chargepump = CP_250uA;
			
													tuner_display_frequency(frec1, frec2, tun_off);
													Band_update();//update band
													AUD_init();  // Init audio on load
													c_update();
													s_update();//update status
													lcd_gotoxy(13,3);
													lcd_puts(" ");
												
													tuner_setFrequency(frec);	
													
													if (frec>=460.0 && frec<=470.0){//fix bug withe receiver symbol
														rxtx_fix();
													}
													
													tuner_start = 0;
													return;
											}
											else
											{
													FLASH_Unlock(FLASH_MEMTYPE_DATA);
													FLASH_ProgramByte(TUNER_FLAG_ADDR, 0);
													FLASH_Lock(FLASH_MEMTYPE_DATA);
													tuner_start = 1;
													break;
											}
									}
									else if (key == '*')
									{
											lcd_gotoxy(13,3);
											lcd_puts(" ");
											tuner_start = 1;
											break;
									}
							}
					}
					else
					{
							tuner_start = 1;
					}
			}

        if (key == '*' || tuner_start == 1) {
							keypad_guard();
            do {
                //input_error = false;
                lcd_gotoxy(13,2);
								lcd_puts("K");
                for (count1 = 0; count1 < 9 ; count1++) {
                    lcd_gotoxy(count1,2);
                    lcd_puts(" "); 
                    lcd_gotoxy(count1,3);                        
                    lcd_data(0x5F);
                } 
                for (count2 = 10; count2 < 13; count2++) {
                    lcd_gotoxy(count2,2);
                    lcd_puts(" "); 
                    lcd_gotoxy(count2,3);                        
                    lcd_data(0x5F); 
                }        

                memset(new_frec1, 0, sizeof(new_frec1));
                new_frec2 = '0';
                input_stage = 1;
                input_position = 0;
                while (1)
                {
									  flag_handler();
                    key = keypad_get_key();
                    if (key >= '0' && key <= '9') {
												if (input_stage == 1 && strlen(new_frec1) < 3) {
														new_frec1[strlen(new_frec1)] = key;
														col = strlen(new_frec1) * 3 - 3; // 0,3,6
														bigfont_digit(col, 2, key - '0');
														if (strlen(new_frec1) == 3) input_stage = 2;
												}
												else if (input_stage == 2) {
														new_frec2 = key;
														bigfont_digit(10, 2, key - '0');  // right after decimal
												}
										}

                    // confirm input with '#'
                    else if (key == '#')
                    {
                        frec1 = atoi(new_frec1);
                        frec2 = new_frec2 - '0';
                        if (frec1 > 870 || frec1 < 44)
                        {
                            input_error = true;
                            break;
                        }
                        frec = frec1 + (frec2 / 10.0);    
                        tuner_setFrequency(frec);
												if (tuner_start == 1){//start values
												
												  AUD_init();//init audio 
													s_update();//update status
												  if (fm_band_flag==1){
														chargepump=CP_50uA;//default for fm mode
													}
													else{
														chargepump=CP_250uA;//default for tv mode
													}
													c_update();//update config
													Band_update();//update band
													memory_save_tuner(frec1, frec2);	//save to memory
												}
                        tuner_display_frequency(frec1, frec2, tun_off);
												tuner_setFrequency(frec);//set frequency second time, ensures correct initialization
												
												if (frec>=460.0 && frec<=470.0){//fix bug withe receiver symbol
														rxtx_fix();
													}
													
                        tuner_start = 0;
												lcd_gotoxy(13,2);
												lcd_puts(" ");
                        return;
                    }

                    // cancel input with '*'
                    else if (key == '*')
                    {
                        return;
                    }
                }

                // show error message if invalid
                if (input_error)
                {
									  for (count1 = 0; count1 < 9 ; count1++) {
												lcd_gotoxy(count1,2);
												lcd_puts(" "); 
												lcd_gotoxy(count1,3);                        
												lcd_puts(" "); 
										} 
										for (count2 = 10; count2 < 13; count2++) {
												lcd_gotoxy(count2,2);
												lcd_puts(" "); 
												lcd_gotoxy(count2,3);                        
												lcd_puts(" ");  
										}        
                    lcd_gotoxy(0, 2);
                    lcd_puts("FREQ.!");
										lcd_gotoxy(0, 3);
                    lcd_puts("ERROR!");
                    delay_ms(2000);
                }

            } while (input_error);
        }
    else{
        return; //go back not pressed anything
    }
	}
    else if (currentMode == MODE_TX)
    {
			if (mod_start == 1) {
            if (FLASH_ReadByte(MOD_FLAG_ADDR) == 1) {
							  lcd_gotoxy(22,2);
                lcd_puts("MEMSET");
								lcd_gotoxy(22,3);
                lcd_puts("#Y*N");
                lcd_gotoxy(14, 3);
                lcd_puts("M");
								keypad_guard();
                while (1) {
									  flag_handler();
                    key = keypad_get_key();
                    if (key == '#') {
											  uint8_t ch;
											  ch = FLASH_ReadByte(MOD_CHAN_ADDR);
											if (ch >= 21 && ch <= 69)
												{
													channel = ch;
													sound = BG_5_5MHZ;
													tpen_enabled = true;
			
													rfmod_start(channel, sound, tpen_enabled);
													mod_display_chan(channel, mod_off);
													c_update();
													s_update();//update status
													lcd_gotoxy(14,3);
													lcd_puts(" ");
			
													mod_start = 0;
													return;
											}
											else
											{
													FLASH_Unlock(FLASH_MEMTYPE_DATA);
													FLASH_ProgramByte(MOD_FLAG_ADDR, 0);
													FLASH_Lock(FLASH_MEMTYPE_DATA);
													mod_start = 1;
													break;
											}
									}
									else if (key == '*')
									{
											lcd_gotoxy(14, 3);
											lcd_puts(" ");
											mod_start = 1;
											break;
									}
							}
					}
					else
					{
							mod_start = 1;
					}
			}
		
		
			if ((key == '*' || mod_start == 1)&& !experimental_mode){// avoid conflict with experimental mode
						keypad_guard();
						do {
							//input_error = false;
							lcd_gotoxy(14,2);
							lcd_puts("K");
							for (count = 22; count < 28; count++) {
									lcd_gotoxy(count, 2);
									lcd_puts(" "); 
									lcd_gotoxy(count, 3);
									lcd_data(0x5F); 
							}
						memset(new_chan, 0, sizeof(new_chan));
						input_position = 0;
						while (1)
						{
							flag_handler();
							key = keypad_get_key();
							if (key >= '0' && key <= '9')
							{
									if (input_position < 2)
									{
											new_chan[input_position++] = key;
											bigfont_digit(22 + (input_position - 1) * 3, 2, key - '0');
									}
							}
							else if (key == '#')
							{
									new_chan[input_position] = '\0'; 
									channel = atoi(new_chan);
									if (channel < 21 || channel > 69)
									{
											input_error = true;
											break;
									}
									if (mod_start == 1){//start values
										rfmod_start(channel, sound, tpen_enabled);
										tpen_enabled=true;//default set to tpen on	
										sound=BG_5_5MHZ;//default sound carrier
										c_update();//update config
										s_update();//update status
										memory_save_mod(channel); //save to memory 
									}
									else{//only change channel, user changes everything else
										rfmod_changeChannel(channel, tpen_enabled);
									}
									mod_display_chan(channel, mod_off);
									mod_start = 0;
									lcd_gotoxy(14,2);
									lcd_puts(" ");
									return;
							}
							else if (key == '*')
							{
									return; //cancel input
							}
					}

					if (input_error)
					{
							for (count = 22; count < 28; count++) {
									lcd_gotoxy(count, 2);
									lcd_puts(" "); 
									lcd_gotoxy(count, 3);
									lcd_puts(" "); 
							}
							lcd_gotoxy(22, 2);
							lcd_puts("CHAN.!");
							lcd_gotoxy(22, 3);
							lcd_puts("ERROR!");
							delay_ms(2000);
					}
			} while (input_error);
	}
	
	// EXPERIMENTAL FREE FREQUENCY SETTING MODE, ONLY AVAIABLE AFTER INITIALIZATION, FREQUENCY RANGE 30-1023MHz
	else if (mod_start == 0)
	{
	#ifdef EXPERIMENTAL_MODE	
    if (key == '0' && !experimental_mode) {
        experimental_mode = true;
        exp_start = true;
				for (count = 15; count < 28; count++) {
									lcd_gotoxy(count, 2);
									lcd_puts(" "); 
									lcd_gotoxy(count, 3);
									lcd_puts(" "); 
				}
        lcd_gotoxy(28, 3);
        lcd_puts("E");   // indicator
    }


    else if (key == '0' && experimental_mode) {
			  experimental_mode = false;
        lcd_gotoxy(28, 3);
        lcd_puts(" ");
				for (count1 = 15; count1 < 28; count1++) { 
				lcd_gotoxy(count1, 2); lcd_puts(" "); 
			  }
				for (count2 = 15; count2 < 28; count2++) { 
				lcd_gotoxy(count2, 3); lcd_puts(" "); 
					}
				perks_show();
				mod_display_chan(channel, mod_off);
				rfmod_start(channel, sound, tpen_enabled);
        return;
    }

    if (!experimental_mode)
        return;

    if (key == '*' || exp_start == true) {
        exp_start = false;
        do {
            memset(new_frec1, 0, sizeof(new_frec1));
            new_frec2 = '0';
            input_stage = 1;
            input_position = 0;
						for (count = 15; count < 28; count++) {
								lcd_gotoxy(count, 2);
								lcd_puts(" "); 
								lcd_gotoxy(count, 3);
								lcd_puts(" "); 
						}
				

						for (count = 15; count < 24; count++) {
								lcd_gotoxy(count, 3);
								lcd_data(0x5F);
						}
						for (count = 25; count < 28; count++) {
								lcd_gotoxy(count, 3);
								lcd_data(0x5F);
						}	
						lcd_gotoxy(14, 2);
						lcd_puts("K");
						lcd_gotoxy(24, 3);
						lcd_data(0xCD);
						keypad_delay();
						keypad_guard();
            while (1)
            {
                flag_handler();
                key = keypad_get_key();
                if (key >= '0' && key <= '9') {
                    if (input_stage == 1 && strlen(new_frec1) < 3) {
                        new_frec1[strlen(new_frec1)] = key;
                        col = 15 + (strlen(new_frec1) - 1) * 3;
                        bigfont_digit(col, 2, key - '0');
                        if (strlen(new_frec1) == 3) input_stage = 2;
                    }
                    else if (input_stage == 2) {
                        new_frec2 = key;
                        bigfont_digit(25, 2, key - '0');
                    }
                }

                else if (key == '#') {
                    uint16_t f1 = atoi(new_frec1);
                    uint8_t f2 = new_frec2 - '0';
                    freq = f1 + (f2 / 10.0);

                    if (freq < 30.0 || freq > 999.9) {
                        input_error = true;
                        break;
                    }


                    rfmod_setFrequencyRaw(freq, sound, tpen_enabled);//set raw frequency with dividers
                    lcd_gotoxy(14, 2);
                    lcd_puts(" ");
                    break;
                }

                else if (key == '*') {
 
                    exp_start = true;
                    return;
                }

                else if (key == '5') {
                    experimental_mode = false;
                    
                }
            }

 
            if (input_error) {
								for (count = 15; count < 28; count++) {
									lcd_gotoxy(count, 2);
									lcd_puts(" "); 
									lcd_gotoxy(count, 3);
									lcd_puts(" "); 
								}
                lcd_gotoxy(15, 2);
                lcd_puts("FREQ.!");
                lcd_gotoxy(15, 3);
                lcd_puts("ERROR!");
                delay_ms(2000);
            }

        } while (input_error);
			}
	}
		else{
			  #endif // end of experimental mode
        return; //go back not pressed anything
    }
	}
}