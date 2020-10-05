/*
 * 09_main_i2c_scan.c
 *
 *  Created on: May 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 *
 *      - Escreve apenas um byte no barramento I2C.
 *      - Exemplo útil para expansor de IO PCF8574T.
 *                          .   .
 *                         /|\ /|\
 *               CI_xyz    10k 10k     MSP430G2xx3
 *              -------     |   |   -------------------
 *             |    SDA|<  -|---+->|P1.7/UCB0SDA       |-
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |       |    |      |                   |
 *             |    SCL|<----+-----|P1.6/UCB0SCL       |
 *              -------            |                   |
 *
 */

/* System includes */
#include <msp430.h>
#include <stdint.h>


/* Project includes */
#include "lib/bits.h"
#include "lib/i2c_master_f2132.h"

/* Define endereço de 7-bits I2C
 * 0x27 é o endereço do PCF8574T -- NXP
 */
#define DEVICE_I2C_ADDR 0x20 // A0 A1 A2 TO GROUND

/**
 * @brief  Configura sistema de clock para usar o Digitally Controlled Oscillator (DCO).
 *         Utililiza-se as calibrações internas gravadas na flash.
 *         Exemplo baseado na documentação da Texas: msp430g2xxx3_dco_calib.c  *
 * @param  none
 *
 * @retval none
 */
void init_clock_system()
{

#ifdef CLOCK_1MHz
	/* Se calibração foi apagada, para aplicação */
	if (CALBC1_1MHZ==0xFF)
	while(1);
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
#endif

#ifdef CLOCK_8MHz

	/* Se calibração foi apagada, para aplicação */
	if (CALBC1_8MHZ==0xFF)
	while(1);

	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	/* Outras fonte de clock devem ser configuradas também *
	 * de acordo com a aplicação  */
#endif

#ifdef CLOCK_12MHz
	/* Se calibração foi apagada, para aplicação */
	if (CALBC1_12MHZ==0xFF)
	while(1);
	DCOCTL = 0;
	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL = CALDCO_12MHZ;
#endif

#ifdef CLOCK_16MHz
	/* Se calibração foi apagada, para aplicação */
	if (CALBC1_16MHZ == 0xFF)
		while (1)
			;
	DCOCTL = 0;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
#endif
}

void init_watchdog_timer()
{
	WDTCTL = WDT_ADLY_1000;      //WD TIMER TO X TIME
	BCSCTL3 |= LFXT1S_2;		// Configura ACLK para usar VLO: ~10kHZ*/

	//WDTCTL = WDT_MDLY_0_5;

	IE1 |= WDTIE;               //WD IRQ ENABLE
}

int main(void)
{

	uint8_t i = 0;
	//i2c_mode i2c_status = IDLE_MODE;

	/* Desliga Watchdog */
	WDTCTL = WDTPW | WDTHOLD;

	init_clock_system();
	init_i2c_master_mode();
	init_watchdog_timer();

	/* Debug LED */
	P1DIR |= BIT1;
	CLR_BIT(P1OUT, BIT1);

	while (1)
	{

		//__bis_SR_register(GIE);


		//if (i2c_status == NACK_MODE)

		//__delay_cycles(16000000);

		//	__bis_SR_register(GIE);

		if (i % 2 == 0)
		{
			/*i2c_status = */i2c_write_single_byte(DEVICE_I2C_ADDR, 0xFF);

			CLR_BIT(P1OUT,BIT1);

		}
		else
		{
			/*i2c_status = */i2c_write_single_byte(DEVICE_I2C_ADDR, 0x00);

			SET_BIT(P1OUT, BIT1);
		}

		i++;
		__bis_SR_register(LPM4_bits);
		//__delay_cycles(1000000);

	}
}

#pragma vector=WDT_VECTOR
__interrupt void delay_wdt_isr(void)
{

	// CPL_BIT(P1OUT,BIT1);
	__bic_SR_register_on_exit(LPM4_bits); // wake cpu

}

