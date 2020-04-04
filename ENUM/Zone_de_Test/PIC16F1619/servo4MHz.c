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

void main(void) {
    LATC=0;
    PORTC=0;
    ANSELC=0;
    TRISC=1;
    LATA=0;
    PORTA=0;
    ANSELA=0;
    TRISA=0;
    //Initalisation oscillateur
    OSCCONbits.IRCF=0b1101; //4MHz
    
    //PWM CONTROL
    PWM3CON=0;
    RC5PPS = 0b00001110; //On "cable" Le PWM3 sur RC5
    PR2 = 155; // PR2=Tpwm*Fosc/(4*TMRprescaler)-1
    PWM3DCH=0; //Clear PulseWidth register
    PWM3DCL=0;//Clear PulseWidth register
    PIR1bits.TMR2IF = 0;//Clear Timer2 flag
    T2CONbits.CKPS = 0b111; //Prescaler 128
    TRISC=0;
    T2CONbits.ON=1; //Start timer
    PWM3CONbits.EN = 1; //Enable PWm
    PWM3CONbits.OUT= 1; //Enable PWM out
    PWM3DCL=0b00;
    
    //PWM3DCH de 0 à 21 0b00010101;
    while (1) {
        PWM3DCH=0b00000000;
        
        for(int i=0;i<20;i++){
           PWM3DCH+=1;
            __delay_ms(511);
        }       
    }
}
