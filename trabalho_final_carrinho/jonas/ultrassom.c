#include <jonas/ultrassom.h>
#include <msp430.h>
#include <stdint.h>

//https://lastminuteengineers.com/arduino-sr04-ultrasonic-sensor-tutorial/


volatile uint16_t ultrassom_1_rise_timestamp = 0;
volatile uint8_t ultrassom_1_cm = 0;

volatile uint16_t ultrassom_2_rise_timestamp = 0;
volatile uint8_t ultrassom_2_cm = 0;

uint8_t get_us1_cm()
{
	return ultrassom_1_cm;
}

uint8_t get_us2_cm()
{
	return ultrassom_2_cm;
}

void config_timerA_1_ultrassom()
{
	/* Timer tipo A:
	 *
	 * CAP ->  Capture mode: 1
	 * CM_3 ->  Capture mode: 1 - both edges
	 * SCS -> Capture sychronize
	 * CCIS_0 -> Capture input select: 0 - CCIxA
	 * CCIE -> Capture/compare interrupt enable
	 *
	 * Configura comparador 0 do timer 1 para captura */

	TA1CCTL0 = CAP | CM_3 | SCS | CCIS_1 | CCIE;
	TA1CCTL1 = CAP | CM_3 | SCS | CCIS_0 | CCIE;

	TA1CTL = TASSEL0 | MC1 | TACLR | TAIE; //32768Hz = 30.51 uS //LESS PROTEUS CPU USAGE

	P3DIR &= ~(BIT6 | BIT7);
	P3SEL |= BIT6 | BIT7;

	// 3.6 Timer1_A2.CCI0B
	// 3.7 Timer1_A2.CCI1A
}

/* ISR0 do Timer A: executado no evento de comparação de captura  */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
	if (TA1CCTL0 & CCI)
	{
		ultrassom_1_rise_timestamp = TA1CCR0;
	}
	else
	{
		//343 * 100 / 32768 =1.04675cm/us ~= 1cm/us
		// ms *  cm / s^-1
		ultrassom_1_cm = (TA1CCR0 - ultrassom_1_rise_timestamp) >> 1;
		__bic_SR_register_on_exit(LPM4_bits);
	}

	if (TA1CCTL1 & CCI)
	{
		ultrassom_2_rise_timestamp = TA1CCR1;
	}
	else
	{
		//343 * 100 / 32768 =1.04675cm/us ~= 1cm/us
		// ms *  cm / s^-1
		ultrassom_2_cm = (TA1CCR1 - ultrassom_2_rise_timestamp) >> 1;
		__bic_SR_register_on_exit(LPM4_bits);
	}
}

