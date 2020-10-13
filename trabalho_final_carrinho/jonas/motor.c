#include <msp430.h>
#include <stdint.h>

#include "motor.h"
#include "lib/bits.h"

void motor_init()
{
	TA0CCR0 = 100; // 8mhz/8k = 1khz

	TA0CCTL1 = OUTMOD_7;
	TA0CCR1 = 0;
	TA0CCTL2 = OUTMOD_7;
	TA0CCR2 = 0;

	//TA0CTL = TASSEL_2 | MC_1 | ID_3;  //8MHZ/1000/8 = 1ms

	TA0CTL = TASSEL_1 | MC_1 ;  //32768hz/100 = 3.051ms


	MOTOR_DIRECTION_DIR |= (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT);
	MOTOR_PWM_PORT_DIR = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
}

void set_motor_pwm(uint16_t pwm)
{
	TA0CCR1 = pwm;
	TA0CCR2 = pwm;
}

void motor_move(uint8_t direction)
{
	switch (direction)
	{
	case STOP:
		MOTOR_PWM_PORT_SEL &= ~(MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		MOTOR_PWM_PORT_OUT &= ~(MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case FOWARD:
		MOTOR_DIRECTION_OUT |= (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT);
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case F_RIGHT:
		MOTOR_PWM_PORT_SEL |= MOTOR_PWM_RIGHT;
		MOTOR_PWM_PORT_SEL &= ~MOTOR_PWM_LEFT;
		MOTOR_DIRECTION_OUT |= MOTOR_DIRECTION_RIGHT;
		break;

	case RIGHT:
		MOTOR_DIRECTION_OUT |= MOTOR_DIRECTION_RIGHT;
		MOTOR_DIRECTION_OUT &= ~MOTOR_DIRECTION_LEFT;
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case B_RIGHT:
		MOTOR_PWM_PORT_SEL |= MOTOR_PWM_LEFT;
		MOTOR_PWM_PORT_SEL &= ~MOTOR_PWM_RIGHT;
		MOTOR_DIRECTION_OUT &= ~(MOTOR_DIRECTION_LEFT);
		break;

	case BACKWARD:
		MOTOR_DIRECTION_OUT &= ~(MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT);
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case B_LEFT:
		MOTOR_PWM_PORT_SEL |= MOTOR_PWM_RIGHT;
		MOTOR_PWM_PORT_SEL &= ~MOTOR_PWM_LEFT;
		MOTOR_DIRECTION_OUT &= ~(MOTOR_DIRECTION_RIGHT);
		break;

	case LEFT:
		MOTOR_DIRECTION_OUT |= MOTOR_DIRECTION_LEFT;
		MOTOR_DIRECTION_OUT &= ~MOTOR_DIRECTION_RIGHT;
		MOTOR_PWM_PORT_SEL = (MOTOR_PWM_LEFT | MOTOR_PWM_RIGHT);
		break;

	case F_LEFT:
		MOTOR_PWM_PORT_SEL |= MOTOR_PWM_LEFT;
		MOTOR_PWM_PORT_SEL &= ~MOTOR_PWM_RIGHT;
		MOTOR_DIRECTION_OUT |= MOTOR_DIRECTION_LEFT;
		break;

	default:
		break;
	}

}
