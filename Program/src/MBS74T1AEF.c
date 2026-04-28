#include "mbs74t1aef.h"

uint16_t freq;
uint8_t fm, fl;
uint8_t datamod[4];
uint8_t datamod2[2];
uint8_t status;
uint16_t N;
uint8_t divider, X;
// audio constants
static const uint8_t rfmod_sound[4] = { 0x00, 0x08, 0x10, 0x18 };

// fm / fl byte helper, ensure valid channel range, also TPEN enable
static void rfmod_setFreqBytes(uint8_t *fm, uint8_t *fl, uint8_t channel, bool testMode) {
    if (channel < 21) {
        channel = 21; 
    }
		else if (channel > 69) {
        channel = 69; 
    }
    //function to change frequency to coresponding channel
    freq = 32 * channel + 1213; //equation - sets correct dividers - not really mine, some smartie figured it out
    *fm = (freq >> 6) & 0xFF;
    if (testMode) {
        *fm |= 0x40; 
    }
    *fl = (freq & 0x3F) << 2;
}

// Start the modulator on given channel with sound + test mode
void rfmod_start(uint8_t channel, rfmod_sound_system sound, bool testMode) {
    
    if (sound > DK_6_5MHZ) sound = MN_4_5MHZ;

    rfmod_setFreqBytes(&fm, &fl, channel, testMode);

    datamod[0] = 0x88;                        
    datamod[1] = 0x40 | rfmod_sound[sound];   
    datamod[2] = fm;
    datamod[3] = fl;

    swi2c_write_array(RFMOD_ADDR, datamod, 4);
}

// finally real sleep mode, not tsa5523 "fake"
void rfmod_sleep(void) {
    datamod2[0] = 0xA8;
    datamod2[1] = 0x20;
    swi2c_write_array(RFMOD_ADDR, datamod2, 2);
}

// change channel while running
void rfmod_changeChannel(uint8_t channel, bool testMode) {

    rfmod_setFreqBytes(&fm, &fl, channel, testMode);

    datamod2[0] = fm;
    datamod2[1] = fl;

    swi2c_write_array(RFMOD_ADDR, datamod2, 2);
}

// sound carrier change
void rfmod_setSoundCarrier(rfmod_sound_system sound) {

    if (sound > DK_6_5MHZ) sound = MN_4_5MHZ;

    datamod2[0] = 0x88;
    datamod2[1] = 0x40 | rfmod_sound[sound];

    swi2c_write_array(RFMOD_ADDR, datamod2, 2);
}

// status byte check
uint8_t rfmod_getStatus(void) {
		uint8_t status = 0;
    if (swi2c_read_array(RFMOD_ADDR, &status, 1) == 0)
        return status;
    else
        return 0xFF; // comm error -> refer to stat and conf
				
}

// experimental free range function through dividers
//WARNING, THIS MODE CREATES HARMONICS, DONT AMPLIFY! CAN DISRUPT COMMUNICATION, PROCEED WITH CAUTION!//
static void rfmod_setFreqRawBytes(float freqMHz, uint8_t *fm, uint8_t *fl, uint8_t *X) {

    if (freqMHz < 30.0) {
        freqMHz = 30.0;
		}
		else if (freqMHz > 1023.0) {
        freqMHz = 1023.0;
		}
		//dividing main oscillator -> results in smaller rf output but enables wider range
    if (freqMHz > 423.0) { divider = 1; *X = 0; }
    else if (freqMHz > 210.0) { divider = 2; *X = 1; }
    else if (freqMHz > 105.0) { divider = 4; *X = 2; }
    else if (freqMHz > 52.0)  { divider = 8; *X = 3; }
    else                      { divider = 16; *X = 4; }

    N = (uint16_t)(freqMHz * 4 * divider);

    *fm = (N >> 6) & 0x3F;
    *fl = (N & 0x3F) << 2;
    *fl |= (*X & 0x03);
}
void rfmod_setFrequencyRaw(float freqMHz, rfmod_sound_system sound, bool testMode) {
		if (sound > DK_6_5MHZ) sound = BG_5_5MHZ;
    rfmod_setFreqRawBytes(freqMHz, &fm, &fl, &X);
    datamod[0] = 0x88;
		if (X & 0b100) datamod[0] |= 0x02;
    datamod[1] = 0x40 | rfmod_sound[sound];
    if (testMode) fm |= 0x40;
    datamod[2] = fm;
    datamod[3] = fl;
    swi2c_write_array(RFMOD_ADDR, datamod, 4);
}
