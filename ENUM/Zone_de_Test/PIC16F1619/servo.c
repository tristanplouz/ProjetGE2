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
#pragma config WDTCPS = WDTCPS1F// WDT Period Select (Software Control (WDTPS))
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config WDTCWS = WDTCWSSW// WDT Window Select (Software WDT window size control (WDTWS bits))
#pragma config WDTCCS = SWC     // WDT Input Clock Selector (Software control, controlled by WDTCS bits)


#include <xc.h>
#define _XTAL_FREQ 500000
void writeAngle(char ang) {
    if (ang > 168) {
        PWM3DCH = 0b00000010;
        PWM3DCL = 0b01000000;
    }
    if (ang > 143 && ang < 168) {
        PWM3DCH = 0b00000010;
        PWM3DCL = 0b00000000;
    }
    if (ang > 118 && ang < 143) {
        PWM3DCH = 0b00000001;
        PWM3DCL = 0b11000000;
    }
    if (ang > 93 && ang < 118) {
        PWM3DCH = 0b00000001;
        PWM3DCL = 0b10000000;
    }
    if (ang > 68 && ang < 93) {
        PWM3DCH = 0b00000001;
        PWM3DCL = 0b01000000;
    }
    if (ang > 43 && ang < 68) {
        PWM3DCH = 0b00000001;
        PWM3DCL = 0b00000000;
    }
    if (ang > 18 && ang < 43) {
        PWM3DCH = 0b00000000;
        PWM3DCL = 0b11000000;
    }
    if (ang > 0 && ang < 18) {
        PWM3DCH = 0b00000000;
        PWM3DCL = 0b01000000;
    }
}
void main(void) {
    LATC = 0;
    TRISC = 0;
    PORTC = 0;
    ANSELC = 0;

    //Définition PWM sur C5
    RC5PPS = 0b00001110; //On "cable" Le PWM3 sur RC5
    PR2 = 275;
    PIR1bits.TMR2IF = 0;
    T2CON = 0b11110000;
    PWM3CON = 0x80;
    while (1) {
        for(int i =0;i<180;i+=10){
            writeAngle(i);
            __delay_ms(50);
        }
        for(int i =180;i>0;i-=10){
            writeAngle(i);
            __delay_ms(50);
        }
        
    }
}


//180
//PWM3DCH=0b00000010;
//PWM3DCL=0b01000000;
//
//PWM3DCH=0b00000010;
//PWM3DCL=0b00000000;
//
//PWM3DCH=0b00000001;
//PWM3DCL=0b11000000;
//
//PWM3DCH=0b00000000;
//PWM3DCL=0b10000000;
/* RC5PPS = 0b00001110; //On "cable" Le PWM3 sur RC5
    PR2 = 275;
    PIR1bits.TMR2IF = 0;
    T2CON = 0b11110000;
    PWM3CON = 0x80;
 * PWM3DCH=0b00000000;
        PWM3DCL=0b01000000;
        __delay_ms(1000);
        PWM3DCH=0b00000000;
        PWM3DCL=0b10000000;
        __delay_ms(1000);
        PWM3DCH=0b00000000;
        PWM3DCL=0b11000000;
        __delay_ms(1000);
        PWM3DCH=0b00000001;
        PWM3DCL=0b00000000;
        __delay_ms(1000);
        PWM3DCH=0b00000001;
        PWM3DCL=0b01000000;
        __delay_ms(1000);
        PWM3DCH=0b00000001;
        PWM3DCL=0b10000000;
        __delay_ms(1000);
        PWM3DCH=0b00000001;
        PWM3DCL=0b11000000;
        __delay_ms(1000);
        PWM3DCH=0b00000010;
        PWM3DCL=0b00000000;
        __delay_ms(1000);
        PWM3DCH=0b00000010;
        PWM3DCL=0b01000000;
        __delay_ms(1000);
        PWM3DCH=0b00000010;
        PWM3DCL=0b10000000;
        __delay_ms(5000);*/
