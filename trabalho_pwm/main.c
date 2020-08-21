#include <msp430.h>
#include <stdint.h>

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

void config_timerA_as_pwm()
{
	TA0CCR0 = 8064; // 8mhz/8k = 1khz

	TA0CCTL1 = OUTMOD_7;
	TA0CCR1 = 4000;
	TA0CCTL2 = OUTMOD_7;
	TA0CCR2 = 4500;

	TA0CTL = TASSEL_2 | MC_1 | ID_0; //8mhz Up to CCR0

	//TA0CTL = TASSEL_2 | MC_2 | ID_3; //8mhz Up to CCR0

	/*
	 TA1CCR0 = 10000;

	 TA1CCTL1 = OUTMOD_2;
	 TA1CCR1 = 40000;

	 TA1CTL = TASSEL_2 | MC_3 | ID_0;
	 */
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
	/* Disable the GPIO power-on default high-impedance mode */
	PM5CTL0 &= ~LOCKLPM5;
#endif

	init_clock_system();
	config_timerA_as_pwm();

	P1DIR = 0xFF;
	P1SEL = BIT2 | BIT3;

	P3DIR = 0xFF;

	while (1)
	{
		P3OUT ^= BIT1;
		_delay_cycles(8000000);
		P3OUT ^= BIT0;

		//__bis_SR_register(LPM4_bits); //(SCG1+SCG0+OSCOFF+CPUOFF)
	}
}
