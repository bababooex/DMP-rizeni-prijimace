#ifndef RXTX_H
#define RXTX_H
#include "stm8_hd44780.h"
/**
 * @brief functions regarding this library
 * - Simply show receiver and transmitter custom chars + text
 * - Fix bug with receiver symbol, when in range between 460 and 470 mhz
 */
void rxtx_show(void);
void rxtx_fix(void);
#endif // RXTX_H



