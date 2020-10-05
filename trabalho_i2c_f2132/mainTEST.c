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

void init_watchdog_timer()
{
	WDTCTL = WDT_ADLY_1000;      //WD TIMER TO X TIME
	BCSCTL3 |= LFXT1S_2;		// Configura ACLK para usar VLO: ~10kHZ*/

	//WDTCTL = WDT_MDLY_0_5;

	IE1 |= WDTIE;               //WD IRQ ENABLE
}

int main(void)
{

	uint8_t i = 0;
	//i2c_mode i2c_status = IDLE_MODE;

	/* Desliga Watchdog */
	WDTCTL = WDTPW | WDTHOLD;

	init_clock_system();
	init_i2c_master_mode();
	///init_watchdog_timer();


	/*lcd_init_4bits();
	lcd_send_data(LCD_LINE_1 + 5, LCD_CMD);
	lcd_write_string("MSP430");
*/
	/* Debug LED */
	P1DIR |= BIT1;
	CLR_BIT(P1OUT, BIT1);

	while (1)
	{
		lcd_send_data(LCD_LINE_1 + 5, LCD_CMD);
		lcd_write_string("MSP430");
		__bis_SR_register(LPM4_bits);


		if (i % 2 == 0)
		{
			/*i2c_status =*/
			 i2c_write_single_byte(DEVICE_I2C_ADDR, 0xFF);

			 CLR_BIT(P1OUT, BIT1);

			//SET_BIT(i2c_port, E_PIN);
			//i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

		}
		else
		{
			/*i2c_status =*/

			 i2c_write_single_byte(DEVICE_I2C_ADDR, 0x00);

			SET_BIT(P1OUT, BIT1);


			//CLR_BIT(i2c_port,E_PIN);
			//i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);
		}

		i++;
		//__bis_SR_register(LPM4_bits);
		//__delay_cycles(1000000);



	}
}

#pragma vector=WDT_VECTOR
__interrupt void delay_wdt_isr(void)
{

	// CPL_BIT(P1OUT,BIT1);
	__bic_SR_register_on_exit(LPM4_bits); // wake cpu

}

