#ifndef STAT_AND_CONF_H
#define STAT_AND_CONF_H

#include "extern_variables.h"
#include "stm8_hd44780.h"
//libraries for two main devices
#include "tsa5523.h"
#include "mbs74t1aef.h"
#include <stdio.h>
//
/**
 * @brief functions regarding this "additional info" library
 * - init config and status (display placeholders)
 * - update status bits based on device type
 * - update configuration by user and display on lcd
 * - Toggle charge pump setting
 * - Toggle test pattern
 * - Toggle audio mode
 */
void s_a_c_init(void);
void s_update(void);
void c_update(void);
// User toggles, connected to c_update
void c_toggleCP(void);
void c_toggleTPEN(void);
void c_toggleAUDIO(void);

#endif // STAT_AND_CONF_H