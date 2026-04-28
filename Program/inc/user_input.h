#ifndef USER_INPUT_H
#define USER_INPUT_H
// note - most complicated library in this whole code (~.~)
#include "stm8_hd44780.h"
#include "bigfont.h"
#include "keypad.h"
#include <stdlib.h>
#include <string.h>
#include "extern_variables.h"
#include "stm8s_delay.h"
#include "freq_and_ch_handler.h"
#include "memory.h"
#include "audio.h"
#include "mbs74t1aef.h"
#include "disp_perks.h"
#include "stat_and_conf.h"
#include "band.h"
#include "rxtx.h"

/**
 * @brief library for handling user input via matrix keypad and other initialisation
 */
 //the allmighty magic handler
void input_handler(void);
#endif // USER_INPUT_H