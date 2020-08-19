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

#define NUN_OF_DISPLAYS 3

void main(void)
{
    /* Para o watchdog timer
     * Necessário para código em depuração */
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= 0xFF;

    //volatile uint16_t i;
	//uint8_t x[4] = {1,2,3,4};
    uint16_t x = 0x1234;
    uint16_t y;
    uint32_t z;

	/* Inicializa displays */
	display_init();
	P1OUT = 1;
	while(1)
	{
		if(z==10)
		{
			z=0;
			x++;
		}
		z++;

		P1OUT = 1 << (NUN_OF_DISPLAYS - 1);
		y = x;
		for(uint8_t i=0; i<NUN_OF_DISPLAYS; i++)
		{
			display_write(y);
			y = y >> 4;
			_delay_cycles(1000);
			P1OUT = P1OUT >> 1;
		}
	}
}
