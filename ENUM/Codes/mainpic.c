/*
 * File:   mainpic.c
 * Author: Tristan
 * Description: Code du PIC16F1619 du Projet GE2, groupe Tristan/Florian 
 * Created on 20 février 2020, 10:25:54
 */


/*      Pinout
 * 
 *       VCC|GND 
 * RA5      |PRGD    RA0
 * RA4 Effet|PRGC    RA1
 * RA3  MCLR|AU Jump RA2
 * RC5  SERV|        RC0
 * RC4      |        RC1
 * RC3      |SDO     RC2
 * RC6      |SDI     RB4
 * RC7  BTTX|BTRX    RB5
 * RB7   SCK|Effet	 RB6
 */

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
#pragma config WRT = OFF        // Flash MemorTy Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral RPin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCD = OFF        // Zero Cross IDetect Disable Bit (ZCD disable.  ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PLLEN = ON       // PLL Enable SBit (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack OverfTlow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out RAeset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power BNrown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// CONFIG3
#pragma config WDTCPS = WDTCPSF// WDT Period Select (1:2097152 (64 s period))
#pragma config WDTE = OFF        // Watchdog Timer Disable 
#pragma config WDTCWS = WDTCWS100// WDT Window Select (100 percent window open time (Legacy WDT) )
#pragma config WDTCCS = LFINTOSC// WDT Input Clock Selector (31.0kHz LFINTOSC)

//Import des librairies
#include <xc.h>

#define _XTAL_FREQ 4000000

//Fonctions d'initialisation des fonctions et modules utilisés plus tard
/*
 Initialisation du module PWM3 pour le servomoteur sur le port C5
 */
void init_PWM(){ 
	PWM3CON=0;
	TRISCbits.TRISC5=0;
    RC5PPS = 0b00001110; //On "câble" le PWM3 sur RC5
    PR2 = 155; // PR2=Tpwm*Fosc/(4*TMRprescaler)-1
    PWM3DCH=0; //Clear PulseWidth Low register
    PWM3DCL=0; //Clear PulseWidth High register
    PIR1bits.TMR2IF = 0;//Clear Timer2 flag
    T2CONbits.CKPS = 0b111; //Prescaler 128
    TRISC=0;
    T2CONbits.ON=1; //Start timer
    PWM3CONbits.EN = 1; //Enable PWm
    PWM3CONbits.OUT= 1; //Enable PWM out
    PWM3DCL=0b00;
}
/*
 Initialisation du module UART: Tx sur le port C7
 */
void init_TX(){
    RC7PPS = 0b00010010; //On "câble" le TX sur le pin C7 
    BAUD1CONbits.BRG16 = 1; //Baud generator sur 8bits pas 16
    TX1STAbits.SYNC = 0; //Mode assynchrone
    TX1STAbits.BRGH = 0; // High Resolution du Baud generator
    RC1STAbits.SPEN = 1; // Enable Serial Port
    TX1STAbits.TXEN = 1; //Enable TX
}
/*
 Initialisation du module TIMER4 pour le système de sécurité de déconnection BT
 */
void init_TMR4(){
	T4CLKCON=0;//Selection de Fosc/4
    T4CONbits.CKPS=0b111;// Prédiviseur de 128
    T4CONbits.OUTPS=0b1001; //Post diviseur de 10
    PR4=160;//Calcul de la période 4*Tosc*Presc*Postsc*x=T
    T4CONbits.ON=1;
    PIE2bits.TMR4IE=1; //Interuption
}
/*
 Initialisation du module SSP pour la communication en SPI entre le PIC et le dsPIC
 */
void init_SPI(){
	RC2PPS=0b00010001;//On câble le SDO sur C2
    RB7PPS=0b00010000;//On câble le SCK sur B7
    SSP1CONbits.CKP=0;
    SSP1STATbits.CKE=0;
    SSP1CON1bits.SSPM=0b0010;
	PIR1bits.SSP1IF=0; //Reset the flag
    SSP1CON1bits.SSPEN=1;//Enable  the connection
    SSP1BUF=100;
}

