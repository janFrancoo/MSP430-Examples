#include <msp430.h> 

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	TACCR0 = 12499;
    TA0CTL |= TASSEL_2 + ID_3 + MC_1 + TACLR;
    TACCTL0 = CCIE;

	P1IES |= 0x08;
	P1IFG &= ~0x08;
	P1IE |= 0x08;
	_enable_interrupt();

	P1DIR = 0x01;
	P1OUT = 0x01;

	while (1);
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void) {
    P1IFG &= ~0x08;

    P1IE &= 0x00;
    unsigned volatile i;
    for (i=0; i<1000; i++) { }

    if (TACCR0 == 62499)
        TACCR0 = 12499;
    else
        TACCR0 = 62499;

    P1IE |= 0x08;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER_A(void) {
    P1OUT ^= 0x01;
}
