#include<msp430.h>

void delay(unsigned int duration) {
    volatile unsigned int i = duration;
    while(i > 0) i--;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdogtimer

    P1DIR |= BIT0 + BIT6;
    P1OUT &= ~(BIT0 + BIT6);

    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    volatile int btn_cnt = 0;

    for(;;) {
        delay(20000);

        if(!(P1IN & BIT3))
            btn_cnt = (btn_cnt + 1) % 4;

        switch(btn_cnt) {
            case 0:
                P1OUT |= BIT6;
                break;
            case 1:
                P1OUT |= BIT6;
                P1OUT |= BIT0;
                break;
            case 2:
               P1OUT ^= BIT6;
               P1OUT ^= BIT0;
               break;
            case 3:
                P1OUT &= ~BIT0;
                P1OUT &= ~BIT6;
                break;
            default:
                break;
        }
    }

    return 0;
}
