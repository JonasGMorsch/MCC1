/*
 * 01_main_simple_blink.c
 *
 *  Created on: Feb 17, 2020
 *      Author: xtarke
 */

#include <msp430.h>

#define LED   BIT0
#define DELAY 5000

int main(void)
{
    int i;

    /* Configuração de hardware */
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;

    /* main não pode retornar */
    while(1)
    {
        /* Liga/Desliga LED */
        P1OUT = P1OUT ^ LED;

        _delay_cycles(1000000);
    }

    return 0;
}
