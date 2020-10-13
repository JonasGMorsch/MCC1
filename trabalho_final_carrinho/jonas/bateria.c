#include <jonas/bateria.h>
#include <msp430.h>
#include <stdint.h>

volatile uint16_t vbat_top = 2000;
volatile uint16_t vbat_bottom = 1000;

uint16_t get_vbat_top()
{
	return vbat_top;
}

uint16_t get_vbat_bottom()
{
	return vbat_bottom;
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

// ISR do ADC10. Executada quando a conversão terminar
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	switch (ADC10CTL1)
	{
	case INCH_0:
		ADC10CTL1 = INCH_1;
		vbat_top = (ADC10MEM << 1) - vbat_bottom;
		break;

	case INCH_1:
		ADC10CTL1 = INCH_0;
		vbat_bottom = ADC10MEM;
		break;
	}

	//P1OUT ^= BIT7; // clock measurement
	ADC10CTL0 &= ~ENC; //RECUCES PROTEUS CPU USAGE BY ABOUT 4X    //__bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
    __bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
}
