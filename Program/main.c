/*
 * File:  main.c
 * Date:   4.11.2025 18.47
 * Author:  Adam Fucik   https://github.com/bababooex
 * Licence: GNU/GPL 
 * Task: Receiver with TV tuner and transmitter with rf modulator -> TV transceiver 
 * Description: Main file for controlling tv tuner and rf modulator, needs all library files to work correctly!
 ****************************************************/
 /* lil bit of ascii art
				o                   o
				\               __/
				 \___          /
						 \__    __/
								\  /
		 ____________\/____________
		/   ____________________   \
		|  /__/  \__   \__/  \__\  |
		| |    __   \__    __   \| |
		| |\__/  \__   \__/  \__ | |
		| |    __   \__    __   \| |
		| |\__/  \__   \__/  \__ | |
		| |    __   \__    __   \| |
		| |\__/  \__   \__/  \__ | |
		| |    __   \__    __   \| |
		| |\__/  \__   \__/  \__ | |
		|  \________\___________/  |
		|                 _   _    |
		|  prs           (|) (/)   |
		\_________________________/
				"--"           "--"
 */
 
//all defines, that directly modify, how code behaves
#define SHOW_GUIDE //show guide and copyright
//#define DEBUG //for debugging over swuart 
//#define RTC_SET //set rtc clock time, so it is correct, after that, it should not be defined again
//#define EXPERIMENTAL_MODE //enables experimental mode for rf modulator, only used it specific situations
//#define FM_MODE //Only if required, or if tuner supports it, for FMR and FML outputs, not used in my code

//standard C libraries for stm8
#include "stm8s.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//all display and other important libraries
#include "off_handler.h"
#include "user_input.h"//input logic library
#include "guide.h"//library for guide
#include "stm8s_tim4.h"//tim4
#include "swuart.h"//software uart for debugging
#include "stm8_hd44780.h"//lcd library
#include "bigfont.h"//bigfont library
#include "stm8s_delay.h"//delay library
#include "audio.h"//handling sound 
#include "band.h"//band info
#include "rxtx.h"//decimal dot and CH
#include "rotary_encoder.h"// handle otary encoder movement
#include "disp_perks.h"//display custom chars
#include "keypad.h"//keypad handling library
#include "extern_variables.h"//all extern variables for all libraries
#include "stat_and_conf.h"//status and config bits
#include "autoset.h"//autoset tuner or modulator
#include "freq_and_ch_handler.h"//handle channel or frequency display and movement
#include "memory.h"//memory save library

//all custom device libraries
#include "mbs74t1aef.h"//modulator 
#include "tsa5523.h"//tuner
#include "pcf8563.h"//rtc clock
#include "ds1841.h" //doesnt need to be here, but nice for working with its functions
//#include "av2026.h" //for future frequenc
//for interrupt
uint16_t tim4_ms_counter;
bool rtc_update_flag;
uint32_t system_ms; // dont have to reset it, it would run for 49 days fine
uint8_t tim4_s_counter; //for seconds
uint8_t status_counter; // for periodically updating status
#define ENCODER_STOP_MS 2000 //prevent encoder movement when switching between modes, avoids unresolvable problems

//debugging routine
uint8_t *data; // pointer for printing data on lcd


//status change flags, to avoid noise etc.
bool audio_changed = false;
uint8_t new_wiper_value = 0;
bool update_status = false;
bool channel_changed = false;
bool frequency_changed = false;

//main mode logic, typedef is in extern variables
void tuner_loop(void); //tuner handling
void modulator_loop(void); //modulator handling
DeviceMode currentMode = MODE_RX; //start receiver
bool modeChanged = false; //checking switch

bool tuner_start=true;//start modes for initialization
bool mod_start=true;//start modes for initialization

// buttons - from encoders and for other functions
#define MODE_PIN GPIOC, GPIO_PIN_3//encoder1
#define MUTE_PIN GPIOG, GPIO_PIN_5//encoder2

#define MUTE_OUT_PIN GPIOD, GPIO_PIN_3//for mute function of amp
//buttons for additional functions
#define BTN4_PIN GPIOF, GPIO_PIN_3
#define BTN3_PIN GPIOF, GPIO_PIN_4
#define BTN2_PIN GPIOF, GPIO_PIN_5
#define BTN1_PIN GPIOF, GPIO_PIN_6

