#ifndef DS1841_H
#define DS1841_H

#include "swi2c.h"
#include "stm8s_delay.h"
// rewritten from arduino drivers
// Default DS1841 address 
#define DS1841_ADDR   (0x28 << 1)

// DS1841 register map
#define DS1841_IVR      0x00   
#define DS1841_CR0      0x02  
#define DS1841_CR1      0x03   
#define DS1841_LUTAR    0x08  
#define DS1841_WR       0x09 
#define DS1841_CR2      0x0A 
#define DS1841_TEMP     0x0C 
#define DS1841_VOLTAGE  0x0E 
#define DS1841_LUT      0x80  
// const for calculation of lut
#define DS1841_VCC_LSB_TO_MV 25.6f
/**
 * @brief ds1841 i2c potentiometer control library
 * - initialise ds1841
 * - enable different functions
 */
// init function
void DS1841_init(void);
// helper functions
uint8_t DS1841_read(uint8_t reg);
void DS1841_write(uint8_t reg, uint8_t value);

// Functions avaiable in ds1841
void DS1841_enableSaveToEEPROM(uint8_t enable);
void DS1841_enableUpdateMode(uint8_t enable);
void DS1841_enableAdderMode(uint8_t enable);
void DS1841_enableManualLUTAddr(uint8_t enable);
void DS1841_enableManualWiper(uint8_t enable);
void DS1841_enableLUTMode(uint8_t enable);

// Helper functions and data get/set functions
uint8_t DS1841_getWiper(void);
uint8_t DS1841_setWiper(uint8_t value);
int8_t  DS1841_getTemperature(void);
float   DS1841_getVoltage(void);
uint8_t DS1841_getLUT(uint8_t index);
uint8_t DS1841_setLUT(uint8_t index, uint8_t value);
uint8_t DS1841_getLUTSelection(void);
uint8_t DS1841_setLUTSelection(uint8_t index);
uint8_t DS1841_setWiperDefault(uint8_t value);

#endif // DS1841_H
