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

# define _XTAL_FREQ 4000000
void __interrupt() tc_int(void){
    LATAbits.LATA4^=1;
    PIR2bits.TMR4IF=0;
    //PIR2bits.OSFIF=0;
}

void main(void) {
    LATC=0;
    PORTC=0;
    ANSELC=0;
    TRISC=0;
    LATA=0;
    PORTA=0;
    ANSELA=0;
    TRISA=0;
    //Initalisation oscillateur
    OSCCONbits.IRCF=0b1101; //4MHz
    
    //Init TIMER4
    T4CLKCON=0;//Selection de Fosc/4
    T4CONbits.CKPS=0b111;// Prédiviseur de 128
    T4CONbits.OUTPS=0b1001; //post diviseur de 10
    PR4=160;//142; //Calcul de la période 4*Tosc*Presc*Postsc*x=T
    T4CONbits.ON=1;
    
    PIE2bits.TMR4IE=1;
    
    INTCONbits.PEIE=1;
    INTCONbits.GIE=1;
    
    while (1) {      
    }
}
