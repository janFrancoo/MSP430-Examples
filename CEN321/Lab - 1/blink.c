#include <msp430.h>				

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;
	P1DIR |= 0x01;
	volatile unsigned int i;

	while(1) {
		P1OUT ^= 0x01;

		for(i=10000; i>0; i--);
	}
}
