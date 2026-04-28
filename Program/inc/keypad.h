#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm8s.h"
#include "stm8s_delay.h"
#include "extern_variables.h"
#include <stdlib.h>
#include <string.h>
// keypad pins
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3

#define ROW0_PIN   GPIO_PIN_4
#define ROW1_PIN   GPIO_PIN_6
#define ROW2_PIN   GPIO_PIN_7
#define ROW3_PIN   GPIO_PIN_5
#define COL0_PIN   GPIO_PIN_5
#define COL1_PIN   GPIO_PIN_2
#define COL2_PIN   GPIO_PIN_1
//keypad ports
#define ROW_PORT   GPIOC
#define COL_PORT   GPIOE  

//avoid nonsense from keypad - china bad
#define TIME_PASSED(now, last) ((uint16_t)((now) - (last)))
typedef enum {IDLE, PRESSED, DEBOUNCE, HELD} key_state_t;
//2D matrix 
static struct {
    key_state_t state;
    uint16_t timer;
} keys[KEYPAD_ROWS][KEYPAD_COLS];


/**
 * @brief nonblocking keypad library with external timer
 * - initialise keypad functions
 * - read current keypad press with debounce
 */
void keypad_init(void);
char keypad_get_key(void);

#endif // KEYPAD_H