#include <msp430.h> 

#define LED BIT0
#define BTN BIT3

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR = LED;    // P1.0 is work as output
	P1OUT = LED;    // P1.0 is high

	P1IE = BTN;     // enable P1 interrupt
	P1IES = BTN;    // since bit3 is high, interrupt flag is set on when there is a high-to-low signal
	P1IFG = 0x00;   // set on when an edge occurs, must be resetted to 0

	_enable_interrupts();   // enabling interrupts

	while (1);  // infinite loop
}

#pragma vector = PORT1_VECTOR;  // associating the function to the interrupt vector
__interrupt void PORT1_ISR() {  // interrupt function
    P1OUT ^= 0x01;  // toggle the led
    P1IFG = 0x00;   // reset interrupt flag
}
