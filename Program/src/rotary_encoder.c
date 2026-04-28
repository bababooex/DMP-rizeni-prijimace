#include "rotary_encoder.h"

static int16_t enc1_delta = 0;
int16_t encoder2_counter; 
int16_t encoder1_counter;

//limits based on encoder steps
#define UPPER_LIMIT 12
#define LOWER_LIMIT 0

int16_t Encoder1_GetDelta(void)//fixes problems with sharing encoder
{
    int16_t delta;
		delta = encoder1_counter;//basically comparing difference for each device, so it should be good
    encoder1_counter = 0;
    return delta;
}
//init, in my application, it could be both at the same time, but whatever
void Encoder_Init(EncoderId id)
{
    switch(id)
    {
        case ENCODER1:
						GPIO_Init(ENC1_PORT, ENC1_A_PIN | ENC1_B_PIN, GPIO_MODE_IN_PU_NO_IT);
						
            break;
        case ENCODER2:
            GPIO_Init(ENC2_PORT, ENC2_A_PIN | ENC2_B_PIN, GPIO_MODE_IN_PU_NO_IT);
						
            break;
    }
}
//encoder for two modes
void Encoder1_Update(void) {
    uint8_t currentState = 0;
		static uint8_t lastState = 0;			
    delay_us(100);

    if (GPIO_ReadInputPin(ENC1_PORT, ENC1_A_PIN)) currentState |= 0x01;
    if (GPIO_ReadInputPin(ENC1_PORT, ENC1_B_PIN)) currentState |= 0x02;
		//stupid, but works
    //clockwise rotation
    if ((lastState == 0x00 && currentState == 0x01) ||
        (lastState == 0x01 && currentState == 0x03) ||
        (lastState == 0x03 && currentState == 0x02) ||
        (lastState == 0x02 && currentState == 0x00)) {
        encoder1_counter--;
    }
    //counter-clockwise rotation
    else if ((lastState == 0x00 && currentState == 0x02) ||
             (lastState == 0x02 && currentState == 0x03) ||
             (lastState == 0x03 && currentState == 0x01) ||
             (lastState == 0x01 && currentState == 0x00)) {
        encoder1_counter++;
    }
		
    lastState = currentState;
}
//encoder for audio
//tim1 updates automatically, but sw requires polling or interrupt
void Encoder2_Update(void) {
    static uint8_t lastState = 0; 
    uint8_t currentState = 0;
					
    delay_us(100);

    if (GPIO_ReadInputPin(ENC2_PORT, ENC2_A_PIN)) currentState |= 0x01;
    if (GPIO_ReadInputPin(ENC2_PORT, ENC2_B_PIN)) currentState |= 0x02;
		
    //clockwise rotation
    if ((lastState == 0x00 && currentState == 0x01) ||
        (lastState == 0x01 && currentState == 0x03) ||
        (lastState == 0x03 && currentState == 0x02) ||
        (lastState == 0x02 && currentState == 0x00)) {
        encoder2_counter--;
    }
    //counter-clockwise rotation
    else if ((lastState == 0x00 && currentState == 0x02) ||
             (lastState == 0x02 && currentState == 0x03) ||
             (lastState == 0x03 && currentState == 0x01) ||
             (lastState == 0x01 && currentState == 0x00)) {
        encoder2_counter++;
    }
		
    lastState = currentState;
		//prevent overflow
		if (encoder2_counter>UPPER_LIMIT){
				encoder2_counter=UPPER_LIMIT;
		}
		else if (encoder2_counter<LOWER_LIMIT){
				encoder2_counter=LOWER_LIMIT;
		}
}
//getting encoder values
int16_t Encoder_GetValue(EncoderId id)
{
    if (id == ENCODER1) {
			  return encoder1_counter;
    } else {
        return encoder2_counter;
    }
}

//encoder reset
void Encoder_Reset(EncoderId id)
{
    if (id == ENCODER1)
				encoder1_counter = 0;
    else
        encoder2_counter = 0;
}
