#ifndef _DELAY_WTD_H
#define _DELAY_WTD_H

#include <msp430.h>
#include <stdint.h>
#include "../lib/bits.h"

#define WDC_STEP 2

void init_watchdog_timer(void);

void __delay_wdt_ms(uint16_t);

void delay_wdt_isr(void);

#endif // _DELAY_WTD_H
