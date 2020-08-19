#include <msp430.h>
#include <stdio.h>
#include "displays/lcd.h"
#include "jonas/teclado.h"

void main()
{
    WDTCTL = WDTPW | WDTHOLD; //Disable Watchdog

    lcd_init_4bits();
    init_matrixkb();

    uint8_t ddram_adress = LCD_LINE_0;
    uint8_t new_key = 0xFF;
    uint8_t past_key = 0xFF;

    while (1)
    {
        new_key = matrixkb();
        if (new_key != 255)
        {
            if ((new_key != past_key) && (ddram_adress != 208) && (new_key != 10) && (new_key != 12) && (new_key != 255))
            {
                past_key = new_key;
                lcd_send_data(ddram_adress, LCD_CMD);
                ddram_adress++;
                if(ddram_adress == 144)
                    ddram_adress = LCD_LINE_1;
                lcd_send_data('0'+ new_key , LCD_DATA);
            }
            if (new_key == 10 || new_key == 12)
            {
                lcd_send_data(LCD_CLEAR, LCD_CMD);
                ddram_adress = LCD_LINE_0;
            }
        }
        else
            past_key = new_key;
    }
}
