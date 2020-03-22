#include <msp430.h> 

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= 0x01;

	volatile unsigned int i, j;

	while(1) {
	    BCSCTL1 &= BIT7;    // DCO(9, 5)
	    BCSCTL1 |= BIT0 + BIT3;
	    DCOCTL &= ~BIT5;
	    DCOCTL |= BIT7;

	    for (j=0; j<5; j++) {
	        P1OUT ^= 0x01;
	        for (i=0; i<50000; i++) { }
	    }

	    BCSCTL1 &= ~(BIT0 + BIT2 + BIT3);   // DCO(2,6)
	    BCSCTL1 |= BIT1;
	    DCOCTL &= ~BIT5;
	    DCOCTL |= BIT6 + BIT7;

	    for (j=0; j<5; j++) {
            P1OUT ^= 0x01;
            for (i=0; i<50000; i++) { }
	    }
	}

}