/*
//multiplexer pins, not really used in here
#define MUX_C GPIOB, GPIO_PIN_7 // for AF output (also FM/TV mode)
#define MUX_B GPIOB, GPIO_PIN_6 // FM/TV MODE HW audio switch
#define MUX_A GPIOD, GPIO_PIN_4 // HW mute function
*/

//frequency and channel variables
float frec;
int8_t frec2;
uint16_t frec1;
uint8_t channel; 
//old test code thingies
//rfmod_sound_system sound;
//bool input_mode = false;

//struct for time -> refer to PCF8563 library
struct {
    unsigned char s;
    unsigned char m;
    unsigned char h;
    unsigned char dy;
    unsigned char dt;
    unsigned char mt;
    unsigned char yr;
} time;
//other handlers and setups
void clock_setup(void); //handle correct clock config and pheripheral setting
void mode_choice(void); //arrow pointer function on lcd
void TIM4_setup(void); //tim4 for handling interrupt
void I2C_Handler(void); //handle i2c communication - low noise option

void main(void) {
	clock_setup();
	lcd_init();
	#ifdef SHOW_GUIDE
	GPIO_Init(BTN1_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(MUTE_OUT_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_WriteHigh(MUTE_OUT_PIN);
	display_guide();//display guide
	while (1) {
	if (GPIO_ReadInputPin(BTN1_PIN) == RESET) {
			      lcd_gotoxy(0,3);
						lcd_puts("Going to receiver mode...     ");
					  delay_ms(2000);
						lcd_select(LCD_CTRL1);
						lcd_clear();
						lcd_select(LCD_CTRL2);
						lcd_clear();
						break;
			}
	}
	#endif
	#ifdef DEBUG
   uart_init();
	#endif
	TIM4_setup();//for nonblocking time
	enableInterrupts();//enabling interrupts globally
	swi2c_init();//enable swi2c
	//INIT clock functions for setting time, if time set, just dont use them
	#ifdef RTC_SET
	PCF8563_init();//put rtc into write mode
  custom_time(); //set custom clock time
	#endif

	lcd_select(LCD_CTRL2);
	bigfont_init();
	perks_show();
	Band_init();
	s_a_c_init();
	AUD_blank();
	#ifndef SHOW_GUIDE //prevent reinit, if show guide
	GPIO_Init(MUTE_OUT_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_WriteHigh(MUTE_OUT_PIN);
	#endif
	rxtx_show();
	keypad_init();
	Encoder_Init(ENCODER1);
	Encoder_Init(ENCODER2);
	GPIO_Init(MODE_PIN, GPIO_MODE_IN_PU_NO_IT);
	#ifndef SHOW_GUIDE //prevent reinit, if show guide
	GPIO_Init(BTN1_PIN, GPIO_MODE_IN_PU_NO_IT);
	#endif
	GPIO_Init(BTN2_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(BTN3_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(BTN4_PIN, GPIO_MODE_IN_PU_NO_IT);
	while (1) { //main switching choice logic -> The magic and the mystery of this program (potato knishes)
        switch (currentMode) {
            case MODE_RX:
                tuner_loop();
                break;
            case MODE_TX:
                modulator_loop();
                break;
        }
    }
}
void clock_setup(void)
	{
		//setup clock for 16 MHz
    CLK_DeInit();
    CLK_HSECmd(DISABLE);
    CLK_LSICmd(DISABLE);
    CLK_HSICmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
    CLK_ClockSwitchCmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,
                          DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE); //using swi2c not hw
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE); //using swuart not hw
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
		//CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, DISABLE); swuart handles this
		CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);//enable tim4 for interupts
  }
void mode_choice(void){//show arrow at current mode
	if (currentMode == MODE_RX){
		 lcd_gotoxy(13,1);
	   lcd_data(0xF7);
	   lcd_gotoxy(14,1);
	   lcd_data(0x3D);
   }
	else if (currentMode == MODE_TX){
		 lcd_gotoxy(13,1);
	   lcd_data(0x3D);
	   lcd_gotoxy(14,1);
	   lcd_data(0xF6);
	 }
}
//handling interrupt timer and flags
void TIM4_setup(void)
{
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125-1);//1ms clock
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}


// tuner loop mechanism
void tuner_loop(void) {
	  flag_handler();
	  #ifdef DEBUG //debugging via uart
		  data = "Tuner initialised\r\n";
      uart_send_string(data);
	  #endif
		modeChanged = false;
		mode_choice();
		if (tuner_start == 1) {
						mod_display_chan(channel, true);
					  AUD_mute();
		}
		else{
			c_update();
			s_update();//update status
		}
		
    while (!modeChanged) {
				if (GPIO_ReadInputPin(MODE_PIN) == RESET) {
						system_ms = 0; //reset and compare
            modeChanged = true;
            currentMode = MODE_TX;
						delay_ms(600);
						continue;
        }
				if (GPIO_ReadInputPin(BTN4_PIN) == RESET) {
					  delay_ms(300);
						tuner_toggle_off();
						#ifdef DEBUG
						data = "Tuner OFF\r\n";
						uart_send_string(data);
						#endif
        }
				if (tun_off) {
					  s_a_c_init(); //serves as off mode also
            tuner_display_off();
            flag_handler();
            continue; //skip other stuff
        }
				
			  input_handler();
				flag_handler();

				if (GPIO_ReadInputPin(BTN1_PIN) == RESET) {
					  delay_ms(300);
				    c_toggleCP();
        }

				if (GPIO_ReadInputPin(BTN3_PIN) == RESET) {
					if (mod_off==1 || mod_start==1) {
            continue; //skip other stuff
					}
					else{
					 autoset_Modulator();
					 delay_ms(300);
					 mod_display_chan(channel, mod_off);
					 rfmod_changeChannel(channel, tpen_enabled);
				  }
        }

			  if (GPIO_ReadInputPin(MUTE_PIN) == RESET) {
					  delay_ms(300);
						AUD_mute();//simply mute audio if not wanted
				}
        if (GPIO_ReadInputPin(MODE_PIN) == RESET) {
            modeChanged = true;
            currentMode = MODE_TX;
						delay_ms(600);
        }
				if (update_status){
								s_update();
								update_status=0;
				}
				AUD_update();
				I2C_Handler();
				if (system_ms >= ENCODER_STOP_MS){
						update_tuner_freq();
				}
    }
}
//modulator loop mechanism
void modulator_loop(void) {
    flag_handler();

    #ifdef DEBUG
        data = "Modulator initialised\r\n";
        uart_send_string(data);
    #endif
		modeChanged = false;
    mode_choice();

    if (mod_start == 1) {
        s_a_c_init();	  
    }
		else{
			c_update();
			s_update();//update status
		}
    while (!modeChanged) {
        if (!experimental_mode && GPIO_ReadInputPin(MODE_PIN) == RESET) {
						system_ms = 0; //reset and compare
            modeChanged = true;
            currentMode = MODE_RX;
            delay_ms(600);
            continue;
        }


        if (!experimental_mode && GPIO_ReadInputPin(BTN4_PIN) == RESET) {
            delay_ms(300);
            mod_toggle_off();
						#ifdef DEBUG
						data = "Modulator OFF\r\n";
						uart_send_string(data);
						#endif
        }

        if (mod_off) {
            s_a_c_init();
            mod_display_off();
            flag_handler();
            continue;
        }

        input_handler();
        flag_handler();


        if (GPIO_ReadInputPin(BTN2_PIN) == RESET) {
            delay_ms(300);
            c_toggleAUDIO();
        }

        if (!experimental_mode && GPIO_ReadInputPin(BTN1_PIN) == RESET) {
            delay_ms(300);
            c_toggleTPEN();
        }


        if (!experimental_mode && GPIO_ReadInputPin(BTN3_PIN) == RESET) {
            if (!tun_off) {
                autoset_Tuner();
                delay_ms(300);
                tuner_display_frequency(frec1, frec2, tun_off);
                tuner_setFrequency(frec);
            }
        }


				if (GPIO_ReadInputPin(MUTE_PIN) == RESET) {
                delay_ms(300);
                AUD_mute();
				}
				if (!tun_off) {
                AUD_update();
				}
				if (update_status){
								s_update();
								update_status=0;
				}
				I2C_Handler();
				if (!experimental_mode || system_ms >= ENCODER_STOP_MS){
						update_mod_chan();
			  }
    }
}

void I2C_Handler(void)//i2c handling separately, because of noise and basically useless i2c communication
{
    if (audio_changed) {
        DS1841_setWiper(new_wiper_value);
        audio_changed = false;
    }
		else if (currentMode == MODE_TX && channel_changed){
				s_update();
				channel_changed = false;
			}
		else if (currentMode == MODE_RX && frequency_changed){
			  s_update();
				Band_update();
				if (frec>=460.0 && frec<=470.0){//fix bug with receiver symbol
					rxtx_fix();
				}
				frequency_changed = false;
			}
}
//----------------------ASSERT-----------------------
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif