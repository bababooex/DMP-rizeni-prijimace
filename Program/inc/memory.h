#ifndef MEMORY_H
#define MEMORY_H
#include "stm8s.h"
#include "stm8s_flash.h"
#include "stm8_hd44780.h"
#include "extern_variables.h"
#include "stm8s_delay.h"
// tuner flags
#define TUNER_FLAG_ADDR   0x4000
#define TUNER_FREC1_ADDR  0x4001
#define TUNER_FREC2_ADDR  0x4003
// mod flags
#define MOD_FLAG_ADDR     0x4004
#define MOD_CHAN_ADDR     0x4005
// time for M flag
#define TIME_PASSED(now, last) ((uint16_t)((now) - (last)))

/**
 * @brief memory saver library
 * - save tomemory based on mode
 * - specifically save tuner frequency (called from memory_save)
 * - specifically save modulator frequency (called from memory_save)
 */
void memory_save(char key); // non-blocking function for memory save
void memory_save_tuner(uint16_t frec1, uint8_t frec2);//save tuner frequency
void memory_save_mod(uint8_t channel);//save mod channel

#endif //MEMORY_H