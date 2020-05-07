#include <msp430.h> 

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR = 0x40;
	P1SEL = 0x40;

	TACCR0 = 50000;
	TACCTL0 = CCIE;
	TACCR1 = 1000;
	TACCTL1 = OUTMOD_7;

	TACTL = TASSEL_2 | MC_1 | ID_3;

	__enable_interrupt();

	while (1)
	    __low_power_mode_0();
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_ISR() {
    if (dir == up) {
        TACCR1 += 1000;
        if (TACCR1 > TACCR0)
            dir = down;
    } else {
        TACCR1 -= 1000;
        if (TACCR1 < 1000)
            dir = up;
    }
}
