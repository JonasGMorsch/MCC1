#include <msp430.h>
#include <stdint.h>

/* Project includes */
#include "lib/uart_f2132.h"
#include "lib/bits.h"
#include "jonas/motor.h"

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
	char my_data[8];

	/* Desliga Watchdog */
	WDTCTL = WDTPW + WDTHOLD;

	/* Inicializa hardware */
	init_clock_system();
	init_uart();

	motor_init();
	set_motor_pwm(50);

	/* Led de depuração */
	P1DIR |= BIT0;

	while (1)
	{
		uart_receive_package((uint8_t *) my_data, 4);

		__bis_SR_register(CPUOFF | GIE);

		for (uint8_t i=0 ; sizeof(my_data) > i; i++)
		{
			if(my_data[i] == 'F')
			{
				motor_move(FOWARD);
			}
			if(my_data[i] == 'B')
			{
				motor_move(BACKWARD);
			}
			if(my_data[i] == 'R')
			{
				motor_move(RIGHT);
			}
			if(my_data[i] == 'L')
			{
				motor_move(LEFT);
			}
		}
		CPL_BIT(P1OUT, BIT0);
	}
}
