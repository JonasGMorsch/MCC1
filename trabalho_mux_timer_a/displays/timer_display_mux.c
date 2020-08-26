#include <msp430.h>
#include <stdint.h>

#include "timer_display_mux.h"

#ifdef COM_ANODO
const uint8_t convTable[] = { 0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78,
								0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E };
#endif

volatile uint16_t _display_data = 0;

void timer_display_mux_init()
{
	/* Configuração de portas */
	DISPLAYS_MUX_PORT_DIR = 0xff;
	DISPLAYS_MUX_PORT_OUT = 0;

	DISPLAY_PORT_DIR = 0xff;
	DISPLAY_PORT_OUT = 0;

	//WDTCTL = WDT_ADLY_1_9;      //WD TIMER TO X TIME
	/*WDTCTL = WDT_ADLY_250;      //WD TIMER TO X TIME
	IE1 |= WDTIE;               //WD IRQ ENABLE*/
	//P1DIR |= LED_DEBUG;

	TA0CTL = TASSEL_2 | MC_2 | ID_1 | TAIE; 	//8M/2/2^16 = 61Hz
}

void timer_display_mux_write(uint16_t data)
{
	_display_data = data;
}

void __inline timer_display_worker()
{
	static int8_t counter;
	static uint16_t data;

	DISPLAYS_MUX_PORT_OUT &= ~(1 << counter); // Turn off last mux pin

	counter--;
	if (counter < 0)
	{
		counter = NUN_OF_DISPLAYS - 1;
		data = _display_data;
	}

	DISPLAYS_MUX_PORT_OUT |= (1 << counter);
	DISPLAY_PORT_OUT = convTable[data & 0x0f];
	data = data >> 4;
}

/*#pragma vector=WDT_VECTOR
__interrupt void timer_display_mux_isr(void)
{
	timer_display_worker();
}
*/

// ISR1 do Timer A: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
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
		timer_display_worker();

	    P1OUT ^= BIT6;
	    P1OUT ^= BIT6; // clock measurement
		break;
	default:
		break;
	}
}
