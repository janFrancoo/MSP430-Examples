#include <msp430.h> 

void strobe() {
    P2OUT |= 0x01;
    P2OUT &= 0xFE;
}

void write(unsigned char data) {
    __delay_cycles(40);
    P1OUT = data;
    strobe();
}

void clear() {
    P2OUT &= 0xFD;
    write(0x01);
    __delay_cycles(2000);
}

void puts(const char *s) {
    P2OUT |= 0x02;
    while (*s)
        write(*s++);
}

void putch(char c) {
    P2OUT |= 0x02;
    write(c);
}

void goTo(unsigned char row, unsigned char column) {
    P2OUT &= 0xFD;
    if (row == 1)
        write(0x80 + ((column - 1) % 16));
    else if (row == 2)
        write(0xC0 + ((column - 1) % 16));
}

void init() {
    __delay_cycles(15000);

    P1OUT = 0x03;
    strobe();
    __delay_cycles(5000);

    strobe();
   __delay_cycles(200);

   strobe();
   __delay_cycles(200);

   P1OUT = 0x02;
   strobe();

   write(0x30); // 8-bit mode, 0x28 for 4-bit mode
   write(0x0C); // Display on, cursor off
   write(0x3C); // Activate second line
   clear();
   write(0x06); // Right-shift, 0x04 for left-shift
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;

	DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;

    P1SEL = 0x00;
    P1OUT = 0x00;
    P1DIR = 0xFF;
    P2SEL = 0x00;
    P2OUT = 0x00;
    P2DIR = 0xFF;

    init();
    goTo(1, 1);
    puts("   JanFrancoo   ");
    goTo(2, 1);
    puts(" janfranco.com  ");

    while(1);
}
