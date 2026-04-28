#ifndef PCF8563_H
#define PCF8563_H

#include "stm8s.h"
#include <stdbool.h>//beacuse of error handling
#include "swi2c.h"
#include "stm8_hd44780.h"
#include "stm8s_delay.h"
//PCF8563 default address
#define PCF8563_ADDR  (0x51 << 1)

// PCF8563 Register Map
#define PCF8563_CONTROL1     0x00
#define PCF8563_CONTROL2     0x01
#define PCF8563_SEC_REG      0x02
#define PCF8563_MIN_REG      0x03
#define PCF8563_HR_REG       0x04
#define PCF8563_DAY_REG      0x05
#define PCF8563_WKDAY_REG    0x06
#define PCF8563_MONTH_REG    0x07
#define PCF8563_YEAR_REG     0x08

/**
 * @brief functions regarding PCF8563 controller
 * - initialise (clean) chip
 * - set custom time
 * - display only time
 * - display only date
 */
void PCF8563_init(void);
void custom_time(void);
void display_time(uint8_t x_pos, uint8_t y_pos);
void display_date(uint8_t x_pos, uint8_t y_pos);
//just helper functions
void PCF8563_write(unsigned char address, unsigned char value);
unsigned char PCF8563_read(unsigned char address);
void PCF8563_set_time(void);
void PCF8563_get_time(void);
unsigned char bcd_to_decimal(unsigned char value);
unsigned char decimal_to_bcd(unsigned char value);

#endif // PCF8563_H


