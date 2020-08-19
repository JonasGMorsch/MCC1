/*
 * 05_main_simple_timer_a.c
 *
 *  Created on: Mar 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo de configuração Timer Tipo A número 0
 *
 *
 *       .
 *      /|\                  +    <-- Overflow (TA0IV_TAIFG - TIMER0_A1_VECTOR)   (2^16)
 *       |                 +
 *       |               +
 *       |             +  <-- Comparador 0 (TACCR0  -> TIMER0_A0_VECTOR)
 *       |           +
 *       |         +
 *       |       + <--- Comparadores 1 e 2 (TACCR1 e TACCR2 - TIMER0_A1_VECTOR)
 *       |     +
 *       |   +
 *       | +
 *       +----------------------------------------->
 *
 *       - Comparadores podem ser configurados para qualquer valor
 *       entre 0 e 65535. IRQs devem ser habilitadas individuais
 *       nos respectivos registradores.
 *
 */


/* System includes */
#include <msp430.h>

/* Project includes */
#include "lib/gpio.h"

#define LED_1 BIT0
#define LED_2 BIT1

#define LED_PORT P3

/**
  * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
  *         Utililiza-se as calibrações internas gravadas na flash.
  *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c
  *         Configura ACLK para utilizar VLO = ~10KHz
  * @param  none
  *
  * @retval none
  */
void init_clock_system()
{
    /* Configuração do MCLK em 8MHz */
    /* Se calibração foi apagada, para aplicação */
    if (CALBC1_8MHZ==0xFF)
        while(1);

    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    /* Configura ACLK para usar VLO: ~10kHZ */
    BCSCTL3 |= LFXT1S_2;
}

/**
  * @brief  Configura temporizador A.
  *
  * @param  none
  *
  * @retval none
  */
void config_timerA_0()
{
    /* Timer A0:
     *
     * TASSEL_2 -> Clock de SMCLK.
     * MC_2 -> Contagem crescente.
     * TAIE -> Habilitação de IRQ.
     * ID_2 -> Prescaler = /2
     */
    TA0CTL = TASSEL_2 | MC_2 | ID_1 | TAIE;

    // IRQ por comparação entre contagem e comparador 0
    //TA0CCTL0 = CCIE;


    //TA0CCTL0 = COV;

    // Valor de comparação é 50000
    //TA0CCR0 = 20000;

    /* TA0CCTL1 = CCIE;
    TA0CCR1 = 200;

    TA0CCTL2 = CCIE;
    TA0CCR1 = 500; */
}


int main(void)
{
    /* Desliga watchdog imediatamente */
    WDTCTL = WDTPW | WDTHOLD;

    /* Configurações de hardware */
    config_timerA_0();
    init_clock_system();

    PORT_DIR(LED_PORT) = LED_1 | LED_2;
    PORT_OUT(LED_PORT) = 0;

    /* Entra em modo de economia de energia com IRQs habilitadas */
    __bis_SR_register(GIE);

    while(1)
    {
    	__bis_SR_register(LPM4_bits); // CPU Sleep Low Power 4
    }
}

/*
// ISR0 do Timer A: executado no evento de comparação  comparador 0 (TACCR0)
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    PORT_OUT(LED_PORT) ^= LED_2;
}
*/

// ISR1 do Timer A: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA0IV,0x0A))
  {
      // Vector  0:  No interrupt
      case  TA0IV_NONE:
          break;

      // Vector  2:  TACCR1 CCIFG -> Comparação 1
      case  TA0IV_TACCR1:
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
          PORT_OUT(LED_PORT) ^= LED_1;

          break;
      default:
          break;
  }
}

