#include <msp430.h> 

#include <jonas/bateria.h>
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

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	init_clock_system();
	init_adc();

	TA0CTL = TASSEL_2 | MC_2 | ID_3 | TAIE; 	//8M/8/2^16 = 15.26Hz

	SET_BIT(P1DIR, BIT5);
	SET_BIT(P1DIR, BIT6);

	while (1)
	{
		if (get_vbat_top() < 641 || get_vbat_bottom() < 641)
			SET_BIT(P1OUT, BIT5);

		else
			CLR_BIT(P1OUT, BIT5);

		__bis_SR_register(LPM4_bits); //Sleep CPU
	}
}

// ISR1 do Timer A: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
	switch (__even_in_range(TA0IV, 0x0A))
	{
	// Vector  0:  No interrupt
	case TA0IV_NONE:
		break;
		// Vector  2:  TACCR1 CCIFG -> Comparação 1
	case TA0IV_TACCR1:
		break;
		// Vector  4:  TACCR2 CCIFG -> Comparação 2
	case TA0IV_TACCR2:
		break;
		// Vector  6:  Reserved
	case TA0IV_6:
		break;
		// Vector  8:  Reserved
	case TA0IV_8:
		break;
		// Vector 10:  TAIFG -> Overflow do timer 0
	case TA0IV_TAIFG:

		ADC10CTL0 |= ENC + ADC10SC;

		P1OUT ^= BIT6; // clock measurement
		break;
	default:
		break;
	}
}

