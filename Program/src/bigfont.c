#include "bigfont.h"

static const uint8_t gBigFontShapeTable[8][8] = {
    {0b00111,0b01111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111}, // LT
    {0b11111,0b11111,0b11111,0b00000,0b00000,0b00000,0b00000,0b00000}, // UB
    {0b11100,0b11110,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111}, // RT
    {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b01111,0b00111}, // LL
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111}, // LB
    {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11110,0b11100}, // LR
    {0b11111,0b11111,0b11111,0b00000,0b00000,0b00000,0b11111,0b11111}, // UMB
    {0b11111,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111}  // LMB
};

// Digit mapping 0-9
static const uint8_t gBigFontDigits[10][6] = {
    {0,1,2,3,4,5},         // 0
    {255,1,2,255,255,5},   // 1
    {6,6,2,3,7,7},         // 2
    {6,6,2,7,7,5},         // 3
    {3,4,2,255,255,5},     // 4
    {0,6,6,7,7,5},         // 5
    {0,6,6,3,7,5},         // 6
    {1,1,2,255,0,255},     // 7
    {0,6,2,3,7,5},         // 8
    {0,6,2,255,255,5}      // 9
};

// letters not used in my code, because why
static const uint8_t gLetterCGRAM[26][6][8] = {
    // A
    {{
        0b00111,
        0b01110,
        0b11100,
        0b11100,
        0b11100,
        0b11100,
        0b11111,
        0b11111
    },{
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111
    },{
        0b11100,
        0b01110,
        0b00111,
        0b00111,
        0b00111,
        0b00111,
        0b11111,
        0b11111
    },{
        0b11111,
        0b11111,
        0b11100,
        0b11100,
        0b11100,
        0b11100,
        0b11100,
        0b11100
    },{
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    },{
        0b11111,
        0b11111,
        0b00111,
        0b00111,
        0b00111,
        0b00111,
        0b00111,
        0b00111
    }},
    // B...
}
;

void bigfont_init(void) {
	  uint8_t i;
    for(i=0;i<8;i++) {
        lcd_store_symbol(i,(uint8_t*)gBigFontShapeTable[i]);
    }
}
void load_cgram_set(uint8_t ctrl, const uint8_t table[8][8]) {
    uint8_t i;
    lcd_select(ctrl);

    for (i = 0; i < 8; i++) {
        lcd_store_symbol(i, (uint8_t*)table[i]); 
    }
}
void load_letter_cgram(char ch) {
	  uint8_t idx = ch - 'A';
		uint8_t slot;
    if (ch < 'A' || ch > 'Z') return;
    for (slot = 0; slot < 6; slot++) {
        lcd_store_symbol(slot, (uint8_t*)gLetterCGRAM[idx][slot]);
    }
}
uint8_t LCD_CTRL_for_row(uint8_t row) {
    if(row < 2) return LCD_CTRL1;
    else        return LCD_CTRL2;
}

void bigfont_digit(uint8_t col,uint8_t row,uint8_t digit) {
	  uint8_t i;
    if(digit>9) return;
    lcd_gotoxy(col,row);
    for(i=0;i<3;i++) {
        uint8_t c = gBigFontDigits[digit][i];
        if(c==255) lcd_data(' ');
        else lcd_data(c);
    }
    lcd_gotoxy(col,row+1);
    for(i=3;i<6;i++) {
        uint8_t c = gBigFontDigits[digit][i];
        if(c==255) lcd_data(' ');
        else lcd_data(c);
    }
}

void bigfont_clear(uint8_t col,uint8_t row) {
    lcd_gotoxy(col,row);
    lcd_puts("   ");
    lcd_gotoxy(col,row+1);
    lcd_puts("   ");
}

void bigfont_letter(uint8_t col, uint8_t row, char ch) {
    if (ch < 'A' || ch > 'Z') return;

    load_letter_cgram(ch);

    lcd_gotoxy(col, row);
    lcd_data(0);
    lcd_data(1);
    lcd_data(2);

    lcd_gotoxy(col, row + 1);
    lcd_data(3);
    lcd_data(4);
    lcd_data(5);
}
void bigfont_number(uint8_t col, uint8_t row, uint32_t number) {
	  uint32_t temp;
		uint8_t digit;
    char buffer[11];
    int i = 0;
    int j;
    temp = number;

    if (number == 0) {
        bigfont_digit(col, row, 0);
        return;
    }


    while (temp > 0) {
        buffer[i++] = (temp % 10);
        temp /= 10;
    }

    for (j = 0; j < i; j++) {
        digit = buffer[i - j - 1];
        bigfont_digit(col + j * 3, row, digit); 
    }
}

void bigfont_text_animate(uint8_t row, uint8_t col, const char *text, uint16_t display_time_ms, uint16_t jump_time_ms) {
    uint8_t base_col = col;
    uint8_t i;

    for (i = 0; text[i]; i++) {
        char ch = text[i];


        if (ch >= 'A' && ch <= 'Z') {
            bigfont_letter(base_col, row, ch);
        } else if (ch >= '0' && ch <= '9') {
            bigfont_digit(base_col, row, ch - '0');
        } else {
            bigfont_clear(base_col, row);
        }

 
        delay_ms(display_time_ms);

        if (text[i+1]) {
            bigfont_clear(base_col, row);
            delay_ms(jump_time_ms); 
        }


        base_col += 3;
    }
		 bigfont_clear(base_col - 3, row);
}