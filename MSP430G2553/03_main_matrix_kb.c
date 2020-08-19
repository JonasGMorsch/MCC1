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

#define PORT_DIR P2DIR
#define PORT_OUT P2OUT
#define PORT_REN P2REN

void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    PORT_DIR = 0b00001111;
    //PORT_DIR = 0xFF;
    //PORT_DIR = 0x0;

    PORT_REN= 0xFF;
    PORT_OUT= 0xFF;
    display_init();

    while(1)
    {
        //PORT_OUT = 0x1111111;
        //PORT_OUT = !BIT4 & 0xFF;
        //PORT_OUT = BIT2

        //PORT_OUT = 0b11111111;

        uint8_t m = matrixkb();
        if(m != 255)
            display_write(m);


    }
}

uint8_t matrixkb()
{
    uint8_t c = 1;

    for(uint8_t i=0; i<4; i++)
    {
        PORT_OUT = ~(1 << i);
        _delay_cycles(2000); //proteus!!
        for(uint8_t j=0; j<3; j++)
        {
            if (TST_BIT(P2IN, (uint8_t)(1 << (j+4))))
                c++;
            else
            {
                if(c < 13)
                {
                    if(c==11)
                        return 0;
                    else
                        return c;
                }
                else
                    return 255;
            }
        }
    }
    return 255;
}
