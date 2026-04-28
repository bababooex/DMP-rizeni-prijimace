#ifndef AV2026_H
#define AV2026_H
//not used in my code, but good function to add in the future (frequency range extending to 2.5 GHz)
#include "swi2c.h"
#include "stm8s_delay.h"
// AV2026 default address and xtal frequency in kHz
#define AV2026_ADDR   (0x60 << 1)
#define AV2026_XTAL   27000
// offset tuner to get IQ output as IF for easy work
#define FREQ_OFFSET 42
// very high bandwidth to avoid filtering issue at IF (doesnt matter, pushing BW to its limits)
#define AV2026_tune(frec) AV2026_setFrequency(frec, 100000) //main macro for most simplicity
//init registers for sat tuner - total 42
static const uint8_t initRegs[] = {
        0x38,0x00,0x00,0x50,0x1F,0xA3,0xFD,0x58,
        0x1E,0x82,0x88,0xB4,
        0xD6,0x40,0x5B,0x6A,
        0x66,0x40,0x80,0x2B,
        0x6A,0x50,0x91,0x27,
        0x8F,0xCC,0x21,0x10,
        0x80,0xEE,0xF5,0x7F,
        0x4A,0x9B,0xE0,0xE0,
        0x36,0x00,0xAB,0x97,
        0xC5,0xA8
    };

/**
 * @brief functions regarding this sat tuner driver
 * - write registers
 * - set frequency and symbolrate (bandwidth)
 */

void AV2026_write_regs(uint8_t regAddr, uint8_t* data, uint8_t length);
void AV2026_setFrequency(float frec, uint32_t bb_sym);


#endif //AV2026_H