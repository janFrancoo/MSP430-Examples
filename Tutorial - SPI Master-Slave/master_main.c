#include <msp430.h>

unsigned int *PTXData = 0, TXByteCtr, cntr = 0;
unsigned int TXData[] = {0x0000, 0x00FA, 0x01F4, 0x02EE, 0x03E8};

void main(void)
{
	// Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // P1.0 & P1.5 Out - P1.0 On
    P1DIR |= BIT0 | BIT5;
    P1OUT |= BIT0;

    // P1.1 - P1.2 - P1.4 -> SPI
    P1SEL = BIT1 | BIT2 | BIT4;
    P1SEL2 = BIT1 | BIT2 | BIT4;
	
	// Btn interrupt
    P1IE |= 0x08;
    P1IES |= 0x08;
    P1IFG = 0x00;

    // Reset before SPI config
    UCA0CTL1 |= UCSWRST;
	
    // Clock Mode 0, MSB first, 8-bit SPI master, SMCLK
    UCA0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC;
    UCA0CTL1 |= UCSSEL_2;
	
	// SPI clk freq = SMCLK/2
    UCA0BR0 |= 0x02;
    UCA0BR1 = 0;
    UCA0MCTL = 0;
	
	// Clear reset
    UCA0CTL1 &= ~UCSWRST;
	
	// Enable rx interrupt
    IE2 |= UCA0RXIE;
    _enable_interrupts();
	
	// Reset slave
    P1OUT &= ~BIT5;
    P1OUT |= BIT5;
	
	PTXData = TXData;
	
    while(1) {
        TXByteCtr = 2;
        LPM0;
    }
}

// Tx interrupt -> Send elements of TxData arr
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIA0TX_ISR(void){
    if (TXByteCtr) {
        if((TXByteCtr % 2) == 0)
            UCA0TXBUF = (*PTXData >> 8);
		
        if((TXByteCtr % 2) == 1) {
            UCA0TXBUF = *PTXData++;
            cntr++;
        }
		
        TXByteCtr--;
    }
    else{
        if(cntr == 5) {
            PTXData = TXData;
            cntr = 0;
        }
        IE2 &= ~UCA0TXIE;
    }
}

// Rx interrupt -> Wake & inactivate slave
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void){
    if(TXByteCtr == 0){
        LPM0_EXIT;
        P1OUT |= 0x01;
    }
}

// Btn interrupt -> Activate slave & tx interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    P1OUT &= ~0x01;
    IE2 |= UCA0TXIE;
	P1IFG = 0x00;
}
