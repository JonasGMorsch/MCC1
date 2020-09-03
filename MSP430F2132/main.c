#include <msp430.h> 


//https://www.ti.com/product/MSP430F2132

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	return 0;
}
