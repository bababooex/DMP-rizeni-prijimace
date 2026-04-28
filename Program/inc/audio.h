#ifndef AUDIO_H
#define AUDIO_H

#include "stm8_hd44780.h"
#include "ds1841.h" // i2c pot
#include "swi2c.h"
#include "rotary_encoder.h" // using second encoder
#include "extern_variables.h"

#define MUTE_OUT_PIN GPIOD, GPIO_PIN_3

/**
 * @brief functions for audio bar control and setting i2c pot
 * - Blank the audio -> audio not yet activated
 * - Init audio - display volume and set to default
 * - Dynamically update audio bar
 * - Mute audio - by button
 */

void AUD_blank(void);
void AUD_init(void);
void AUD_update(void);
void AUD_mute(void);
#endif // AUDIO_H


