#include <msp430.h> 

#include "lib/gpio.h"
#include "lib/bits.h"

volatile uint16_t vbat_top = 2000;
volatile uint16_t vbat_botton = 1000;

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

void init_adc()
{
	//https://www.ti.com/lit/ds/symlink/msp430f2132.pdf

	//ADC10CTL0:
	//ADC10SHT_2:  16 x ADC10CLKs
	//ADC10ON   :  ADC10 On/Enable
	//ADC10IE   :  IRQ Enable
	//SREF1 // REF+ on P2.4 and REF-=VSS
	//ADC10SR 50 kbps
	ADC10CTL0 |= SREF1 + ADC10ON + ADC10IE;

	// Input A2
	ADC10CTL1 = INCH_1; //START ON BOTTON BATTERY

	//P2.2 ADC option select
	ADC10AE0 = BIT0 | BIT1;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	init_clock_system();
	init_adc();
	TA0CTL = TASSEL_2 | MC_2 | ID_3 | TAIE; 	//8M/8/2^16 = 15.26Hz

	SET_BIT(P1DIR, BIT4);
	SET_BIT(P1DIR, BIT5);
	SET_BIT(P1DIR, BIT6);
	SET_BIT(P1DIR, BIT7);

	while (1)
	{
		if (vbat_top < 641 || vbat_botton < 641)
		{
			SET_BIT(P1OUT, BIT5);
		}
		else
		{
			CLR_BIT(P1OUT, BIT5);
		}
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

		P1OUT ^= BIT6;
		//P1OUT ^= BIT6; // clock measurement
		break;
	default:
		break;
	}
}

// ISR do ADC10. Executada quando a conversão terminar
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	switch (ADC10CTL1)
	{
	case INCH_0:
		ADC10CTL1 = INCH_1;
		vbat_top = (ADC10MEM * 2) - vbat_botton;
		break;

	case INCH_1:
		ADC10CTL1 = INCH_0;
		vbat_botton = ADC10MEM;
		break;
	}

	P1OUT ^= BIT7; // clock measurement
	ADC10CTL0 &= ~ENC; //RECUCES PROTEUS CPU USAGE BY ABOUT 4X    //__bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
    __bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
}
