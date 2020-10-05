/* System includes */
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/bits.h"
#include "lib/lcdi2c.h"

void init_clock_system()
{

#ifdef CLOCK_16MHz
	if (CALBC1_16MHZ == 0xFF)
		while (1)
			;
	DCOCTL = 0;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
#endif
}

int main(void)
{
	/* Desliga Watchdog */
	WDTCTL = WDTPW | WDTHOLD;

	init_clock_system();
	init_i2c_master_mode();
	lcd_init_4bits();

	P1DIR |= BIT0 | BIT1;
	CLR_BIT(P1OUT, BIT0 | BIT1);

	while (1)
	{

		lcd_send_data(LCD_LINE_1 + 5, LCD_CMD);
		lcd_write_string("MSP430");
		SET_BIT(P1OUT, BIT0);

		__bis_SR_register(LPM4_bits);
	}
}
