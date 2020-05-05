#include <msp430.h>

unsigned int RXData;
unsigned int RXByteCtr = 0;

void main() {
	
	// Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
 
	// P1.1 - P1.2 - P1.4 - P1.5 -> SPI
    P1DIR |= BIT6;
    P1SEL = BIT1 | BIT2 | BIT4 | BIT5 | BIT6;
    P1SEL2 = BIT1 | BIT2 | BIT4 | BIT5;

	// Reset before config
    UCA0CTL1 |= UCSWRST;
	
    // Clock Mode 0, MSB first, 8-bit SPI slave, four pin mode (STE active low)
    UCA0CTL0 |= UCCKPH | UCMSB | UCSYNC | UCMODE_2;
    
	// Clear reset
	UCA0CTL1 &= ~UCSWRST;

	// Enable rx - tx interrupts
    IE2 |= UCA0RXIE | UCA0TXIE;

	// Timer config - SMCLK - UP Mode - /8 freq
    TACCR1 = 0;
    TACCR0 = 999;
    TACCTL1 = OUTMOD_7;
    TACTL = TASSEL_2 | MC_1 | ID_3;

	// Enable interrupts & sleep
    _enable_interrupts();
	LPM0;
}

// Tx interrupt -> Send 0 & disable
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR (void) {
    UCA0TXBUF = 0x00;
    IE2 &= ~UCA0TXIE;
}

// Rx interrupt -> Receive 16 byte data and set TACCR1
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void) {
    if((RXByteCtr % 2) == 0) {
        RXData = UCA0RXBUF;
        RXData = (RXData << 8);
    } else {
        RXData |= UCA0RXBUF;
        TACCR1 = RXData;
    }
	
    RXByteCtr++;
    if (RXByteCtr == 2)
		RXByteCtr = 0;
	
    IE2 |= UCA0TXIE;
}
