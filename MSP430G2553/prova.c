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

#include "displays/led_display.h"
#include "lib/bits.h"

#define NUN_OF_DISPLAYS 3

#define LED_DEBUG   BIT0
#define ENCODER_INPUT  BIT4

volatile uint16_t pulses = 0;


void config_ext_irq()
{
    //CLR_BIT(P2DIR,ENCODER_INPUT);
    P2DIR &= ~ENCODER_INPUT;
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

volatile uint16_t x = 0x1234;

void main(void)
{
    /* Para o watchdog timer
     * NecessÃ¡rio para cÃ³digo em depuraÃ§Ã£o */
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= 0xFF;
    P3DIR = 0xFF;




    uint16_t y;
    uint32_t z;

    /* Inicializa displays */
    //display_init();
    config_ext_irq();

    //P1OUT = 1;
    while(1)
    {
        if(z==10)
        {
            z=0;
            //x++;
        }
        z++;

 /*       P1OUT = 1 << (NUN_OF_DISPLAYS - 1);
        y = x;
        for(uint8_t i=0; i<NUN_OF_DISPLAYS; i++)
        {
            display_write(y);
            y = y >> 4;
            _delay_cycles(1000);
            P1OUT = P1OUT >> 1;
        }
    }*/
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
    //pulses++;
    x++;

    /* Limpa sinal de IRQ do botão 0 */
    P2IFG &= ~ENCODER_INPUT;

    /* Essa linha deve ser descomentada para o main executar */
    //  __bic_SR_register_on_exit(LPM4_bits);
}
