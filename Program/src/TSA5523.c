#include "tsa5523.h"
#include "extern_variables.h"

uint8_t db1, db2, cb, pb;
uint8_t datatun[4];
uint8_t datatun2[2];
uint16_t Nt;
uint32_t freq_khz;

bool fm_band_flag;

static const uint8_t tuner_chargepump[4] = {0x10, 0x00, 0x40};

void tuner_setFrequency(float frec) {

#ifdef FM_MODE
    fm_band_flag = false;
#endif

    if (frec < 44.0) {
        frec = 44.0;
    } else if (frec > 870.0) {
        frec = 870.0;
    }

    freq_khz = (uint32_t)(frec * 1000);

#ifdef FM_MODE
    if (freq_khz >= 87500 && freq_khz <= 108000) {
        if (!fm_band_flag) {
            uint8_t dummy_tv[4] = {0x20, 0x00, 0xCA, 0xA0};
            swi2c_write_array(TSA5523_ADDR, dummy_tv, 4);//needs dummy before correct fm mode
            delay_ms(5);
            fm_band_flag = true;
        }

        freq_khz += 10700;
        Nt = freq_khz / 50;
        pb = 0xA4;        
        cb = 0x88 | tuner_chargepump[chargepump];
    }
    else
		#endif
    {
#ifdef FM_MODE
        fm_band_flag = false;
#endif
				//handle frequency jumps -> set different mixer in tuner
        if (freq_khz < 165000)
            pb = 0xA0;
        else if (freq_khz < 446000)
            pb = 0x90;
        else
            pb = 0x30;

        Nt = freq_khz / 31.25 + 1248; // TV IF offset for 39MHz
        cb = 0xCA | tuner_chargepump[chargepump];
    }

    db1 = (Nt >> 8) & 0x7F;
    db2 = Nt & 0xFF;

    datatun[0] = db1;
    datatun[1] = db2;
    datatun[2] = cb;
    datatun[3] = pb;

    swi2c_write_array(TSA5523_ADDR, datatun, 4);
}


// charge pump logic
// works after first start and then changing global variable
void tuner_setChargepump(tuner_CP_settings chargepump) {
	  if (chargepump > CP_250uA) chargepump = CP_OFF;
		
    datatun2[0] = cb | tuner_chargepump[chargepump];
    datatun2[1] = pb;
    swi2c_write_array(TSA5523_ADDR, datatun2, 2);
}

// Read status byte from tuner
uint8_t tuner_getStatus(void) {
		uint8_t status = 0;
    if (swi2c_read_array(TSA5523_ADDR, &status, 1) == 0)
        return status;
    else
        return 0xFF; // comm error -> refer to stat and conf
}


// Put tuner to sleep, not sure about this chonky boi
void tuner_sleep(void) {
    db1 = db2 = 0x00;
    cb  = 0xCA | 0x01;
    pb  = 0x01;

    datatun[0] = db1;
    datatun[1] = db2;
    datatun[2] = cb;
    datatun[3] = pb;

    swi2c_write_array(TSA5523_ADDR, datatun, 4);
}
