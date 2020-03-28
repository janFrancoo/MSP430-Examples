#include <msp430g2553.h>

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;                   // stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;                      // set DCO to 1Mhz
    DCOCTL = CALDCO_1MHZ;                       // set DCO for 1Mhz

    TACCR0 = 62499;                             // 0.5s delay (delay = (value * frequency) - 1)
    TA0CTL |= TASSEL_2 + ID_3 + MC_1 + TACLR;    // using SMCLK with prescalar of 8 in up mode

    P1DIR |= 0x41;                              // P1.0 & P1.6 as output
    P1OUT = 0x01;                               // P1.0 high

    while (1) {
        if ((TA0CTL & TAIFG) != 0) {            // monitor the flag status
            P1OUT ^= 0x41;                      // toggle the led
            TA0CTL &= ~(TAIFG);                 // clear the flag
        }
    }

}
