/*
 * 02_simple_display_mux.c
 *
 *  Created on: Feb 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *
 *      Exemplo de aplica√ß√£o:
 *
 *      - Utiliza o WatchDog Timer
 *      para multiplexar 4 displays de 7 segmentos.
 *      - Utiliza IRQ externa para contar n√∫mero
 *      de pulsos exibindo-os no display.
 *
 *
 */

#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#include "lib/gpio.h"
#include "displays/watchdog_display_mux.h"

volatile uint16_t i = 0;

#define PULSES  BIT0


#define LED_DEBUG   BIT6
#define ENCODER_INPUT  BIT7

void config_ext_irq()
{
    //CLR_BIT(P2DIR,ENCODER_INPUT);
    P1DIR &= ~(ENCODER_INPUT);
    P1REN = ENCODER_INPUT;
    P1OUT = ENCODER_INPUT;

    /* HabilitaÁ„o da IRQ apenas bot„o */
    P1IE = ENCODER_INPUT;
    /* TransiÁ„o de nÌvel alto para baixo */
    P1IES = ENCODER_INPUT;
    /* TransiÁ„o de nÌvel baixo para alto*/
    //P1IES = !ENCODER_INPUT;

    /* Limpa alguma IRQ pendente */
    P1IFG &= ~ENCODER_INPUT;

    /* Habilita IRQs e desliga CPU */
    //__bis_SR_register(LPM4_bits | GIE);

    __bis_SR_register(GIE); //int_enable
}


/**
  * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
  *         Utililiza-se as calibra√ß√µes internas gravadas na flash.
  *         Exemplo baseado na documenta√ß√£o da Texas: msp430g2xxx3_dco_calib.c
  *         Configura ACLK para utilizar VLO = ~10KHz
  * @param  none
  *
  * @retval none
  */
void init_clock_system(){

    /* Configura√ß√£o do MCLK em 8MHz */

    /* Se calibra√ß√£o foi apagada, para aplica√ß√£o */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}

void main(void)
{
    /* Para o watchdog timer
     * Necess√°rio para c√≥digo em depura√ß√£o */
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= LED_DEBUG;

    init_clock_system();
    config_ext_irq();

    /* Inicializa displays */
    watchdog_display_mux_init();

    while(1)
    {
    	__bis_SR_register(LPM4_bits); // CPU Sleep Low Power 4
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
	P1OUT ^= LED_DEBUG;
	//watchdog_display_mux_write(i++);
	watchdog_display_mux_write(0x123);
    P1IFG &= ~ENCODER_INPUT; //Clear IRQ queue
    __bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
}
