/*
 * 01_main_simple_gpio.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>

#include "lib/bits.h"

#define LED_1   BIT7
#define LED_2   BIT6
#define BUTTON  BIT0

/**
  * @brief  Configura hardware.
  * @param  Nenhum
  *
  * @retval Nenhum.
  */

/**
  * @brief  main: todo programa C deve ter!
  * @param  Nenhum
  *
  * @retval Nenhum.
  */
int main(void)
{
    /* Para o watchdog timer
     * Necess�rio para c�digo em depura��o */
    WDTCTL = WDTPW | WDTHOLD;

    //P1DIR =

    /* Acesso direto: P1.0 e P1.6 como sa�das. Demais como entrada */
    P1DIR |= LED_1 | LED_2;

    /* Habilita resistor de pull up ou pull down */
    P3REN |= BUTTON;
    /* Habilita resitor como pull up */
    P3OUT |= BUTTON;

    /* La�o infinito */
    while(1){

        if (TST_BIT(P3IN, BUTTON)) /* Equivalente a: if (P1IN & BIT3) */
            CLR_BIT(P1OUT, LED_1 | LED_2);    /* Equivalente a P1OUT &= ~(BIT0 | BIT6); */
        else
            SET_BIT(P1OUT, LED_1 | LED_2);    /* Equivalente a: P1OUT |= BIT0 | BIT6; */
    }

}
