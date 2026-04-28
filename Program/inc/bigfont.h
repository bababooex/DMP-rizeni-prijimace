#ifndef BIGFONT_H
#define BIGFONT_H
#include "stm8_hd44780.h"
#include "stm8s_delay.h"
/**
 * @brief bigfont library for channel and frequency
 * - initialise bigfont (cgram set)
 * - print digits
 * - print letters
 * - clear bigfont
 * - print numbers (most imporntant)
 * - animate text (requires handling chars separatelly)
 */
void bigfont_init(void);
void bigfont_digit(uint8_t col,uint8_t row,uint8_t digit);
void bigfont_letter(uint8_t col, uint8_t row, char ch);
void bigfont_clear(uint8_t col,uint8_t row);
void bigfont_number(uint8_t col, uint8_t row, uint32_t number);
void bigfont_text_animate(uint8_t row, uint8_t col, const char *text,uint16_t display_time_ms, uint16_t jump_time_ms);
#endif // BIGFONT_H