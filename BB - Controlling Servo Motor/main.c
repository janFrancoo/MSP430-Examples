#include <msp430.h> 

int dir = 27000;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR = 0x40;
	P1SEL = 0x40;

	P1IE = 0x08;
	P1IES = 0x08;
	P1IFG = 0x00;

	P2DIR = 0x40;
	P2SEL = 0x40;

	TACCR0 = 30000;
    TACCR1 = dir;
    TACCTL1 = OUTMOD_3;

    TACTL = TASSEL_2 | MC_1;
	
    __enable_interrupt();
    LPM0;
}

#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR() {
    P1IES = 0x00;
    P1IFG = 0x00;
    unsigned volatile int i;
    for (i=0; i<10000; i++) { }

    dir += 500;
    if (dir == 29500)
        dir = 27000;
    TACCR1 = dir;

    P1IES = 0x08;
}
