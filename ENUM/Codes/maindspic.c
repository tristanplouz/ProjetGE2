/*
 * File:   mainDsPIC.c
 * Author: Tristan
 * Description: Code du dsPIC30F2010 du Projet GE2, groupe Tristan/Florian 
 * Created on 30 mars 2020, 15:21:51
 *************************/

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
#pragma config WDT = WDT_ON 		// WatchdTog Timer (Enabled) 

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

#include <xc.h> //import de xc16
#include <libpic30.h> 
#include <p30F2010.h> //Définition des ports du dsPIC

int delay=500;

void __attribute__ ((interrupt(auto_psv))) _SPI1Interrupt(void){
	//_LATB0^=1;
	IFS0bits.SPI1IF=0;
	int datain= SPI1BUF;
	delay=datain*2;
}	

int main(void) {
    //Initialisation des registres de Sorties
    LATF=0;
    LATD=0;
    LATB=0;
    LATE=0;
    
    //Definition des registres TRIS enregisTANt les entrées/sorties 
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;
    TRISFbits.TRISF2=1;
    TRISEbits.TRISE8=1;
    
    //intialisation des paramétres du SPI
    SPI1CONbits.MODE16=0;
    SPI1CONbits.CKP=0;
    SPI1CONbits.CKE=0;
    SPI1CONbits.SSEN=0;
    SPI1CONbits.MSTEN=0;
    SPI1CONbits.SMP=0;
    SPI1STATbits.SPIROV=0;
    
    //Mise en place des interuptions
    IFS0=0;
    IPC2bits.SPI1IP=0b111;
    IEC0bits.SPI1IE=1;
    
    SPI1STATbits.SPIEN=1;//Enable Serial port
    
    //Enable SACDEI
	U1STAbits.ALTIO=0;
	
    while(1){
		_LATB0=1;
		__delay_ms(delay);
		_LATB0=0;
		__delay_ms(delay);
    }
    return 0; 
}
