#include <msp430.h>
#include <stdint.h>

#include "displays/lcd.h"
#include "lib/gpio.h"
#include "lib/bits.h"

void init_clock_system()
{
	if (CALBC1_8MHZ == 0xFF)
		while (1)
		{
		}

	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	BCSCTL3 |= LFXT1S_2; //32768Hz
}
/**
 * @brief  Configura temporizador A.
 *
 * @param  none
 *
 * @retval none
 */
void config_timerA_1()
{
	/* Timer tipo A:
	 *
	 * CAP ->  Capture mode: 1
	 * CM_3 ->  Capture mode: 1 - both edges
	 * SCS -> Capture sychronize
	 * CCIS_0 -> Capture input select: 0 - CCIxA
	 * CCIE -> Capture/compare interrupt enable
	 *
	 * Configura comparador 0 do timer 1 para captura */
	TA1CCTL0 = CAP | CM_3 | SCS | CCIS_0 | CCIE;

	/* TASSEL_2 ->  Timer A clock source select: 2 - SMCLK
	 * MC_2 ->  Timer A mode control: 2 - Continous up
	 * TACLR ->  Timer A counter clear
	 * TAIE ->  Timer A counter interrupt enable (overflow)
	 *
	 * Configuração da fonte do clock do timer 1 */
	//TA1CTL = TASSEL_2 | MC_2 | TACLR | TAIE;  //8MHZ = 125 nS

	TA1CTL = TASSEL_1 | MC_2 | TACLR | TAIE;  //32768Hz = 30.51 uS

}

/* Variáveis globais */
volatile uint16_t overflow = 0;
volatile uint16_t rise_timestamp = 0;
volatile uint16_t fall_timestamp = 0;

volatile uint32_t interval = 0;

void main(void)
{
	/* Para o watchdog timer
	 * Necessário para código em depuração */
	WDTCTL = WDTPW | WDTHOLD;

	init_clock_system();
	lcd_init_4bits();
	config_timerA_1();

	P1DIR = BIT0;
	P1SEL |= BIT1;

	SET_BIT(P3DIR, BIT0);


	while (1)
	{
		//lcd_send_data(LCD_LINE_1 + 5, LCD_CMD);
		//lcd_write_string("MSP430");
		__bis_SR_register(LPM4_bits);
	}
}

/* ISR0 do Timer A: executado no evento de comparação de captura  */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)

{
	if (TA1CCTL0 & CCI)
	{
		P1OUT ^= BIT0;
		rise_timestamp = TA1CCR0;

		overflow = 0;

		P1OUT ^= BIT0;
	}
	else
	{
		P1OUT ^= BIT0;
		fall_timestamp = TA1CCR0;

		interval = fall_timestamp - rise_timestamp;

		/*if (interval > 3000 * 8)
		{
			SET_BIT(P3OUT, BIT0);
		}
		else
		{
			CLR_BIT(P3OUT, BIT0);
		}*/


		lcd_send_data(LCD_LINE_0, LCD_CMD);

		//uint8_t string[12];
		//snprintf(string, 6, "%d", interval/8);
		//lcd_write_string(string);
/*
		lcd_send_data('0'+ interval/8000%10 , LCD_DATA);
		lcd_send_data('0'+ interval/800%10 , LCD_DATA);
		lcd_send_data('0'+ interval/80%10 , LCD_DATA);
		lcd_send_data('0'+ interval/8%10 , LCD_DATA);
		lcd_write_string("uS");
		lcd_send_data(' ' , LCD_DATA);
		lcd_send_data(' ' , LCD_DATA);*/

		/*interval*=30.51;

		lcd_send_data('0'+ interval/1000%10 , LCD_DATA);
		lcd_send_data('0'+ interval/100%10 , LCD_DATA);
		lcd_send_data('0'+ interval/10%10 , LCD_DATA);
		lcd_send_data('0'+ interval%10 , LCD_DATA);
		lcd_write_string("uS");
		lcd_send_data(' ' , LCD_DATA);
		//lcd_send_data(' ' , LCD_DATA);

		*/

		//343*100/32768 ~=1cm/us
		//ms *cm / 1/s

		interval/=2;
		lcd_send_data('0'+ interval/100%10 , LCD_DATA);
		lcd_send_data('0'+ interval/10%10 , LCD_DATA);
		lcd_send_data('0'+ interval%10 , LCD_DATA);
		lcd_write_string("cm");

	}

}

/* ISR1 do Timer A: executado toda a vez que o temporizador estoura, evento do comparador 1 ou evento do comparador 2 */
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
	switch (__even_in_range(TA1IV, 0x0A))
	{
	/* Vector  0:  No interrupt */
	case TA1IV_NONE:
		break;

		/* Vector  2:  TACCR1 CCIFG -> Comparação 1*/
	case TA1IV_TACCR1:

		break;
		/* Vector  4:  TACCR2 CCIFG -> Comparação 2*/
		//case TA1IV_TACCR2:
		//break;
		/* Vector  6:  Reserved  */
	case TA1IV_6:
		break;

		/* Vector  8:  Reserved  */
	case TA1IV_8:
		break;

		/* Vector 10:  TAIFG -> Overflow do timer 0*/
	case TA1IV_TAIFG:
		overflow++;
		//P1OUT ^= BIT0;

		break;
	default:
		break;
	}
}

/*
 const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
 const int echoPin = 6; // Echo Pin of Ultrasonic Sensor

 void setup()
 {
 Serial.begin(9600); // Starting Serial Terminal
 }

 void loop()
 {
 long duration, cm;
 pinMode(pingPin, OUTPUT);
 digitalWrite(pingPin, LOW);
 delayMicroseconds(2);
 digitalWrite(pingPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(pingPin, LOW);

 pinMode(echoPin, INPUT);
 duration = pulseIn(echoPin, HIGH);
 cm = microsecondsToCentimeters(duration);

 Serial.print(cm);
 Serial.print("cm");
 Serial.println();

 delay(100);
 }


 long microsecondsToCentimeters(long microseconds)
 {
 return microseconds / 29 / 2;
 }
 */
