#ifndef OFF_HANDLER_H
#define OFF_HANDLER_H

#include "stm8s.h"
#include "stm8s_delay.h"
#include "stm8_hd44780.h"
#include "extern_variables.h"
#include "tsa5523.h"
#include "mbs74t1aef.h"
#include "audio.h"
#include "band.h"
#include "freq_and_ch_handler.h"
#include "stat_and_conf.h"
/**
 * @brief library for handling off mode
 * - toggle both devices off
 * - display off on both devices
 */
 
void tuner_toggle_off(void);
void mod_toggle_off(void);
void tuner_display_off(void);
void mod_display_off(void);

#endif //OFF_HANDLER_H