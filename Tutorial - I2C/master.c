// for master device
#include <msp430.h>

unsigned int *ptxData = 0;
unsigned int txData[] = {0x0000, 0x00FA, 0x01F4, 0x02EE, 0x03E8};
unsigned int txByteCntr;
unsigned int high = 0, low = 0;
unsigned int startEnable = 0, cntr = 0;

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;
	
    P1DIR |= BIT5;          // for reset
    P1SEL |= BIT6 | BIT7;   // for I2C
    P1SEL2 |= BIT6 | BIT7;  // for I2C

    // push button interrupt
    P1IE |= BIT3;
    P1IES |= BIT3;
    P1IFG = 0x00;

    // I2C config
    UCB0CTL1 |= UCSWRST;                    // reset
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;   // master mode, i2c, sync
    UCB0CTL1 = UCSSEL_2;                    // smclk for scl
    UCB0BR0 = 12;                           // baud rate
    UCB0BR1 = 0;                            // baud rate (~100kHz)
    UCB0I2CSA = 0x48;                       // slave address
    UCB0CTL1 &= ~UCSWRST;                   // clear reset

    IE2 |= UCB0TXIE;
    ptxData = txData;

    // reset slave device
    P1OUT &= ~BIT5;
    P1OUT |= BIT5;

    _enable_interrupts();

    while (1) {
        txByteCntr = 2;
        while (UCB0CTL1 & UCTXSTP);

        if(startEnable) {
            UCB0CTL1 |= UCTR + UCTXSTT; // transmitter, generate START signal
            LPM0;
        }
    }
}

// start when push button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void){
    startEnable = 1;
    P1IFG = 0x00;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){
    if (txByteCntr) {
        if ((txByteCntr % 2) == 0) {
            high = *ptxData;
            UCB0TXBUF = (high >> 8);
        }
        else if ((txByteCntr % 2) == 1) {
            low = *ptxData++;
            UCB0TXBUF = low;
            cntr++;
        }
        txByteCntr--;
    } else {
        if (cntr == 5) {
            ptxData = ptxData - 5;
            cntr = 0;
        }
        startEnable = 0;
        UCB0CTL1 |= UCTXSTP;    // STOP signal
        IFG2 &= ~UCB0TXIFG;
        LPM0_EXIT;
    }
}
