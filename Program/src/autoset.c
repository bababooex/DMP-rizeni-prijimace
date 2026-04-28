#include "autoset.h"
//default constants - for UHF TV channel, it is 471.25, so 471.2 and it is channel 21, others calculated
const float baseFreq = 471.2;
const uint8_t baseChannel = 21;
const uint8_t step = 8; // default tv channel step - 8 MHz for analog UHF TV

void autoset_Modulator(void){
    int32_t offset;
		if (frec >= 460.0 && frec <= 870.0) {  //valid channel range
        offset = (int32_t)((frec - baseFreq) / step + ((frec - baseFreq) >= 0 ? 0.5 : -0.5)); //calculate offset
				channel = baseChannel + offset;
				//valid channel range
				if (channel < 21) channel = 21;
				if (channel > 69) channel = 69;
    } else {
        return;
    }
}

void autoset_Tuner(void) {
    if (channel >= 21 && channel <= 69) {
        frec = baseFreq + step * (channel - baseChannel); 
				frec1 = (uint16_t)frec; 
				frec2 = (int8_t)((frec - frec1) * 10 + 0.5);
				//valid frequency range (but there should be no problem so just in case)
				if (frec < 44.0) frec = 44.0;
				if (frec > 870.0) frec = 870.0;		
    } else {
        return;
    }
}