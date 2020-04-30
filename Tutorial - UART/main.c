#include <msp430.h> 

int data = 0;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR = 0x40;
	P1OUT = 0x00;

	P1IE = 0x08;
	P1IES = 0x08;
	P1IFG = 0x00;

	// P1.1 && P1.2 -> UART
	P1SEL = 0x06;
	P1SEL2 = 0x06;

	// baud rate 9600 (values are taken from docs)
	UCA0BR0 = 104;
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS_1;

	UCA0STAT |= UCLISTEN; // for internal comm, short circuit

	UCA0CTL1 = UCSSEL_2; // SMCLK for clock
	// UART is async, this clock feeds the baud

	UCA0CTL1 &= ~UCSWRST; // just necessary
	// configure the settings and reset

	IE2 |= UCA0RXIE | UCA0TXIE; // enabling rx/tx interrupts
	_enable_interrupts();

	LPM4;
}

// tx interrupt
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAOTX_ISR() {
    UCA0TXBUF = data;
}

// rx interrupt
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR() {
    P1OUT = ((UCA0RXBUF) ? 0x40 : 0x00);
}

// btn interrupt
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR() {
    P1IE = 0x00;
    P1IFG = 0x00;

    unsigned volatile i;
    for (i=0; i<1000; i++) { }

    data = (data == 1) ? 0 : 1;
    P1IE = 0x08;
}
