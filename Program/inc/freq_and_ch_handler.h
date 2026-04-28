#ifndef FREQ_AND_CH_HANDLER_H   
#define FREQ_AND_CH_HANDLER_H   

#include "stm8_hd44780.h"
#include "stdlib.h"
#include "rotary_encoder.h"//reads this
#include "extern_variables.h"
#include "bigfont.h"
//libraries for two main devices
#include "tsa5523.h"
#include "mbs74t1aef.h"
/**
 * @brief functions for updating channel and frequency
 * - update tuner frequency global var
 * - display it
 * - update modulator channel global var
 * - display it
 */
void update_tuner_freq(void);
void tuner_display_frequency(uint16_t frec1, int8_t frec2, bool tun_off);
void update_mod_chan(void);
void mod_display_chan(uint8_t channel, bool mod_off);

#endif // FREQ_AND_CH_HANDLER_H   