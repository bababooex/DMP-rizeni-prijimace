#include "extern_variables.h"
//blink colon - mainly for visual appeal
bool blink_colon;

void flag_handler(void)//handle rtc clock flag, must be run in almost everything, to make it appear "behind" all
{
		if (tuner_start){ //display rtc on the start, than let flags do the job
			lcd_gotoxy(29, 3);
			lcd_puts("RTC:");
			display_time(34, 3);//display time in the beginning
			tim4_s_counter = 0;
		}
		lcd_gotoxy(36, 3);
		lcd_data(blink_colon ? ':' : ' ');//switch function - nice
		
    if (rtc_update_flag)//rtc update flag
    {
        rtc_update_flag = 0;
        display_time(34, 3);
    }
}