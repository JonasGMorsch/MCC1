#include <msp430.h>
#include <stdint.h>

//#include "lib/bits.h"

void main(void)
{
    WDTCTL = WDT_ADLY_250;  //WD TIMER TO X TIME
    IE1 |= WDTIE;           //WD IRQ ENABLE
    BCSCTL3 |= LFXT1S_2;  // Configura ACLK para usar VLO: ~10kHZ
    __bis_SR_register(GIE);

    P3DIR = 0xFF;

    while (1)
    {
        for(uint8_t i=0; i<4; i++)
        {
            _delay_cycles(15000);
            P3OUT ^= BIT1;
        }
        __bis_SR_register(LPM4_bits); // CPU Sleep Low Power 4
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__) //WD IRQ FUNCTION
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    P3OUT ^= BIT0;

    __bic_SR_register_on_exit(LPM4_bits); // Disable Low Power 4 on Exit
}
