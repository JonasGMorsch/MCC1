#include <msp430.h> 

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	return 0;
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
