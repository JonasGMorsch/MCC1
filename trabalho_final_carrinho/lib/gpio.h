/*
 * gpio.h : GPIO general purpose macros
 *
 *  Created on: Mar 2, 2020
 *      Author: xtarke
 */
#ifndef _GPIO_H_
#define _GPIO_H_

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>
#include "lib/bits.h"

/* Convert Px to PxOUT */
#define PORT_OUT(...) PORT_OUT_SUB(__VA_ARGS__)
#define PORT_OUT_SUB(port) (port##OUT)

/* Convert Px to PxDIR */
#define PORT_DIR(...) PORT_DIR_SUB(__VA_ARGS__)
#define PORT_DIR_SUB(port) (port##DIR)

/*
void _delay_us(uint16_t us)
{
    while (us)
    {
        __delay_cycles(1); // 1 for 1 Mhz set 16 for 16 MHz
        us--;
    }
}

void _delay_ms(uint16_t ms)
{
    while (ms)
    {
        __delay_cycles(1000); //1000 for 1MHz and 16000 for 16MHz
        ms--;
    }
}

*/
#endif /* LIB_GPIO_H_ */
