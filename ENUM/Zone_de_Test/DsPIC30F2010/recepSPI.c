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

#define SYS_FREQ 7370000
#define FCY SYS_FREQ/4 

#include "xc.h"
#include <libpic30.h>
#include <p30F2010.h>//Définition des ports du dsPIC

void __attribute__((interrupt)) inter(void){
	_LATB0^=1;
    if(IFS0bits.SPI1IF==1){
	    
        IFS0bits.SPI1IF=0;
        char datain = SPI1BUF;
        SPI1BUF= datain;
    }
	__delay_ms(5000);
}
int main(void) {
    LATF=0;
    LATD=0;
    LATB=0;
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;
    TRISFbits.TRISF2=1;
    TRISEbits.TRISE8=0;
   
    
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
    SPI1BUF=0b0010010;
    __delay_ms(1000);
    _LATB1=1;
    while(1){
        //_LATB0=SPI1STATbits.SPIRBF;
    }
    return 0;
}
