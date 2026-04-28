#include "audio.h"

static uint8_t Speaker[8]   = {0x00,0x03,0x05,0x19,0x11,0x19,0x05,0x03};
static uint8_t StartMark[8] = {0x01,0x02,0x04,0x04,0x04,0x04,0x02,0x01};
static uint8_t EmptyBar[8]  = {0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x1F};
static uint8_t Bar[8]       = {0x1F,0x00,0x1F,0x1F,0x1F,0x1F,0x00,0x1F};
static uint8_t EndMark[8]   = {0x10,0x08,0x04,0x04,0x04,0x04,0x08,0x10};

#define AUDIO_BAR_SEGMENTS   12
#define WIPER_MAX            127
#define WIPER_MIN            0
#define WIPER_STEP           ((WIPER_MAX + 1) / AUDIO_BAR_SEGMENTS) // approx calculation for 12 bars

static uint8_t volume_value = 64; //default half volume (7 bit)
static uint8_t last_volume  = 255; //based on my previous things, not sure why is there

// Convert wiper value to bars approx
static uint8_t AUD_WiperToSegments(uint8_t val) {
    float ratio = (float)val / (float)WIPER_MAX;
    uint8_t seg = (uint8_t)(ratio * AUDIO_BAR_SEGMENTS + 0.5f); // approx match ratio for conversion
    if (seg > AUDIO_BAR_SEGMENTS) seg = AUDIO_BAR_SEGMENTS;
    return seg;
}

// Convert encoder position (0 - 12) to wiper value smoothly
static uint8_t AUD_EncoderToWiper(uint8_t enc_val) {
	  float ratio;
    if (enc_val == 0) return 0; // mute
    ratio = (float)(enc_val - 1) / (float)(AUDIO_BAR_SEGMENTS - 1);
    return (uint8_t)(ratio * WIPER_MAX + 0.5f);
}
// Draw the volume bar and mute indicator
static void AUD_DrawBar(uint8_t segments) {
    uint8_t col;
    lcd_select(LCD_CTRL1);

    // Speaker char
    lcd_gotoxy(4, 0);
    lcd_data(0);

    // Mute indicator
    lcd_gotoxy(5, 0);
    if (segments == 0) lcd_data('x');
    else lcd_data(' ');

    // Start
    lcd_gotoxy(6, 0);
    lcd_data(1);

    // 12 segment bar
    for (col = 0; col < AUDIO_BAR_SEGMENTS; col++) {
        lcd_gotoxy(7 + col, 0);
        if (col < segments)
            lcd_data(3);
        else
            lcd_data(2);
    }

    // End mark
    lcd_gotoxy(7 + AUDIO_BAR_SEGMENTS, 0);
    lcd_data(4);
}

// Helper: write DS1841
static void AUD_SetVolume(uint8_t new_val) {
    if (new_val > WIPER_MAX) new_val = WIPER_MAX;
    if (new_val < WIPER_MIN) new_val = WIPER_MIN;
		
		if (new_val==0){
			GPIO_WriteHigh(MUTE_OUT_PIN); // set mute 
		}
		else{
			GPIO_WriteLow(MUTE_OUT_PIN);
		}
    volume_value = new_val;
		//preventing noise from i2c
		new_wiper_value = WIPER_MAX - volume_value;
    audio_changed = true;
    
}

void AUD_blank(void) {//just store chars
	  lcd_select(LCD_CTRL1);
    lcd_store_symbol(0, Speaker);
    lcd_store_symbol(1, StartMark);
    lcd_store_symbol(2, EmptyBar);
    lcd_store_symbol(3, Bar);
    lcd_store_symbol(4, EndMark);
    lcd_gotoxy(0,0);
		lcd_puts("Vol.");
		lcd_gotoxy(4,0);
		lcd_puts("----------------");
}

void AUD_init(void) {
	
		GPIO_WriteLow(MUTE_OUT_PIN);
		
	  DS1841_init();
		encoder2_counter=6;
    volume_value = WIPER_MAX / 2;  // approx half
    DS1841_setWiper(volume_value); // wiper volume
    AUD_DrawBar(AUD_WiperToSegments(volume_value));
}

// Main loop updater // maybe needs fix
void AUD_update(void) {
	  uint8_t enc_val;
    uint8_t new_wiper;
		uint8_t step;
		Encoder2_Update();
    enc_val = Encoder_GetValue(ENCODER2); 
    new_wiper = AUD_EncoderToWiper(enc_val);

    if (new_wiper != volume_value) {
			  uint8_t seg;
        AUD_SetVolume(new_wiper);
        seg = AUD_WiperToSegments(new_wiper);
        AUD_DrawBar(seg);
    }
}

// Button to mute function
void AUD_mute(void) {
    Encoder_Reset(ENCODER2);
    AUD_SetVolume(0);
    AUD_DrawBar(0);
}
	