/*
 * File:   mainpic.c
 * Author: tristan
 *
 * Created on 20 février 2020, 10:25
 */


/*      pinout
 * 
 *       VCC|GND 
 * RA5      |PRGD    RA0
 * RA4      |PRGC    RA1
 * RA3  MCLR|        RA2
 * RC5  SERV|        RC0
 * RC4      |        RC1
 * RC3      |SDO     RC2
 * RC6      |SDI     RB4
 * RC7  BTTX|BTRX    RB5
 * RB7      |SCK     RB6

 */

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

//Import des librairies
#include <xc.h>

#define _XTAL_FREQ 4000000


char datain;
//Vecteur d'intéruption
void __interrupt() td_inter(void){
    if( PIR1bits.SSP1IF==1){
        LATAbits.LATA2^=1;
        datain=SSP1BUF;
        PIR1bits.SSP1IF=0;
    }
}

void main(void) {
    LATC=0;
    PORTC=0;
    ANSELC=0;
    TRISC=1;
    LATA=0;
    PORTA=0;
    ANSELA=0;
    TRISA=0;
    TRISBbits.TRISB4=1;//SDin
    TRISCbits.TRISC2=0;//SDout
    TRISBbits.TRISB7=0;//CLK out
    
    //Initalisation oscillateur
    OSCCONbits.IRCF=0b1101; //4MHz
    
    //Baudrate générateur
    SP1BRGL = 25;//Valeur du baud générator (4M/16*9600)-1=25.04
    SP1BRGH = 0;//Valeur réelle de 9615
    
    //SPI init
    RC2PPS=0b00010001;//On cable le SDO sur C2
    RB7PPS=0b00010000;
    SSP1CONbits.CKP=0;
    SSP1STATbits.CKE=0;
    SSP1CON1bits.SSPM=0b0010;
    SSP1CON1bits.SSPEN=1;//Enable  the connection
    
    
    //Interuption
    PIR1bits.SSP1IF=0;
    PIE1bits.SSP1IE=1;
    INTCONbits.PEIE=1;
    INTCONbits.GIE=1;
    
    while (1) {    
        SSP1BUF=50;
        __delay_ms(2111);
	SSP1BUF=60;
        __delay_ms(2111);
    }
}
