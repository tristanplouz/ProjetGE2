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
#pragma config FPR = XTL                // Primary Oscillator Mode (XTL)
#pragma config FOS = FRC                // Oscillator Source (Internal Fast RC)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = PGD                // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#define SYS_FREQ 7370000
#define FCY SYS_FREQ/4 

#include "xc.h" //import de xc16
#include <libpic30.h> 
#include <p30F2010.h> //Définition des ports du dsPIC

//double targetCons=65535;
double consOndu=65535;

int sect=0;
int err=0;
int powerTarg=0;
int power=0;

void initTimer(){
    T2CONbits.TCKPS=0b01;
    T2CONbits.TON=1;
    T3CONbits.TCKPS=0b01;
    T3CONbits.TON=1;
	TMR2=0;
    TMR3=0;
}
void initPWM(){ //0-230
    PTCONbits.PTOPS=0;
	PTCONbits.PTCKPS=0;
	PTCONbits.PTMOD=0;
	
	PTMR=0;
	
	PTPER = 114;//16kHZ
        
	PWMCON1bits.PMOD3=0;
	PWMCON1bits.PMOD2=0;
	PWMCON1bits.PMOD1=0;
	
	PWMCON1bits.PEN1H=1;
	PWMCON1bits.PEN2H=1;
	PWMCON1bits.PEN3H=1;
	
	PWMCON1bits.PEN1L=1;
	PWMCON1bits.PEN2L=1;
	PWMCON1bits.PEN3L=1;
    PDC1=power;
	PDC2=power;
	PDC3=power;
}
void updatePWM(){
    PDC1=power;
	PDC2=power;
	PDC3=power;
    switch(sect){
        case 0:
            OVDCON = 0b0000010100000000;
            sect++;
            break;
        case 1:
            OVDCON = 0b0010010000000000;
            sect++;
            break;
        case 2:
            OVDCON = 0b0010000100000000;
            sect++;
            break;
        case 3:
            OVDCON = 0b0000100100000000;
            sect++;
            break;
        case 4:
            OVDCON = 0b0001100000000000;
            sect++;
            break;
        case 5:
            OVDCON = 0b0001001000000000;
            sect=0;
            break;
	}
}
void initSPI(){
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
}
void updateCons(){
    if(power<50){
        consOndu=65535;
    }
    else if(power>50&&power<231){
        //targetCons=-332*power+81744;
        consOndu=-83*power+20436;
        consOndu*=4;
        //targetCons+=16209;
    }
}
void __attribute__((interrupt(auto_psv))) _MathError(void)
{
	err++;
}
void __attribute__ ((interrupt(auto_psv))) _SPI1Interrupt(void){
	//_LATB0^=1;
	IFS0bits.SPI1IF=0;
	powerTarg= SPI1BUF;
}
void __attribute__ ((interrupt(auto_psv))) _DefaultInterrupt( void )   
{
    
    err++;
    IFS1 = 0;
    IFS0 = 0;
    IFS2 = 0;
}
void __attribute__ ((interrupt(auto_psv))) _INT1Interrupt( void )               
{
    TMR2=0;
    TMR3=0;
    sect=0;
    updateCons();
    updatePWM();
    IFS1bits.INT1IF = 0;
}

void __attribute__ ((interrupt(auto_psv))) _T2Interrupt( void )               
{
    //LATEbits.LATE8^=1;
    TMR2=0;
	updatePWM();
    IFS0bits.T2IF = 0;
}
void __attribute__ ((interrupt(auto_psv))) _T3Interrupt( void )               
{
    PR2=consOndu/6;
    PR3=consOndu;
    TMR3=0;
	updatePWM();
    IFS0bits.T3IF = 0;
}

void main(void) {
	//https://turbofuture.com/misc/Configure-PWM-on-dspic30f-and-dspic33f
    //http://www.isilf.be/Articles/ISILF05p157gramme.pdf
    //http://www.moteurindustrie.com/brushless/technique.html
	TRISE=0;
    TRISB=0;
	PORTE=0;
	TRISDbits.TRISD0=1;
            
	initPWM();
	initTimer();
    initSPI();
	
	PTCONbits.PTEN=1;
    
	OVDCON = 0b0011001100000000;
	
    
	IFS0=0;
    INTCON2bits.INT1EP=1;
    IPC4bits.INT1IP=0b111;
    IPC2bits.SPI1IP=0b111;
   
    SPI1STATbits.SPIEN=1;//Enable Serial port

    IEC0bits.T2IE=1;
    IEC0bits.T3IE=1;
    IEC1bits.INT1IE = 1;
    IEC0bits.SPI1IE=1;
    
	while(1){
        updateCons();
        if(power<powerTarg){
            LATEbits.LATE8=0;
            power++;
        }
        else if(power>powerTarg){
            LATEbits.LATE8=0;
            power--;
        }
        else if(power==powerTarg){
            LATEbits.LATE8=1;
           
        }
       __delay_ms(80);
   
    }
	
}
