#include <msp430.h>
#include <stdint.h>

#include "displays/lcd.h"
#include "lib/gpio.h"
#include "lib/bits.h"
#include "jonas/ultrassom.h"

void init_clock_system()
{
	if (CALBC1_8MHZ == 0xFF)
		while (1)
		{
		}

	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	BCSCTL3 |= LFXT1S_2; //32768Hz
}

void main(void)
{
	/* Para o watchdog timer
	 * Necessário para código em depuração */
	WDTCTL = WDTPW | WDTHOLD;

	init_clock_system();
	lcd_init_4bits();
	config_timerA_1_ultrassom();

	WDTCTL = WDT_ADLY_250;      //WD TIMER TO X TIME
	IE1 |= WDTIE;               //WD IRQ ENABLE

	P1DIR |= BIT0;

	while (1)
	{
		lcd_send_data(LCD_LINE_0, LCD_CMD);
		lcd_send_data('0' + get_cm() / 100 % 10, LCD_DATA);
		lcd_send_data('0' + get_cm() / 10 % 10, LCD_DATA);
		lcd_send_data('0' + get_cm() % 10, LCD_DATA);
		lcd_write_string("cm");

		__bis_SR_register(LPM4_bits);
	}
}

#pragma vector=WDT_VECTOR
__interrupt void delay_wdt_isr(void)
{
	P1OUT |= BIT0;
	_delay_cycles(80); // 10us
	P1OUT &= ~BIT0;
}

