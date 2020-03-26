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
    if(PIR1bits.RCIF==1){ //QQch arrive sur le BT
        char datain = RC1REG; //On lit la donnée
        PIR1bits.RCIF=0; //reset du flag
        //Début du traitement
        if(datain == 0){
            //reset du TIMER de sécurité
            LATAbits.LATA2^=1;
        }
        else if (datain<22&& datain>2){ //Commande de direction
            PWM3DCH=datain; //Valeur directement correct pour le PWM du servo
        }
        else if (datain<100&& datain>22){ //Commande de la puissance
             //traitement pour la puissance
        }
        else if (datain>201){ //Commande des effets
            switch(datain){
                case 202:
                    LATAbits.LATA4^=1;
                    break;
            }
        }
         
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
    PWM3DCH=9;
    //PWM3DCH de 0 à 21 0b00010101;
    
    //Initialisation TX
    RC7PPS = 0b00010010; //On "cable" le TX sur le pin C7 
    BAUD1CONbits.BRG16 = 1; //Baud generator sur 8bits pas 16
    SP1BRGL = 25;//Valeur du baud générator (4M*0.92/16*9600)-1=22.958333//2.2555
    SP1BRGH = 0; //Valeur réel du baud rate 10416
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
    }
}
