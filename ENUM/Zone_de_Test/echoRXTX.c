// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switch Over (Internal External Switch Over mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCD = OFF        // Zero Cross Detect Disable Bit (ZCD disable.  ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PLLEN = ON       // PLL Enable Bit (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// CONFIG3
#pragma config WDTCPS = WDTCPSF// WDT Period Select (1:2097152 (64 s period))
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config WDTCWS = WDTCWS100// WDT Window Select (100 percent window open time (Legacy WDT) )
#pragma config WDTCCS = LFINTOSC// WDT Input Clock Selector (31.0kHz LFINTOSC)


#include <xc.h>
#define _XTAL_FREQ 500000


void __interrupt() interruptHandler(void) {
    if (PIR1bits.RCIF == 1) {
        TX1REG = RC1REG;
        PIR1 = 0;
    }

}

void main(void) {
    ANSELC = 0;

    //Initialisation TX
    RC7PPS = 0b00010010; //On "cable" le TX sur le pin C7 
    BAUD1CONbits.BRG16 = 0; //Baud generator sur 8bits pas 16
    SP1BRGL = 2; //Valeur du baud générator (500000/16*9600)-1=2.2555
    SP1BRGH = 0; //Valeur réel du baud rate 10416
    TX1STAbits.SYNC = 0; //Mode assynchrone
    TX1STAbits.BRGH = 1; // High Resolution du Baud generator
    RC1STAbits.SPEN = 1;
    TX1STAbits.TXEN = 1; //Enable TX
    TX1REG = 94; //Valeur a envoyer

    //Initialisation Interuption
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;

    //Initialisation RX
    ANSELB = 0;
    RC1STAbits.CREN = 1; //Enable reception

    INTCONbits.GIE = 1;

    while (1) {
    }
}
