/*
 * 02_main_display_7_seg.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */
#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#include "displays/simple_display_mux.h"

#define LED_DEBUG   BIT5
#define ENCODER_INPUT  BIT7

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
volatile uint8_t d;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disable Watchdog
    BCSCTL3 |= LFXT1S_2; // ACLK = 32768Hz
    //P1DIR ^= LED_DEBUG;
    P1DIR |= LED_DEBUG;

    display_mux_init(); //Display setup
    config_ext_irq();

    while(1) //If this goes, the code goes with it
    {
        //display_mux_write(0x456);
        for(d=0; d<40; d++)
            display_mux_write(x);
        __bis_SR_register(LPM4_bits);
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
    x++;
    d=0; // IF X CHANGE D STARTS COUNTING AGAIN
    P1IFG &= ~ENCODER_INPUT; //Clear IRQ queue
    __bic_SR_register_on_exit(LPM4_bits); //Disable Low power 4
}
