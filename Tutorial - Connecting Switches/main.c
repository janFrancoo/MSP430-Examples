#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR = 0x41;   // pin0 & pin6 of port1 as output
	P1OUT = 0x01;   // pin0 is high

	unsigned int i;

	// if button is pressed, p1.3 will be logic 0
	// button is attached to ground, if it is pressed VCC is going to go gnd directly
	// so, if it is not pressed, VCC is going to flow p1.3
	// if pressed -> 0, if not pressed -> 1

	while (1) {
	    if ((P1IN & 0x08) != 0x08)  // if button is pressed
	        P1OUT ^= 0x41;  // toggle leds

	    for(i=0; i<50000; i++) { }  // for delay
	}

	return 0;
}
