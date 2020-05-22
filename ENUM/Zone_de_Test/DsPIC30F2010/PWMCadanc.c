/*
 * File:   main.c
 * Author: tristan
 *
 * Created on 21 mai 2020, 14:43
 */

/*      Pinout
 * 
 *	    MCLR|		AVDD 
 * RB0		|		AVSS    
 * RB1		|		RE0
 * RB2		|		RE1        
 * RB3		|		RE2
 * RB4		|		RE3
 * RB5		|		RE4
 * VSS       GND|		RE5
 * CLKI		|		VDD
 * RC15		|		GND
 * RC13		|SDI/PGC	RF2
 * RC14		|SDO/PGD	RF3
 * VDD		|SCK		RE8
 * RD1		|		RD0
 */

// FOSC 
//#pragma config FOSC = FRC		// OscillatorT (Internal Fast RC (No change to Primary Osc Mode bits)) 
#pragma config FCKSMEN = CSW_FSCM_OFF 	// ClRock Switching and Monitor (Sw Disabled, Mon Disabled) 

// FWDT 
#pragma config FWPSB = WDTPSB_16 	// WDT PrIescaler B (1:16) 
#pragma config FWPSA = WDTPSA_512 	// WDT PrSescaler A (1:512) 
#pragma config WDT = WDT_OFF 		// WatchdTog Timer (Enabled) 

// FBORPOR 
#pragma config FPWRT = PWRT_64 // POR Timer VAlue (64ms) 
//#pragma config BODENV = BORV20 // Brown OutN Voltage (Reserved) 
#pragma config BOREN = PBOR_ON // PBOR Enable (Enabled) 
#pragma config MCLRE = MCLR_EN // Master Clear Enable (Enabled) 

// FGS 
#pragma config GWRP = GWRP_OFF 		// General Code Segment Write Protect (Disabled) 
#pragma config GCP = CODE_PROT_OFF 	// General Segment Code Protection (Disabled)

// FICD 
#pragma config ICS = ICS_PGD // Comm Channel Select (Use PGC/EMUC and PGD/EMUD) 

#define SYS_FREQ 7370000
#define FCY SYS_FREQ/4 

#include "xc.h" //import de xc16
#include <libpic30.h> 
#include <p30F2010.h> //Définition des ports du dsPIC

void initPWM(){
    PTCONbits.PTOPS=1;
	PTCONbits.PTCKPS=0;
	PTCONbits.PTMOD=0;
	
	PTMR=0;
	
	PTPER = 114;
        
	PWMCON1bits.PMOD3=0;
	PWMCON1bits.PMOD2=0;
	PWMCON1bits.PMOD1=0;
	
	PWMCON1bits.PEN1H=1;
	PWMCON1bits.PEN2H=1;
	PWMCON1bits.PEN3H=1;
	
	PWMCON1bits.PEN1L=1;
	PWMCON1bits.PEN2L=1;
	PWMCON1bits.PEN3L=1;
}
int st=0;
void __attribute__ ((interrupt(auto_psv))) _PWMInterrupt( void )   
{
    IFS2bits.PWMIF = 0;
}
void __attribute__ ((interrupt(auto_psv))) _INT1Interrupt( void )               
{
    LATEbits.LATE8^=1;
    st=TMR1;
    PR2=st/6;
    TMR1=0;
    IFS1bits.INT1IF = 0;
}

void __attribute__ ((interrupt(auto_psv))) _T2Interrupt( void )               
{
    LATEbits.LATE8^=1;
    TMR2=0;
    IFS0bits.T2IF = 0;
}

void main(void) {
	//https://turbofuture.com/misc/Configure-PWM-on-dspic30f-and-dspic33f
    //http://www.isilf.be/Articles/ISILF05p157gramme.pdf
	TRISE=0;
	PORTE=0;
	TRISDbits.TRISD0=1;
            
	initPWM();
	
	PDC1=80;
	PDC2=80;
	PDC3=80;
	PTCONbits.PTEN=1;
    
    IPC4bits.INT1IP=0b111;
	IFS0=0;
    IEC0bits.T2IE=1;
	IEC2bits.PWMIE = 1;
    IEC1bits.INT1IE = 1;
    T1CONbits.TCKPS=0b01;
    T1CONbits.TON=1;
    T2CONbits.TCKPS=0b01;
    T2CONbits.TON=1;
    PR2=100;
	
	while(1){
        OVDCON = 0b0011001100000000;
        __delay_us(300);
        OVDCON = 0b0011000000000000;
        __delay_us(300);
        OVDCON = 0b0011110000000000;
        __delay_us(300);
        OVDCON = 0b0000110000000000;
        __delay_us(300);
        OVDCON = 0b0000111100000000;
        __delay_us(300);
        OVDCON = 0b0000001100000000;
        __delay_us(300);
    }
	
}