char puis=10,ang=10,fail=0,gfail=0,eff1=0,eff2=0;

//Vecteur d'interruption
void __interrupt() td_int(void){
	if(PIR1bits.RCIF){ //QQch arrive sur le BT
		char datain = RC1REG; //On lit la donnée
        PIR1bits.RCIF=0; //reset du flag
        TMR4=0; //reset du TIMER de sécurité
        //Début du traitement
        if(datain == 0){
            //ce caractère est envoyé toutes les 100ms par l'application
			fail=0; //Ne se met pas en mode erreur
        }
        else if (datain<22&& datain>2){ //Commande de direction
            ang=datain; //Valeur directement correct pour le PWM du servo
        }
        else if (datain>100&& datain<200){ //Commande de la puissance
             //traitement pour la puissance
			puis=255-datain;
			LATAbits.LATA2^=1;
        }
        else if (datain>201){ //Commandes des effets stylés
            switch(datain){
                case 202:
                    eff1^=1;
                    break;
				case 203:
                    eff2^=1;
                    break;
				case 204:
                    eff2^=1;
					eff1^=1;
                    break;
            }
        }
    }
    else if(PIR2bits.TMR4IF){ //Le Timer de Sécurité overflow, 200ms ce sont écoulés sans reception BT
        fail=1;//On set un failflag
        PIR2bits.TMR4IF=0;//On reset le flag
    }
    else if( PIR1bits.SSP1IF){ //Fin de manoeuvre SPI
		char datain;
        datain=SSP1BUF; //On lit le buffer
        PIR1bits.SSP1IF=0;//On reset le flag
    }
	else if(INTCONbits.INTF){
        fail=1;//On set un failflag
		gfail=1;////On set un global failflag
		//LATBbits.LATB6^=1;
        INTCONbits.INTF=0;//On reset le flag
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
    
    LATB=0;
	ANSELB = 0;
    TRISBbits.TRISB4=1;//SDin
    TRISCbits.TRISC2=0;//SDout
    TRISBbits.TRISB7=0;//CLK out
    TRISBbits.TRISB6=0;//Effet
	
    //Initalisation oscillateur
    OSCCONbits.IRCF=0b1101; //4MHz
	
	//PWM CONTROL
    init_PWM();
    
    //Baudrate générateur
    SP1BRGL = 25;//Valeur du baud générator (4M/16*9600)-1=25.04
    SP1BRGH = 0;//Valeur réelle de 9615
	
	//Initialisation TX
    init_TX();

    //Initialisation RX
    RC1STAbits.CREN = 1; //Enable reception RX

    //Init TIMER4
    init_TMR4();
    
    //SPI init
    init_SPI();
	
	//AU init
	OPTION_REGbits.INTEDG=0;
	INTCONbits.INTF = 0;

    //Initialisation Interuption
    INTCONbits.INTE=1;
    PIE1bits.RCIE = 1;
    PIE1bits.SSP1IE=1;
    INTCONbits.PEIE=1;
    INTCONbits.GIE=1;
    
    //Enable SACDEI
	PID1CONbits.MODE=0b000;
    
    while (1) {
		
		if(gfail==1){
			LATAbits.LATA4^=1;
			LATBbits.LATB6^=1;
			INTCONbits.GIE=0;
			SSP1BUF=0;
			PWM3DCH=10;
			__delay_ms(100);	
		}
		else if(fail==1){ //fail triggered
			LATAbits.LATA4^=1;
			SSP1BUF=0;
			PWM3DCH=10;
			__delay_ms(100);
		}
		else{
			SSP1BUF=puis; //On envoie la valeur de la puissance sur le SPI
			PWM3DCH=ang; //On ecrit la valeur de l'impulsion pour le PWM
			LATAbits.LATA4=eff1;
			LATBbits.LATB6=eff2;
			__delay_ms(10); // Repète l'opération toutes les 10ms
		}
    }
}

