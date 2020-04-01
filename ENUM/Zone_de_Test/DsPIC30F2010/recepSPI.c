/*
 * File:   newmainXC16.c
 * Author: tristan
 *
 * Created on 30 mars 2020, 10:25
 */

/*      pinout
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
//#pragma config FOSC = FRC		// Oscillator (Internal Fast RC (No change to Primary Osc Mode bits)) 
#pragma config FCKSMEN = CSW_FSCM_OFF 	// Clock Switching and Monitor (Sw Disabled, Mon Disabled) 

// FWDT 
#pragma config FWPSB = WDTPSB_16 	// WDT Prescaler B (1:16) 
#pragma config FWPSA = WDTPSA_512 	// WDT Prescaler A (1:512) 
#pragma config WDT = WDT_ON 		// Watchdog Timer (Enabled) 

// FBORPOR 
#pragma config FPWRT = PWRT_64 // POR Timer Value (64ms) 
//#pragma config BODENV = BORV20 // Brown Out Voltage (Reserved) 
#pragma config BOREN = PBOR_ON // PBOR Enable (Enabled) 
#pragma config MCLRE = MCLR_EN // Master Clear Enable (Enabled) 

// FGS 
#pragma config GWRP = GWRP_OFF 		// General Code Segment Write Protect (Disabled) 
#pragma config GCP = CODE_PROT_OFF 	// General Segment Code Protection (Disabled)

// FICD 
#pragma config ICS = ICS_PGD // Comm Channel Select (Use PGC/EMUC and PGD/EMUD) 

#define SYS_FREQ 7370000
#define FCY SYS_FREQ/4 

#include <xc.h>
#include <libpic30.h> 
#include <p30F2010.h>//Définition des ports du dsPIC

void __attribute__ ((interrupt(auto_psv))) _SPI1Interrupt(void){
	_LATB1^=1;
	IFS0bits.SPI1IF=0;
	char datain= SPI1BUF;
	if(datain==50){
		_LATB0^=1;
	}
}	
int main(void) {
    LATF=0;
    LATD=0;
    LATB=0;
    LATE=0;
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;
    TRISFbits.TRISF2=1;
    TRISEbits.TRISE8=1;
    
    SPI1CONbits.MODE16=0;
    SPI1CONbits.CKP=0;
    SPI1CONbits.CKE=0;
    SPI1CONbits.SSEN=0;
    SPI1CONbits.MSTEN=0;
    SPI1CONbits.SMP=0;
    SPI1STATbits.SPIROV=0;
    //interuption
    IFS0=0;
    IPC2bits.SPI1IP=0b111;
    IEC0bits.SPI1IE=1;
    
    SPI1STATbits.SPIEN=1;//Enable Serial port
    
    while(1){
	
    }
    return 0; 
}