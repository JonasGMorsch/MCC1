#include <lib/uart_g2553.h>
#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/bits.h"

/*
 #ifndef __MSP430G2553__
 #error "Clock system not supported for this device"
 #endif
 */

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

#ifdef CLOCK_16MHz
	//Se calibração foi apagada, para aplicação
	if (CALBC1_16MHZ == 0xFF)
		while (1)
			;
	DCOCTL = 0;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
#endif

}

int main()
{
	uint8_t message[] = "R";

	uint8_t test[] = "RR";
	//const char message_bin_data[] = { 65, 63, 87, 87};

	char my_data[8];

	/* Desliga Watchdog */
	WDTCTL = WDTPW + WDTHOLD;

	/* Inicializa hardware */
	init_clock_system();
	init_uart();

	/* Led de depuração */
	P1DIR |= BIT0;

	while (1)
	{
		/* Configura o recebimento de um pacote de 4 bytes */
		//uart_receive_package((uint8_t *)my_data, 4);
		/* Desliga a CPU enquanto pacote não chega */
		//__bis_SR_register(CPUOFF | GIE);
		/* Envia resposta */
		//uart_send_package((uint8_t *)message,sizeof(message));
		//uart_send_package((uint8_t *)message,sizeof(message));

		//uart_send_data_pooling(test[0]);
		/*
		 uart_send_package((uint8_t *)message,sizeof(message));


		 uart_send_package((uint8_t *)message,sizeof(message));*/

		/* Pisca LED para sinalizar que dados chegaram */
		CPL_BIT(P1OUT, BIT0);
	}
}
