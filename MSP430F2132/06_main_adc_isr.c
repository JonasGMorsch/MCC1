/*
 * 06_main_adc_isr.c
 *
 *  Created on: May 15, 2020
 *      Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      Exemplo do conversor analógico digital.
 *      - Trigger do ADC por software.
 *      - Temporização em função do main.
 *
 *                  MSP430G2553
 *               -----------------
 *           /|\|              XIN|-
 *            | |                 |
 *            --|RST          XOUT|-
 *              |                 |
 *  LCD_RS <--  | P1.0    P1.2 A2 | <-- Potenciômetro
 *  LCD_E  <--  | P1.1            |
 *              |                 |
 * LCD_DATA <-- | P2.[0..3]       |
 */

#include <msp430.h>
/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>
#include <stdio.h>

#include "lib/gpio.h"
#include "lib/bits.h"
#include "displays/lcd.h"

/*#ifndef __MSP430G2553__
#error "Clock system not supported for this device"
#endif*/

volatile uint16_t adc_val = 0;

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
	if (CALBC1_8MHZ == 0xFF)
		while (1)
		{
		}

	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	BCSCTL3 |= LFXT1S_2; //32768Hz
}


/**
  * @brief  Configura conversor analógico digital (ADC10).
  *
  * @param  none
  *
  * @retval none
  */
void init_adc(){

    /* ADC10CTL0:
     * ADC10SHT_2:  16 x ADC10CLKs
     * ADC10ON   :  ADC10 On/Enable
     * ADC10IE   :  IRQ Enable
     */
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;

    /* Input A2 */
    ADC10CTL1 = INCH_2;

    /*  P1.2 ADC option select */
    ADC10AE0 |= BIT2;
}

int main (void)
{
    char string[8];

    /* Desliga Watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Sistema de clock */
    init_clock_system();
    init_adc();

    /* Pino de debpuração */
    SET_BIT(P1DIR,BIT5 | BIT6);


    /* Inicializa hardare: veja lcd.h para
     * configurar pinos */
    lcd_init_4bits();
    /* Escreve string */

    lcd_send_data(LCD_LINE_1, LCD_CMD);
    lcd_write_string("MSP430");

    _delay_cycles(100000);

    while (1){
        /* Início da conversão: trigger por software */
        ADC10CTL0 |= ENC + ADC10SC;

        /* Desliga CPU até ADC terminar
         * Nesse exemplo o LCD é muito mais lento */
        __bis_SR_register(GIE);

        /* Remove caracteres antigos do LCD */
      /*  //if (adc_val < 1000)
            lcd_send_data(' ', LCD_DATA);
        //if (adc_val < 1000)
            lcd_send_data(' ', LCD_DATA);
        //if (adc_val < 10)
            lcd_send_data(' ', LCD_DATA);

        lcd_send_data(LCD_LINE_0, LCD_CMD);
        snprintf(string, 8, "%d", adc_val);

        lcd_write_string(string);*/

        //snprintf(string, 8, "%d", adc_val);
        if(adc_val > 641)
        {
        	SET_BIT(P1OUT, BIT5);
        }
        else
        {
        	CLR_BIT(P1OUT, BIT5);
        }
    }
}



/* ISR do ADC10. Executada quando a conversão terminar */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    adc_val = ADC10MEM;

    /* Complementa P1.6 para ver valocidade do ADC */
    CPL_BIT(P1OUT,BIT6);

    //__bic_SR_register_on_exit(CPUOFF);
}

