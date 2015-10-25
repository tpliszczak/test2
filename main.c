/* 
 * File:   main.c
 * Author: Tomasz Pliszczak
 *
 * Created on 22 Pazdziernik 2015, 19:01
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = HS    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is enabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (VCAP pin function disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = ON      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)



 volatile unsigned int timer1;
 volatile unsigned int timer2;
 volatile unsigned int timer3;
 volatile unsigned int timer4;
 volatile unsigned int timer5;
 volatile unsigned char przycisk;
 volatile unsigned char przycisk_tmp;
 volatile unsigned char buzer;
volatile unsigned char Hz;
volatile unsigned char ramkaTX[256];
volatile unsigned char znakTX;
volatile unsigned long multi;
volatile unsigned char flagPomiarStart;
volatile unsigned char flagPomiarStop;
volatile unsigned long wynikHz;
volatile unsigned char n;
volatile unsigned char wynikHzTmp;
volatile unsigned char i;

//funkcje
void USART_init(void);
void USART_putc(unsigned char c );
void USART_puts(volatile unsigned char *s,unsigned char l);


//git


int main(int argc, char** argv) {

    //8MHz
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF3 = 1;
    
 


    //4s watchdog
    // WDTCONbits.WDTPS0=0;
    //WDTCONbits.WDTPS1=0;
    //WDTCONbits.WDTPS2=1;
    //WDTCONbits.WDTPS3=1;
    //WDTCONbits.WDTPS4=0;
    
    OPTION_REGbits.INTEDG = 0;  // falling edge


    //timer0
    OPTION_REGbits.PS0=0;  //pre8 010
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=0;
    //OPTION_REGbits.TMR0CS = 0;  //intosc/4
    OPTION_REGbits.PSA =  1;  //prescaler
    
    OPTION_REGbits.TMR0CS = 1; //pin T0CKI
    

    INTCONbits.TMR0IE = 1;  //interrupt enable

    //timer1
    T1CONbits.TMR1ON = 1; //enable
    T1CONbits.T1CKPS0 = 0;
    T1CONbits.T1CKPS1 = 0;//prescaler 8
    
    T1CONbits.TMR1CS0 = 0;   
    T1CONbits.TMR1CS1 = 0;    
    
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    


    //tomer2 PWM

     TRISCbits.TRISC2 = 1; //in

     
//     CCP1CONbits.CCP1M0 = 0; //pwm mode
//     CCP1CONbits.CCP1M1 = 0;
//     CCP1CONbits.CCP1M2 = 1;
//     CCP1CONbits.CCP1M3 = 1;  
     
//     CCP1CONbits.DC1B0 = 1; //duty 2 lsb
//     CCP1CONbits.DC1B1 = 1;
     
     PR2 = 244;// 78; //
     //CCPR1L = 5;
     
     //PIR1bits.TMR2IF = 0;//flaga
     
     T2CONbits.T2CKPS0 = 1;
     T2CONbits.T2CKPS1 = 1;  //preskaler 4
     T2CONbits.TMR2ON = 1;  // on
     
     T2CONbits.T2OUTPS0 = 1;
     T2CONbits.T2OUTPS1 = 1;     
     T2CONbits.T2OUTPS2 = 1;     
     T2CONbits.T2OUTPS3 = 1;
     
     TRISCbits.TRISC2 = 0; //in
     PIE1bits.TMR2IE = 1;
     
     
     INTCONbits.GIE = 1;  //interrupt glob enable 
          
    
    //RB0-2  interupt
    //IO RB0-5
    ANSELBbits.ANSB0 = 0;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    ANSELBbits.ANSB4 = 0;
    ANSELBbits.ANSB5= 0;

    //RB0-2 wyjscia
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;

    //RA0-7----------------puste piny
    //IO 
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA3 = 0;
    ANSELAbits.ANSA5 = 0;

    //RA0-7 wyjscia
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 0;
    TRISAbits.TRISA6 = 0;
    TRISAbits.TRISA7 = 0;
    
    //niski
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    LATAbits.LATA6 = 0;
    LATAbits.LATA7 = 0;


    //RB3 dioda status
    ANSELBbits.ANSB3 = 0;  // dida power
    TRISBbits.TRISB3 = 0; // out
    LATBbits.LATB3 = 0;  //
    //RB4-7  IN plc
    //IO RB4-7

    ANSELBbits.ANSB4 = 0;
    ANSELBbits.ANSB5 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    
    
    
    //wejscie przycisku
    TRISBbits.TRISB5 = 1;
    //RB5 pull up
    OPTION_REGbits.nWPUEN = 0;  // enable all pull up
    WPUBbits.WPUB5 = 1;
    
    IOCBNbits.IOCBN5 = 1;
    
    INTCONbits.IOCIE = 1;  //interrupt enable



    // RC LED output
    TRISC = 0; // pins is output
    LATC = 0; // pins low
    ANSELC = 0;  // digital io
    
    TRISCbits.TRISC6 = 1; //in

    //dioda led
    ANSELAbits.ANSA5 = 0;
    TRISAbits.TRISA5 = 0;
    LATAbits.LATA5 = 0;

    //temp
   // FVRCONbits.TSEN = 1;
    //FVRCONbits.FVREN = 0;
    //FVRCONbits.TSRNG = 1;

  //  ADCON0bits.ADON = 1;
  //  ADCON0bits.CHS = 0b11110;
  //  ADCON1 |= 0b10000000;
    
    
    USART_init();
    Hz = 0;

    while(1){   
                    //LATCbits.LATC4 ^= 1;    
                   
        
        if (timer1==0) {
            timer1 =20;

           //  = Hz++;
            //USART_puts("BMW",3);
                        
           
            Hz = TMR0;
                       
//            USART_puts("Start pomiaru\r\n", strlen("Start pomiaru\r\n")); 
// 
//            sprintf(ramkaTX,"Frequency: %d MHz",Hz);
//            USART_puts(ramkaTX, strlen(ramkaTX)); 
//                           
//            sprintf(ramkaTX,"  Multi: %d ",multi);
//            USART_puts(ramkaTX, strlen(ramkaTX)); 
//            
//            USART_puts("\r\n",2);      

//            if ((flagPomiarStart == 0) && (flagPomiarStop == 0)) {
//                
//                flagPomiarStart =1;
//            }



        }

                   
        if ((flagPomiarStart == 0) && (flagPomiarStop == 0)) {
                
                flagPomiarStart =1;
            }
        
        //pomiar start
        if (flagPomiarStart == 1) {

            USART_puts("Start pomiaru\r\n", strlen("Start pomiaru\r\n")); 

           // TMR1H = 0;
           // TMR1L = 0; 
            multi = 0; //mno¿nik
            TMR0 = 0; 
            TMR2 = 0; //zerowanie timera2
            n=0;

        
            flagPomiarStart =2;
            
        }

        if (flagPomiarStop == 1) {                  
            sprintf(ramkaTX,"Czestotliwosc: %lu ",wynikHz);
            USART_puts(ramkaTX, strlen(ramkaTX)); 
            USART_puts("  Hz", strlen("  Hz")); 
            USART_puts("\r\n",2);       
            flagPomiarStop = 0;  
        }

        //CLRWDT();
    }

    return (EXIT_SUCCESS);
}


//funkcje

void USART_init(void)
{
    
    BAUDCONbits.ABDOVF = 1; 
  
            
    TXSTAbits.TXEN = 1;     // enable transmitter
    TXSTAbits.SYNC = 0;     // asynchronous operation.
    RCSTAbits.SPEN = 1;     // configures the TX/CK I/O pin as an output.
    TXSTAbits.BRGH = 1;     // high speed mode
    
    


    
    // Baud Rate = 9600; SPBRGL 51; 
    SPBRGL = 51;
    // Baud Rate = 9600; SPBRGH 0; 
    SPBRGH = 0x00;
    
    
    //zewnetrzny 20MHz    
    BAUDCONbits.BRG16 = 1; 
        // Baud Rate = 9600; SPBRGL 8; 
    SPBRGL = 8;
    // Baud Rate = 9600; SPBRGH 2; 
    SPBRGH = 2;
}

void USART_putc(unsigned char c)
{
    while (!TXSTAbits.TRMT); // wait until transmit shift register is empty
    TXREG = c;               // write character to TXREG and start transmission
}

void USART_puts(volatile unsigned char *s,unsigned char l)
{
    for(unsigned char i = 0;i < l;i++)
    {
        USART_putc(*s);     // send character pointed to by s
       // *s = 0;  // kasowanie
        s++;                // increase pointer location to the next character
    }


}

//przerwanie

void interrupt ISR(void){
    
    wynikHzTmp = TMR0;
    
        
    if ( INTCONbits.TMR0IF == 1 ) { //1ms
        multi++;
        INTCONbits.TMR0IF = 0;
    }
    
    
    
    
    //timer2
        if (    PIR1bits.TMR2IF == 1) { // co 1ms  
            n++;
            if ( (n>19) && (flagPomiarStart == 2) )  {         
               // wynikHz = TMR0;    
                wynikHz = (9978UL*((multi * 256)+ wynikHzTmp))/10000;   //20MHz 9536UL     
                flagPomiarStart = 0;
                flagPomiarStop = 1;
                n=0;
                            
                LATCbits.LATC4 ^= 1;  //test    
            }
                        
                //LATCbits.LATC4 ^= 1;  //test  

            if (timer1 >0) {
                timer1--;
            }
            if (timer2 >0) {
                timer2--;
            }
            if (timer3 >0) {
                timer3--;
            }
            if (timer4 >0) {
                timer4--;
            }
            if (timer5 >0) {
                timer5--;
            }
            //LATCbits.LATC4 ^= 1;  //test         
            PIR1bits.TMR2IF = 0;
        }



  //przerwanie timer 1
    if (    PIR1bits.TMR1IF == 1) { // co 1ms  
        //koniec pomiaru
//        if (flagPomiarStart == 2) {  
//            wynikHz = TMR0;    
//            wynikHz = (9553UL*((multi * 256)+ wynikHz))/1000;   //20MHz 9536UL     
//            //wynikHz = (3888UL*((multi * 256)+ wynikHz))/1000;  //8MHz 
//            flagPomiarStart = 0;
//            flagPomiarStop = 1;
//        }
        TMR1H = 255;
        TMR1L = 6;
      
        //20MHz
        TMR1H = 253;
        TMR1L = 143;          
        TMR1H = 255;
        TMR1L = 0;  
                  
        LATCbits.LATC5 ^= 1;  //test  
        PIR1bits.TMR1IF = 0;
    }






    //przerwanie int port b
       if ( INTCONbits.IOCIF == 1 ) {
       
           //RB5
           if (IOCBFbits.IOCBF5 == 1) {                  

               if (przycisk == 0) {
                   buzer = 4;
                   timer1 = 1000; //1 sekunda 
                   przycisk = 1;               
                  // LATCbits.LATC4 ^= 1;//testowa dioda
               }

           }
           IOCBF = 0;  //clear all
       }
    
   

}