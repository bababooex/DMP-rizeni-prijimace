#include "keypad.h"
//debounce and hold, needs to be set exactly to work well
#define DEBOUNCE_MS  395
#define HOLD_MS      360

static const char keymap[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

static uint8_t row_pins[KEYPAD_ROWS] = {ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN};
static uint8_t col_pins[KEYPAD_COLS] = {COL0_PIN, COL1_PIN, COL2_PIN};

void keypad_init(void) {
	  uint8_t r,c;
    for (r = 0; r < KEYPAD_ROWS; r++) {
        GPIO_Init(ROW_PORT, row_pins[r], GPIO_MODE_OUT_PP_HIGH_FAST);
        GPIO_WriteHigh(ROW_PORT, row_pins[r]);
    }
    for (c = 0; c < KEYPAD_COLS; c++) {
        GPIO_Init(COL_PORT, col_pins[c], GPIO_MODE_IN_PU_NO_IT);
    }
    memset(keys, 0, sizeof(keys));
}

char keypad_get_key(void) {
    uint8_t r,c;
		uint16_t now;
    char pressedKey;
		now = system_ms;
    pressedKey = 0;

    for (r = 0; r < KEYPAD_ROWS; r++) {
        GPIO_WriteLow(ROW_PORT, row_pins[r]);

        for (c = 0; c < KEYPAD_COLS; c++) {
            uint8_t pressed = (GPIO_ReadInputPin(COL_PORT, col_pins[c]) == RESET);

            switch (keys[r][c].state) {//working with 2D array, that has 4 states for best debounce 
							case IDLE:
									if (pressed) {
											keys[r][c].state = DEBOUNCE;
											keys[r][c].timer = now;
									}
									break;
							
							case DEBOUNCE:
									if (!pressed) {
											keys[r][c].state = IDLE; 
									} else if (TIME_PASSED(now, keys[r][c].timer) >= DEBOUNCE_MS) {
											keys[r][c].state = PRESSED;
											keys[r][c].timer = now;
											pressedKey = keymap[r][c];
									}
									break;
							
							case PRESSED:
									if (!pressed) {
											keys[r][c].state = IDLE;
									} else if (TIME_PASSED(now, keys[r][c].timer) >= HOLD_MS) {
											keys[r][c].state = HELD;
									}
									break;
							
							case HELD:
									if (!pressed) {
											keys[r][c].state = IDLE;
									}
									break;
							}
        }

        GPIO_WriteHigh(ROW_PORT, row_pins[r]);
    }

    return pressedKey;
}
