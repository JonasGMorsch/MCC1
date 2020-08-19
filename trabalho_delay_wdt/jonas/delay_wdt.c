#include "../lib/gpio.h"
#include "delay_wdt.h"

volatile uint16_t remainig_millis = 0;

void init_watchdog_timer()
{
    WDTCTL = WDT_ADLY_1_9;      //WD TIMER TO X TIME
    BCSCTL3 |= LFXT1S_2;		// Configura ACLK para usar VLO: ~10kHZ*/

    //WDTCTL = WDT_MDLY_0_5;

    IE1 |= WDTIE;               //WD IRQ ENABLE
}

void __delay_wdt_ms(uint16_t t)
{
    remainig_millis = t;
    if (remainig_millis >= WDC_STEP)
        __bis_SR_register(LPM4_bits); // Low Power 4
}

#pragma vector=WDT_VECTOR
__interrupt void delay_wdt_isr(void)
{
    P3OUT ^= BIT0;
    P3OUT ^= BIT0;

    if (remainig_millis <= WDC_STEP)
        __bic_SR_register_on_exit(LPM4_bits); // wake cpu
    else
        remainig_millis -= WDC_STEP;
}
