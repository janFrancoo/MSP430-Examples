#include <msp430.h> 

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR = 0x41;
    P1OUT = 0x00;

    P1IE = 0x08;
    P1IES = 0x08;
    P1IFG = 0x00;

    P1SEL = 0x06;
    P1SEL2 = 0x06;

    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_1;
    UCA0CTL1 = UCSSEL_2;
    UCA0CTL1 &= ~UCSWRST;

    IE2 |= UCA0RXIE;
    _enable_interrupts();
    LPM4;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR() {
    int data = UCA0RXBUF;

    if (data == 0x00)
        P1OUT = 0x00;
    else if (data == 0x01)
        P1OUT = 0x01;
    else if (data == 0x10)
        P1OUT = 0x40;
    else if (data == 0x11)
        P1OUT = 0x41;
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR() {
    P1IE = 0x00;
    P1IFG = 0x00;

    unsigned volatile i;
    for (i=0; i<1000; i++) { }

    P1OUT = 0x41;
    for (i=0; i<1000; i++) { }
    P1OUT = 0x00;

    P1IE = 0x08;
}
