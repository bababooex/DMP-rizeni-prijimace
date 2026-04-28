#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H
#include "stm8s.h"
#include "stm8s_delay.h"
#include "extern_variables.h"
//encoder choice based on movement
typedef enum {
    ENCODER1 = 0,   
    ENCODER2        
} EncoderId;
//for second encoder
#define ENC2_A_PIN   GPIO_PIN_7
#define ENC2_B_PIN   GPIO_PIN_4
#define ENC2_PORT    GPIOG

#define ENC1_A_PIN   GPIO_PIN_1
#define ENC1_B_PIN   GPIO_PIN_2
#define ENC1_PORT    GPIOC

/**
 * @brief functions regarding this sw/hw rotary encoder control
 * - init encoder tim handler
 * - get value of rotary encoder
 * - reset rotary encoder value (debug etc.)
 */
void Encoder_Init(EncoderId id);
int16_t Encoder_GetValue(EncoderId id); //basic getter function, that gets two counters, only used for audio in this instance
int16_t Encoder1_GetDelta(void); //new function, that is made to work with two modes, each handles difference separatelly
void Encoder_Reset(EncoderId id);
void Encoder2_Update(void);
void Encoder1_Update(void);
#endif // ROTARY_ENCODER_H



