/* 
 * File:   blink.c
 * Author: skytronic
 *
 * Created on 10 mars 2020, 22:04:10
 */

#define SYS_FREQ 7370000L 
#define FCY SYS_FREQ/4 

// FOSC 
//#pragma config FOSC = FRC 			// Oscillator (Internal Fast RC (No change to Primary Osc Mode bits)) 
#pragma config FCKSMEN = CSW_FSCM_OFF 	// CLock Switching and MOnitor (Sw Disabled, Mon Disabled) 

// FWDT 
#pragma config FWPSB = WDTPSB_16 	// WDT PRescaler B (1:16) 
#pragma config FWPSA = WDTPSA_512 	// WDT Prescaler A (1:512) 
#pragma config WDT = WDT_ON 		// Watchdog TImer (Enabled) 

// FBORPOR 
#pragma config FPWRT = PWRT_64 // POR Timer VAlue (64ms) 
//#pragma config BODENV = BORV20 // BrowN Out Voltage (Reserved) 
#pragma config BOREN = PBOR_ON // PBOR Enable (Enabled) 
#pragma config MCLRE = MCLR_EN // Master Clear Enable (Enabled) 

// FGS 
#pragma config GWRP = GWRP_OFF 		// General Code Segment Write Protect (Disabled) 
#pragma config GCP = CODE_PROT_OFF 	// General Segment Code Protection (Disabled)

// FICD 
#pragma config ICS = ICS_PGD // Comm Channel Select (Use PGC/EMUC and PGD/EMUD) 

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h> 

/*
 * This program blink a LED on output B0
 */
int main(void) {
    
    _TRISB0 = 0;
    
    while(1){
        _LATB0 = 0;
        __delay_ms(200);
        _LATB0 = 1;
        __delay_ms(500);
    }
    
    return 0; 
}
