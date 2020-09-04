#include <jonas/ultrassom.h>
#include <msp430.h>
#include <stdint.h>

//https://lastminuteengineers.com/arduino-sr04-ultrasonic-sensor-tutorial/

#define US_TRIGGER BIT0
#define US_TRIGGER_PORT P1OUT
#define US_TRIGGER_DIR P1DIR
#define US_TRIGGER_SEL P1SEL

#define US_CAPTURE BIT1

volatile uint16_t ultrassom_rise_timestamp = 0;
volatile uint8_t ultrassom_cm = 0;

uint8_t get_cm()
{
	return ultrassom_cm;
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

	TA1CCTL0 = CAP | CM_3 | SCS | CCIS_0 | CCIE;

	//TA1CTL = TASSEL_2 | MC_2 | TACLR | TAIE;  //8MHZ = 125 nS
	TA1CTL = TASSEL0 | MC1 | TACLR | TAIE; //32768Hz = 30.51 uS //LESS PROTEUS CPU USAGE

	US_TRIGGER_DIR |= US_TRIGGER;
	US_TRIGGER_SEL |= US_CAPTURE;

}

/* ISR0 do Timer A: executado no evento de comparação de captura  */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
	if (TA1CCTL0 & CCI)
	{
		ultrassom_rise_timestamp = TA1CCR0;
	}
	else
	{
		//343 * 100 / 32768 =1.04675cm/us ~= 1cm/us
		// ms *  cm / s^-1
		ultrassom_cm = (TA1CCR0 - ultrassom_rise_timestamp) >> 1;
		__bic_SR_register_on_exit(LPM4_bits);
	}
}

/* ISR1 do Timer A: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2 */
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
	switch (__even_in_range(TA1IV, 0x0A))
	{
	/* Vector  0:  No interrupt */
	case TA1IV_NONE:
		break;

		/* Vector  2:  TACCR1 CCIFG -> Comparação 1*/
	case TA1IV_TACCR1:
		//US_TRIGGER_PORT ^= US_TRIGGER;
		break;

		/* Vector  6:  Reserved  */
	case TA1IV_6:
		break;

		/* Vector  8:  Reserved  */
	case TA1IV_8:
		break;

		/* Vector 10:  TAIFG -> Overflow do timer 0*/
	case TA1IV_TAIFG:
		//US_TRIGGER_PORT ^= US_TRIGGER;
		break;

	default:
		break;
	}
}
