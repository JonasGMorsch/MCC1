#include <msp430.h>
#include <stdint.h>

#include "simple_display_mux.h"
#include "led_display.h"
#include "../lib/bits.h"

void display_mux_init()
{
    /* Configuração de portas */
    DISPLAYS_MUX_PORT_DIR = 0xff;
    DISPLAYS_MUX_PORT_OUT = 0;

    display_init();
}

void display_mux_write(uint16_t data)
{
    for(int8_t i=NUN_OF_DISPLAYS-1; i>=0; i--)
    {
        SET_BIT(P1OUT,1<<i);
        display_write(data);
        data = data >> 4;
        _delay_cycles(5000);
        CLR_BIT(P1OUT,1<<i);
    }
}
