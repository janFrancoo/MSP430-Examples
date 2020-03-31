#include <msp430.h> 

void main(void)
{
	WDTCTL = WDT_ARST_1000; // 1s

	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
	
	P1DIR = 0x41;
	P1OUT = 0x01;

	unsigned int i;
	for (i=0; i<30000; i++);

	P1OUT = 0x40;

	while(1);
}
