#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR = 0x41;   // set pin0 and pin6 as output
	P1OUT = 0x01;   // set pin0 high, pin6 low

	unsigned int i;

	while (1) {
	    P1OUT ^= 0x41;  // toggle leds with XOR
	    for(i=0; i<30000; i++) {}
	}

	return 0;
}
