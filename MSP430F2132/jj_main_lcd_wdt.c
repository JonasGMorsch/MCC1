#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#include "jonas/lcd.h"
#include "jonas/delay_wdt.h"

void main(void)
{

    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;


	init_watchdog_timer();
    lcd_init_4bits();

    P3DIR = 0xFF;

    uint8_t i = 0;
    char string[8];


    while (1)
    {
        P3OUT ^= BIT1;

        lcd_send_data(LCD_LINE_0, LCD_CMD);
        lcd_write_string("    Tarefa 3    ");
        lcd_send_data(LCD_LINE_1+7, LCD_CMD);

        snprintf(string, 8, "%d", i);
        lcd_write_string(string);
        i++;

        __delay_wdt_ms(500); // Sleep Cpu for %00ms
    }
}
