#include "av2026.h"
//helper
void AV2026_write_regs(uint8_t regAddr, uint8_t* data, uint8_t length)
{
    uint8_t buf[16];
    uint8_t i;

    buf[0] = regAddr;

    for (i = 0; i < length; i++)
        buf[i + 1] = data[i];

    swi2c_write_array(AV2026_ADDR, buf, length + 1);
}
//main logic
void AV2026_setFrequency(float frec, uint32_t bb_sym)
{
    static uint8_t tuner_initial = 0;
		uint8_t i;
		uint32_t p1;
		uint32_t p2;
		
		uint32_t freq_khz;
    uint8_t reg[50];
    uint8_t reg_start;
    uint32_t fracN;
    uint32_t BW;
    uint32_t BF;
    uint8_t r;
    uint8_t auto_scan = 0;

    if (bb_sym == 0 || bb_sym == 45000)
        auto_scan = 1;

    if (tuner_initial == 0)
    {
        for (i = 0; i < 42; i++)
            reg[i] = initRegs[i];

        reg_start = 0;
        AV2026_write_regs(reg_start, reg, 12);
        delay_ms(1);

        reg_start = 13;
        AV2026_write_regs(reg_start, reg + 13, 12);

        reg_start = 25;
        AV2026_write_regs(reg_start, reg + 25, 11);

        reg_start = 36;
        AV2026_write_regs(reg_start, reg + 36, 6);

        delay_ms(1);

        reg_start = 12;
				AV2026_write_regs(reg_start, reg + 12, 1);

        delay_ms(100);
				
        reg_start = 0;
        AV2026_write_regs(reg_start, reg, 12);
        delay_ms(1);

        reg_start = 13;
        AV2026_write_regs(reg_start, reg + 13, 12);

        reg_start = 25;
        AV2026_write_regs(reg_start, reg + 25, 11);

        reg_start = 36;
        AV2026_write_regs(reg_start, reg + 36, 6);

        delay_ms(1);

        reg_start = 12;
        AV2026_write_regs(reg_start, reg + 12, 1);

        delay_ms(4);

        tuner_initial = 1;
    }
		
		frec=frec-FREQ_OFFSET;//calculate IF offset to act as IF, not as zero IF
    freq_khz = (uint32_t)(frec * 1000);//convert float to khz
    fracN = (freq_khz + AV2026_XTAL / 2) / AV2026_XTAL;
    if (fracN > 0xFF) fracN = 0xFF;

    reg[0] = fracN & 0xFF;
		//handle too large number with two 32 bit uints
		p1 = freq_khz / AV2026_XTAL;
		p2 = freq_khz % AV2026_XTAL;

		fracN = (p1 << 17) + ((p2 << 17) / AV2026_XTAL);
    fracN = fracN & 0x1FFFF;
		
    reg[1] = ((fracN >> 9) & 0xFF);
    reg[2] = ((fracN >> 1) & 0xFF);
    reg[3] = (((fracN << 7) & 0x80) | 0x50);

    if (auto_scan)
    {
        reg[5] = 0xA3;
    }
    else
    {    
        BW = bb_sym * 135 / 200;

        if (bb_sym < 6500)
            BW = BW + 6000;

        BW = BW + 2000;
        BW = BW * 108 / 100;

        if (BW < 4000)  BW = 4000;
        if (BW > 40000) BW = 40000;

        BF = (BW * 127 + 21100/2) / (21100);
        reg[5] = (uint8_t)BF;
    }
    AV2026_write_regs(0, reg, 4);
    AV2026_write_regs(5, reg + 5, 1);
    if (!auto_scan)
    {
        reg[37] = 0x06;
        AV2026_write_regs(37, reg + 37, 1);

        reg[12] = 0x96;
        AV2026_write_regs(12, reg + 12, 1);
    }
}
