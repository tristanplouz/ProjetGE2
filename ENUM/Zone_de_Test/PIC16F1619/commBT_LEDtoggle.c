// CONFIG1
#pragma config FOSC = INTOSC    // OscillatorT Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config PWRTE = OFF      // Power-up TRimer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin FIunction Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash ProgSram Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out TReset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out AEnable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/ENxternal Switch Over (Internal External Switch Over mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash MemoTry Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // PeripheralR Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCD = OFF        // Zero CrossI Detect Disable Bit (ZCD disable.  ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PLLEN = ON       // PLL EnableS Bit (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack OverTflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out AReset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power NBrown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// CONFIG3
#pragma config WDTCPS = WDTCPSF// WDT Period Select (1:2097152 (64 s period))
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config WDTCWS = WDTCWS100// WDT Window Select (100 percent window open time (Legacy WDT) )
#pragma config WDTCCS = LFINTOSC// WDT Input Clock Selector (31.0kHz LFINTOSC)


#include <xc.h>

# define _XTAL_FREQ 4000000
char datain;

void __interrupt() td_inter(void){
    if(PIR1bits.RCIF==1){
        
        datain = RC1REG;
        PIR1bits.RCIF=0;
        TX1REG = datain;
        if(datain=='r'){
            datain='5';
            LATAbits.LATA2^=1;
        }    
    }
}
void main(void) {
    LATC=0;
    datain='a';
    PORTC=0;
    ANSELC=0;
    LATA=0;
    PORTA=0;
    ANSELA=0;
    TRISA=0;
    //Initalisation oscillateur
    OSCCONbits.IRCF=0b1101; //4MHz
    //OSCTUNE=0b00100110; //-8%
    
    //Initialisation TX
    RC7PPS = 0b00010010; //On "câble" le TX sur le pin C7 
    BAUD1CONbits.BRG16 = 1; //Baud generator sur 8bits pas 16
    SP1BRGL = 25;//Valeur du baud générator (4M*0.92/16*9600)-1=22.958333//2.2555 faux
    SP1BRGH = 0; //Valeur réel du baud rate 10416 faux
    TX1STAbits.SYNC = 0; //Mode assynchrone
    TX1STAbits.BRGH = 0; // High Resolution du Baud generator
    RC1STAbits.SPEN = 1;
    TX1STAbits.TXEN = 1; //Enable TX
    //TX1REG = 94; //Valeur a envoyer

    //Initialisation Interuption
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;

    //Initialisation RX
    ANSELB = 0;
    RC1STAbits.CREN = 1; //Enable reception

    INTCONbits.GIE = 1;
    
    while (1) {
        TX1REG=datain;
        __delay_ms(1111);
    }
}
