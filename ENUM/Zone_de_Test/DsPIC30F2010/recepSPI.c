/*
 * File:   newmainXC16.c
 * Author: tristan
 *
 * Created on 30 mars 2020, 10:25
 */

#define SYS_FREQ 7370000
#define FCY SYS_FREQ/4 

#include "xc.h"
#include <libpic30.h>
void __attribute__((interrupt)) inter(void){
    _LATB0^=1;
    if(_SPI1IF){
        _LATB0^=1;
        _SPI1IF=0;
        char datain = SPI1BUF;
        SPI1BUF= datain;
    }
}
int main(void) {
    LATF=0;
    LATD=0;
    TRISFbits.TRISF2=1;
    TRISDbits.TRISD0=1;
    
    
    SPI1CONbits.MODE16=0;
    SPI1CONbits.CKP=0;
    SPI1CONbits.CKE=0;
    SPI1CONbits.MSTEN=0;
    
    
    //interuption
    _SPI1IF=0;
    IEC0bits.SPI1IE=1;
    
    SPI1STATbits.SPIEN=1;
    SPI1BUF=0b0010010;
    
    while(1){
        SPI1BUF=0b0010010;
        __delay_ms(1000);
    }
    return 0;
}
