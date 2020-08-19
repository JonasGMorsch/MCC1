#ifndef _TECLADO_H
#define _TECLADO_H


#include <msp430.h>
#include <stdint.h>

#include "../lib/bits.h"


#define KB_PORT_DIR P2DIR
#define KB_PORT_OUT P2OUT
#define KB_PORT_REN P2REN
#define KB_PORT_IN  P2IN


void init_matrixkb();

uint8_t matrixkb();


#endif
