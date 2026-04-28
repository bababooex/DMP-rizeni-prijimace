#ifndef EXTERN_VARIABLES_H
#define EXTERN_VARIABLES_H

#include "stm8s.h"
#include <stdbool.h>
#include "pcf8563.h"

// this file is not a library, but rather for all external variables and typedefs used, mainly for beter readability etc.

void flag_handler(void);
extern uint16_t tim4_ms_counter;//for ms
extern uint8_t tim4_s_counter;//for seconds
extern uint8_t status_counter;//for status update
extern uint32_t system_ms; //global "milis"
extern uint32_t debounce_time; //debouncer - not sure, why it is there
//updates
extern bool update_status;//status bool
extern bool rtc_update_flag;//for rtc updating
extern bool blink_colon;//colon bool

//this is for manipulating with main frequencies etc.
extern int8_t frec2; //for decimal (needs to be int, otherwise cant go under zero)
extern uint16_t frec1; //for whole number
extern float frec; //for float
extern uint8_t channel; //for modulator channel

//choosing between tuner or modulator avaiable for every library 
typedef enum {
    MODE_RX = 0,   // tuner - default
    MODE_TX = 1    // modulator
} DeviceMode;
extern DeviceMode currentMode;

//audio settings
typedef enum { //RF modulator can move in these 4 modes -> mhz means subcarrier audio distance from vidceo signal
    MN_4_5MHZ = 0,
    BG_5_5MHZ,
    I_6_0MHZ,
    DK_6_5MHZ
} rfmod_sound_system;
extern rfmod_sound_system sound;

//chargepump settings
typedef enum {
	  CP_OFF = 0, //OFF mode only for experimental purposes - see datasheet for test modes
    CP_50uA,
    CP_250uA
} tuner_CP_settings;
extern tuner_CP_settings chargepump;

extern bool fm_band_flag;//fm flag for charge pump default config
//tpen flag
extern bool tpen_enabled;
//off modes
extern bool tun_off;
extern bool mod_off;
//start flags
extern bool tuner_start;
extern bool mod_start;
//encoder counters
extern int16_t encoder1_counter;//setting first rotary encoder to my own value
extern int16_t encoder2_counter;//setting second rotary encoder to my own value
//experimental mode
extern bool experimental_mode;
//again not used
/*
#define MUX_C GPIOB, GPIO_PIN_7
#define MUX_B GPIOB, GPIO_PIN_6
#define MUX_A GPIOD, GPIO_PIN_4
*/
//audio
extern uint8_t new_wiper_value;
//i2c nonblocking flags
extern bool audio_changed;
extern bool channel_changed;
extern bool frequency_changed;


#endif // EXTERN_VARIABLES_H