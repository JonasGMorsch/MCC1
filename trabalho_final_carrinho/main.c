#include <msp430.h>
#include <stdint.h>

#include "jonas/ultrassom.h"
#include <jonas/bateria.h>
#include "jonas/motor.h"

#include "lib/uart_f2132.h"
#include "lib/gpio.h"
#include "lib/bits.h"

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

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	init_clock_system();
	config_timerA_1_ultrassom();
	motor_init();
	set_motor_pwm(100);
	init_adc();

	WDTCTL = WDT_ADLY_250;      //WD TIMER TO X TIME
	IE1 |= WDTIE;               //WD IRQ ENABLE

	P3DIR |= BIT2;  //BAT_PROTECTION
	P3DIR |= BIT3; //US_TRIGGER

	__bis_SR_register(GIE);

	while (1)
	{
		///////////////////// Battery Protection ////////////////
		if (get_vbat_top() < 641 || get_vbat_bottom() < 641)
			SET_BIT(P3OUT, BIT2);

		else
			CLR_BIT(P3OUT, BIT2);
		////////////////////////////////////////////////////////

		/////////////////// Ultrasound Check //////////////////
		if (get_us1_cm() < 10) //left sensor
		{
			motor_move(RIGHT);

		}
		else if (get_us2_cm() < 10) //right sensor
		{
			motor_move(LEFT);
		}
		////////////////////////////////////////////////////////

		/////////////////// UART Comunication /////////////////
		uint8_t uart_command = 0;
		uart_receive_package((uint8_t *) uart_command, 1);

		//__bis_SR_register(CPUOFF | GIE);

		if (uart_command == 'F')
		{
			motor_move(FOWARD);
		}
		else if (uart_command == 'B')
		{
			motor_move(BACKWARD);
		}
		else if (uart_command == 'R')
		{
			motor_move(RIGHT);
		}
		else if (uart_command == 'L')
		{
			motor_move(LEFT);
		}
		else if (uart_command == 'S')
		{
			motor_move(STOP);
		}
		////////////////////////////////////////////////////////

		__bis_SR_register(LPM4_bits);
	}
}

#pragma vector=WDT_VECTOR
__interrupt void delay_wdt_isr(void)
{
	P3OUT |= BIT3;
	_delay_cycles(80); // 10us trigger
	P3OUT &= ~BIT3;

	ADC10CTL0 |= ENC + ADC10SC; // check ADC values
}
