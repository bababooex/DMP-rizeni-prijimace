#ifndef TSA5523_H
#define TSA5523_H

#include "swi2c.h"
#include "extern_variables.h"
#include "stm8s_delay.h"
// TSA5523 default address
#define TSA5523_ADDR (0x61 << 1)

/*
typedef enum {
	CP_OFF = 0,
    CP_50uA,
    CP_250uA
} tuner_CP_settings;
*/

/**
 * @brief functions regarding rf tuner
 * - set frequency of rf tuner(start it)
 * - update frequency (can be problematic with pb set wrong)
 * - change control byte as you like - additional setup
 * - put tv tuner to "pseudo" sleep, it doesnt have any sleep mode as default
 */
void tuner_setFrequency(float frec);
void tuner_setChargepump(tuner_CP_settings chargepump);
uint8_t tuner_getStatus(void);
void tuner_sleep(void);

#endif // TSA5523_H
