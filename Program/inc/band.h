#ifndef BAND_H
#define BAND_H 
#include "stm8_hd44780.h"
#include "extern_variables.h"
/**
 * @brief Library handling band info based on frequency of tv tuner
 * - initializes band indicator
 * - updates band in main loop based on float frec
 */
void Band_init(void);
void Band_update(void);
#endif // BAND_H
