#include "memory.h"

#define M_BLINK_MS 300 //blink time

// static blink states - M symbol 
static bool tuner_blink_active = false;
static uint16_t tuner_blink_start = 0;
static bool mod_blink_active = false;
static uint16_t mod_blink_start = 0;

void memory_save(char key) {
	  uint16_t now;
    now = system_ms;

    if (tuner_blink_active && TIME_PASSED(now, tuner_blink_start) >= M_BLINK_MS) {
        lcd_gotoxy(13, 3);
        lcd_puts(" ");
        tuner_blink_active = false;
    }

    if (mod_blink_active && TIME_PASSED(now, mod_blink_start) >= M_BLINK_MS) {
        lcd_gotoxy(14, 3);
        lcd_puts(" ");
        mod_blink_active = false;
    }
	  //save new values if user wants
    if (key == '#') {
        if (currentMode == MODE_RX) {
            memory_save_tuner(frec1, frec2);
        } else if (currentMode == MODE_TX) {
            memory_save_mod(channel);
        }
    }
}
//flash magic (from my old code from MIT)
void memory_save_tuner(uint16_t frec1, uint8_t frec2) {
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(TUNER_FREC1_ADDR, frec1 & 0xFF);
    FLASH_ProgramByte(TUNER_FREC1_ADDR + 1, (frec1 >> 8) & 0xFF);
    FLASH_ProgramByte(TUNER_FREC2_ADDR, frec2);
    FLASH_ProgramByte(TUNER_FLAG_ADDR, 1);
    FLASH_Lock(FLASH_MEMTYPE_DATA);

    lcd_gotoxy(13, 3);
    lcd_puts("M");
    tuner_blink_start = system_ms;
    tuner_blink_active = true;
}

void memory_save_mod(uint8_t channel) {
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(MOD_CHAN_ADDR, channel);
    FLASH_ProgramByte(MOD_FLAG_ADDR, 1);
    FLASH_Lock(FLASH_MEMTYPE_DATA);

    lcd_gotoxy(14, 3);
    lcd_puts("M");
    mod_blink_start = system_ms;
    mod_blink_active = true;
}