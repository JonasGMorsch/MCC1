#include <msp430.h>
#include <stdint.h>
#include "displays/timer_display_mux.h"

#define LED_DEBUG   BIT6
#define ENCODER_INPUT  BIT7

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

void config_ext_irq()
{
    //CLR_BIT(P2DIR,ENCODER_INPUT);
    P1DIR &= ~(ENCODER_INPUT);
    P1REN = ENCODER_INPUT;
    P1OUT = ENCODER_INPUT;

    /* Habilitação da IRQ apenas botão */
    P1IE = ENCODER_INPUT;
    /* Transição de nível alto para baixo */
    P1IES = ENCODER_INPUT;
    /* Transição de nível baixo para alto*/
    //P1IES = !ENCODER_INPUT;

    /* Limpa alguma IRQ pendente */
    P1IFG &= ~ENCODER_INPUT;

    /* Habilita IRQs e desliga CPU */
    //__bis_SR_register(LPM4_bits | GIE);

    __bis_SR_register(GIE); //int_enable
}

volatile uint16_t x = 0x0123;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disable Watchdog
    init_clock_system();
    timer_display_mux_init(); //Display setup
    config_ext_irq();

    while(1) //If this goes, the code goes with it
    {
        __bis_SR_register(LPM4_bits); //Sleep CPU
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void ext_iqr(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(asdsd))) dgsfg (void)
#else
#error Compiler not supported!
#endif
{
    x++;
    P1IFG &= ~ENCODER_INPUT; //Clear IRQ queue
    timer_display_mux_write(x);
    //__bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
}
