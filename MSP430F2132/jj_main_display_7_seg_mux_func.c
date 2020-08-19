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

volatile uint16_t pulses = 0;

void config_ext_irq()
{
    //CLR_BIT(P2DIR,ENCODER_INPUT);
    P2DIR &= ~(ENCODER_INPUT);
    P2REN = ENCODER_INPUT;
    P2OUT = ENCODER_INPUT;

    /* Habilitação da IRQ apenas botão */
    P2IE = ENCODER_INPUT;
    /* Transição de nível alto para baixo */
    P2IES = ENCODER_INPUT;
    /* Transição de nível baixo para alto*/
    //P2IES = !ENCODER_INPUT;

    /* Limpa alguma IRQ pendente */
    P2IFG &= ~ENCODER_INPUT;

    /* Habilita IRQs e desliga CPU */
    //__bis_SR_register(LPM4_bits | GIE);

    __bis_SR_register(GIE); //int_enable
}

volatile uint16_t x = 0x0123;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Disable Watchdog
    BCSCTL3 |= LFXT1S_2; // VLO to ~10kHZ

    P1DIR |= 0xFF;
    P3DIR = 0xFF;

    /* Inicializa displays */
    display_mux_init();
    config_ext_irq();

    while(1)
    {
        //display_mux_write(0x456);
        display_mux_write(x);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void ext_iqr(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(asdsd))) dgsfg (void)
#else
#error Compiler not supported!
#endif
{
    /* Liga/desliga LED quando detectado borda no encoder */
    P1OUT ^= LED_DEBUG;

    x++;

    /* Limpa sinal de IRQ do botão 0 */
    P2IFG &= ~ENCODER_INPUT;

    /* Essa linha deve ser descomentada para o main executar */
    //  __bic_SR_register_on_exit(LPM4_bits);
}
