#include <msp430.h>
#include <stdint.h>

#include "jonas/motor.h"

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
	WDTCTL = WDTPW | WDTHOLD;


	init_clock_system();
	motor_init();

	set_motor_pwm(50);

	while (1)
	{
		motor_move(FOWARD);
		_delay_cycles(2000000);

		motor_move(F_RIGHT);
		_delay_cycles(2000000);

		motor_move(RIGHT);
		_delay_cycles(2000000);

		motor_move(B_RIGHT);
		_delay_cycles(2000000);

		motor_move(BACKWARD);
		_delay_cycles(2000000);

		motor_move(B_LEFT);
		_delay_cycles(2000000);

		motor_move(LEFT);
		_delay_cycles(2000000);

		motor_move(F_LEFT);
		_delay_cycles(2000000);

		motor_move(STOP);
		_delay_cycles(8000000);

		__bis_SR_register(LPM4_bits); //(SCG1+SCG0+OSCOFF+CPUOFF)
	}
}
