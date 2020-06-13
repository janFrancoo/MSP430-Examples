// for slave device
#include <msp430.h>

unsigned int rxData, rxByteCntr = 0;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= BIT2;                  // for led
    P1SEL |= BIT6 | BIT7 | BIT2;    // for i2c & pwm
    P1SEL2 |= BIT6 | BIT7;          // for i2c & pwm

    // I2C config
    UCB0CTL1 |= UCSWRST;            // reset
    UCB0CTL0 = UCMODE_3 | UCSYNC;   // master mode, i2c, sync
    UCB0I2COA = 0x48;               // set device address
    UCB0CTL1 &= ~UCSWRST;           // clear reset

    UCB0I2CIE |= UCSTPIE | UCSTTIE; // START & STOP interrupts
    IE2 |= UCB0RXIE;                // rx interrupt

    // pwm setup
    TACCR1 = 0;
    TACCR0 = 999;
    TACCTL1 = OUTMOD_7;
    TACTL = TASSEL_2 | MC_1 | ID_3;

    _enable_interrupts();

    while (1){
        rxByteCntr = 0;
        LPM0;
    }
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){
    if ((rxByteCntr % 2) == 0) {
        rxData = UCB0RXBUF;
        rxData = (rxData << 8);
    } else if ((rxByteCntr % 2) == 1) {
        rxData |= UCB0RXBUF;
        TACCR1 = rxData;
    }

    rxByteCntr++;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void){
    UCB0STAT &= ~(UCSTPIFG | UCSTTIFG);
    if (rxByteCntr) LPM0_EXIT;
}
