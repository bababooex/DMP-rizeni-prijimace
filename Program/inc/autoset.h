#ifndef AUTOSET_H
#define AUTOSET_H

#include "extern_variables.h"
/**
 * @brief simple library for basic autoset tune function - can set both devices, for experimental purposes
 * - set modulator channel as close as tuners frequency
 * - set tuner frequency directly to modulator channel -> transmitting an receiving video over air
 */
void autoset_Modulator(void);
void autoset_Tuner(void);

#endif // AUTOSET_H
