/*
 * 03_main_ext_irq_encoder.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>
#include <stdint.h>

#define LED_DEBUG   BIT0
#define ENCODER_INPUT  BIT7

volatile uint16_t pulses = 0;

void config_ext_irq()
{
    P2DIR = LED_DEBUG | BIT1;
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



void main()
{
    WDTCTL = WDTPW | WDTHOLD; //stop WDT

    P3DIR = 0xFF;

    //__bis_SR_register(LPM4_bits); //turn off cpu

    /* Este laço nunca é executado nesse exemplo */
    while (1)
    {
        //(...)

        P2OUT ^= BIT1;
        _delay_cycles(100000);

    }

}


/* Port 1 ISR (interrupt service routine) */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    /* Liga/desliga LED quando detectado borda no encoder */
    P3OUT ^= LED_DEBUG;

    /* Conta o número de pulsos */
    pulses++;

    /* Limpa sinal de IRQ do botão 0 */
    P2IFG &= ~ENCODER_INPUT;

    /* Essa linha deve ser descomentada para o main executar */
    //  __bic_SR_register_on_exit(LPM4_bits);
}
