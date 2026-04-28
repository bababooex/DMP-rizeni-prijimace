#include "ds1841.h"

// read registers
uint8_t DS1841_read(uint8_t reg) {
    uint8_t val = 0;
    swi2c_read_buf(DS1841_ADDR, reg, &val, 1);
    return val;
}
// write registers
void DS1841_write(uint8_t reg, uint8_t value) {
    swi2c_write_buf(DS1841_ADDR, reg, &value, 1);
}
// init ds1841
void DS1841_init(void) {

    if (swi2c_test_slave(DS1841_ADDR) != 0)
        return;

    DS1841_enableSaveToEEPROM(0);
    DS1841_enableAdderMode(0);
    DS1841_enableManualLUTAddr(1);
    DS1841_enableManualWiper(1);
    DS1841_enableUpdateMode(1);
}
//get wiper value
uint8_t DS1841_getWiper(void) {
    return DS1841_read(DS1841_WR);
}
//set wiper value
uint8_t DS1841_setWiper(uint8_t value) {
    if (value > 127) return 1;
    DS1841_write(DS1841_WR, value);
    return 0;
}
//other stuff 
int8_t DS1841_getTemperature(void) {
    return (int8_t)DS1841_read(DS1841_TEMP);
}

float DS1841_getVoltage(void) {
    uint8_t val = DS1841_read(DS1841_VOLTAGE);
    return val * DS1841_VCC_LSB_TO_MV;
}

uint8_t DS1841_getLUT(uint8_t index) {
    if (index > 71) return 0xFF;
    return DS1841_read(DS1841_LUT + index);
}

uint8_t DS1841_setLUT(uint8_t index, uint8_t value) {
    if (index > 71 || value > 127) return 1;
    DS1841_write(DS1841_LUT + index, value);
    delay_ms(20);
    return 0;
}

uint8_t DS1841_getLUTSelection(void) {
    uint8_t raw = DS1841_read(DS1841_LUTAR);
    return raw - DS1841_LUT;
}

uint8_t DS1841_setLUTSelection(uint8_t index) {
    if (index > 71) return 1;
    DS1841_write(DS1841_LUTAR, DS1841_LUT + index);
    return 0;
}


void DS1841_enableSaveToEEPROM(uint8_t enable) {
    uint8_t val = DS1841_read(DS1841_CR0);
    if (enable) val &= ~(1 << 7);
    else val |= (1 << 7);
    DS1841_write(DS1841_CR0, val);
}

void DS1841_enableUpdateMode(uint8_t enable) {
    uint8_t val = DS1841_read(DS1841_CR1);
    if (enable) val |= (1 << 0);
    else val &= ~(1 << 0);
    DS1841_write(DS1841_CR1, val);
}

void DS1841_enableAdderMode(uint8_t enable) {
    uint8_t val = DS1841_read(DS1841_CR1);
    if (enable) val |= (1 << 1);
    else val &= ~(1 << 1);
    DS1841_write(DS1841_CR1, val);
}

void DS1841_enableManualLUTAddr(uint8_t enable) {
    uint8_t val = DS1841_read(DS1841_CR2);
    if (enable) val |= (1 << 1);
    else val &= ~(1 << 1);
    DS1841_write(DS1841_CR2, val);
}

void DS1841_enableManualWiper(uint8_t enable) {
    uint8_t val = DS1841_read(DS1841_CR2);
    if (enable) val |= (1 << 2);
    else val &= ~(1 << 2);
    DS1841_write(DS1841_CR2, val);
}

void DS1841_enableLUTMode(uint8_t enable) {
    DS1841_enableManualLUTAddr(enable);
    DS1841_enableManualWiper(!enable);
    DS1841_enableUpdateMode(1);
}


uint8_t DS1841_setWiperDefault(uint8_t value) {
    if (value > 127) return 1;

    DS1841_enableSaveToEEPROM(1);
    DS1841_enableUpdateMode(0);
    delay_ms(20);

    DS1841_write(DS1841_IVR, value);
    delay_ms(20);

    DS1841_enableSaveToEEPROM(0);
    DS1841_enableUpdateMode(1);
    return 0;
}
