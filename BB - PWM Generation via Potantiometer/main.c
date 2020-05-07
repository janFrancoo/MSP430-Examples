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
    TACCTL1 = OUTMOD_3;

    TACTL = TASSEL_2 | MC_1 | ID_3;

    ADC10CTL1 |= CONSEQ1;
    ADC10CTL0 |= ADC10SHT_2 + ADC10ON + MSC + ADC10SR;
    ADC10AE0 |= 0x01;
    ADC10CTL0 |= ADC10SC + ENC;

    __enable_interrupt();

    while (1);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA_ISR() {
    TACCR1 = ADC10MEM * 45;
}
