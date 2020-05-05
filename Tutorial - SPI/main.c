#include <msp430.h> 

int data = 0;

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// P1.0 & P1.6 Out - Off
	P1DIR = 0x41;
	P1OUT = 0x00;

	// P1.1 & P1.2 -> SPI
	P1SEL = 0x06;
	P1SEL2 = 0x06;

	// P1.3 Interrupt
	P1IE = 0x08;
	P1IES = 0x08;
	P1IFG = 0x00;

	// Reset before config
	UCA0CTL1 = UCSWRST;

	// SPI Config -> SMCLK - MSB first - 8 bit - Master device
	UCA0CTL0 = UCCKPL + UCMSB + UCMST + UCSYNC;
	UCA0CTL1 |= UCSSEL_2;

	// No modulation - f = SMCLK/2 - Clock of SPI
	UCA0BR0 |= 0x02;
	UCA0BR1 = 0;
	UCA0MCTL = 0;

	// Activate loopback & clear reset
	UCA0STAT |= UCLISTEN;
	UCA0CTL1 &= ~UCSWRST;

	// Enable interrupts & sleep
	IE2 |= UCA0RXIE;
	_enable_interrupts();
	LPM4;
}

// Btn interrupt -> change value of data
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR() {
    P1IE = 0x00;
    P1IFG = 0x00;

    IE2 |= UCA0TXIE;
    data = (!data) ? 1 : 0;

    P1IE = 0x08;
}

// Tx interrupt -> transmit data and disable tx
#pragma vector = USCIAB0TX_VECTOR;
__interrupt void TX_ISR() {
    UCA0TXBUF = data;
    IE2 &= ~UCA0TXIE;
}

// Rx interrupt -> receive data & blink led
#pragma vector = USCIAB0RX_VECTOR;
__interrupt void RX_ISR() {
    P1OUT = UCA0RXBUF ? 0x01 : 0x40;
}
