#include "PCF8563.h"

bool err;
extern struct {
    unsigned char s;
    unsigned char m;
    unsigned char h;
    unsigned char dy;
    unsigned char dt;
    unsigned char mt;
    unsigned char yr;
} time;

void PCF8563_init(void)//call after swi2c init
{
    PCF8563_write(PCF8563_CONTROL1, 0x00);
    PCF8563_write(PCF8563_CONTROL2, 0x00);
}

unsigned char PCF8563_read(unsigned char address)
{
    uint8_t value = 0x00;
    swi2c_read_buf(PCF8563_ADDR, address, &value, 1);
		return value;
}

void PCF8563_write(unsigned char address, unsigned char value)
{
    swi2c_write_buf(PCF8563_ADDR, address, &value, 1);
}

unsigned char bcd_to_decimal(unsigned char value)
{
    return ((value & 0x0F) + (((value >> 4) & 0x0F) * 10));
}

unsigned char decimal_to_bcd(unsigned char value)
{
    return (((value / 10) << 4) | (value % 10));
}

void PCF8563_get_time(void)
{
    uint8_t buf[7];
		if (swi2c_read_buf(PCF8563_ADDR, PCF8563_SEC_REG, buf, 7) != 0) {
        err=true;
    }
		else{
			  err=false;
		}
    time.s  = bcd_to_decimal(buf[0] & 0x7F);
    time.m  = bcd_to_decimal(buf[1] & 0x7F);
    time.h  = bcd_to_decimal(buf[2] & 0x3F);
    time.dt = bcd_to_decimal(buf[3] & 0x3F);
    time.dy = bcd_to_decimal(buf[4] & 0x07);
    time.mt = bcd_to_decimal(buf[5] & 0x1F);
    time.yr = bcd_to_decimal(buf[6]);
}

void PCF8563_set_time(void)
{
    uint8_t buf[7];

    buf[0] = decimal_to_bcd(time.s) & 0x7F;
    buf[1] = decimal_to_bcd(time.m) & 0x7F;
    buf[2] = decimal_to_bcd(time.h) & 0x3F;
    buf[3] = decimal_to_bcd(time.dt) & 0x3F;
    buf[4] = decimal_to_bcd(time.dy) & 0x07;
    buf[5] = decimal_to_bcd(time.mt) & 0x1F;
    buf[6] = decimal_to_bcd(time.yr);

    swi2c_write_buf(PCF8563_ADDR, PCF8563_SEC_REG, buf, 7);
}

void show_value(uint8_t x, uint8_t y, uint8_t value)
{
    char ch[3];
    ch[0] = (value / 10) + '0';
    ch[1] = (value % 10) + '0';
    ch[2] = '\0';

    lcd_gotoxy(x, y);
    lcd_puts(ch);
}

void display_time(uint8_t x_pos, uint8_t y_pos)
{
	  static uint8_t last_s = 255;
    PCF8563_get_time();
		if (err==true) {
        lcd_gotoxy(x_pos,y_pos);
				lcd_puts("--");
				lcd_gotoxy(x_pos + 3,y_pos);
				lcd_puts("--");
        return;
    }
		show_value(x_pos, y_pos, time.h);
		show_value(x_pos + 3, y_pos, time.m);
		//handled elsewhere
		/*
    if (time.s != last_s) { // trying to be nonblocking without interrupts
        show_value(x_pos, y_pos, time.h);
        //lcd_data(':');
        show_value(x_pos + 3, y_pos, time.m);
        //lcd_data(':'); //not using seconds, updates only by one minute
        //show_value(x_pos + 6, y_pos, time.s);
        last_s = time.s;
    }
		*/
}
//doesnt need fix, not used
void display_date(uint8_t x_pos, uint8_t y_pos)
{
		static uint8_t last_dt = 255;
    static uint8_t last_mt = 255;
    static uint8_t last_yr = 255;
    PCF8563_get_time();
    if (time.dt != last_dt || time.mt != last_mt || time.yr != last_yr) //same logic, if not changed, skip
    {
        show_value(x_pos, y_pos, time.dt);
        lcd_data('/');
        show_value(x_pos + 3, y_pos, time.mt);
        lcd_data('/');
        show_value(x_pos + 6, y_pos, time.yr);

        last_dt = time.dt;
        last_mt = time.mt;
        last_yr = time.yr;
    }
}
//set custom time - only once, then disable!
void custom_time(void)
{
    time.h  = 12;
    time.m  = 36;
    time.s  = 20;
    time.dt = 1;
    time.mt = 1;
    time.yr = 0;
    time.dy = 1;
		PCF8563_set_time();
}