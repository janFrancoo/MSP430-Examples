#include <msp430.h> 

void delay(unsigned int duration) {
    volatile unsigned int i = duration;
    while (i > 0) i--;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= BIT0 + BIT6;
	P1OUT &= ~(BIT0 + BIT6);

	P1DIR &= ~BIT3;

	P1REN |= BIT3;
	P1OUT |= BIT3;

	for(;;) {
	    delay(20000);
	    if (P1IN & BIT3) {
	        P1OUT &= ~BIT0;
	        P1OUT ^= BIT6;
	    } else {
	        P1OUT |= BIT0;
	        P1OUT |= BIT6;
	    }
	}

	return 0;
}
