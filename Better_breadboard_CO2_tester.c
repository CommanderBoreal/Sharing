/*
 * File:   Better_breadboard_CO2_tester.c
 * Author: Clayton Stewart
 *
 * Created on June 2, 2017, 3:42 AM
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

void halfDelay();
void delay();
void duoDelay();
void triDelay();
void quadDelay();
void upshift();
void displayOutput(int shiftNum);
void finalDisplay(int shiftNum);

void main(void) 
{
    //RB7 is for giving pulses
    //RA0 is arm extesnsion
    //RA1 is arm retract
    //rc0-rc7 and rb4-rb6 are for outputting the number of shifts
    //RA5 is for accepting the shift pulses
   
    int i = 1;
    int currentShiftNumber = 0;
    int previousShiftNumber = 0;
    
    OSCCON = 0x68;
    //configure c register to act as output
    
    //-----------Initialize Timer0
    OPTION_REG = 0X87;
    
    //sets up timer 1 module as counter, puts gate on pin RA4
    T1GSEL = 0;
    T1CON = 0b10000101;
    T1GCON = 0b11000000;
    
    //disables interrupts
    INTCON = 0;
    
    ANSELA = 0; //digital
    ANSELB = 0; //digital
    ANSELC = 0; //digital
    
    TRISA5 = 1; //sets a5 to accept pulses
    TRISA4 = 1; //sets a3 to accept gate 
    TRISA2 = 0;
    TRISA1 = 0; //OUTPUT
    TRISA0 = 0; //OUTPUT
    TRISB = 0; //OUTPUT
    TRISC = 0; //OUTPUT
    
    TRISB5 = 1; //button acts as input
    
    LATC = 0;
    
    RB7 = 0;
    //open timer1 counter
#asm
_asm
    BANKSEL TMR1H
    MOVLW 0x00
    MOVWF TMR1H
    BANKSEL TMR1L
    MOVWF TMR1L
    BANKSEL T1CON
    BSF T1CON, 0
_endasm
#endasm
    
    while(i == 1) //main loop
    {
        previousShiftNumber = currentShiftNumber;
        
        RA1 = 1;
        quadDelay();
        quadDelay();
        RA1 = 0;
        quadDelay();
        RA0 = 1;
        duoDelay();
        RB7 = 1;
        delay();
        RB7 = 0;
        delay();
        quadDelay();
        RA0 = 0;
        quadDelay();
        
        currentShiftNumber = ((TMR1H<<8)+TMR1L);
        displayOutput(currentShiftNumber);
        
        if(currentShiftNumber == previousShiftNumber)
        {
            i = 0;
        }
    }
    
while(1)
{
    finalDisplay(previousShiftNumber);
}
    
    return;
}

void halfDelay()
{
        //tmr0 used to have a slight delay
    T0IF=0;//T0IF is the timer0 interrupt
    TMR0=7;//sets the timer to 7, half of the timer0 register
    while(T0IF==0)
    {
        continue;
    }
    return;
}

void delay()
{
    //tmr0 used to have a slight delay
    T0IF=0;//T0IF is the timer0 interrupt
    TMR0=0;//sets the timer to zero
    while(T0IF==0)
    {
        continue;
    }
    return;
}

void duoDelay()
{
    delay();
    delay();
}

void triDelay()
{
    delay();
    delay();
    delay();
}

void quadDelay()
{
    delay();
    delay();
    delay();
    delay();
    
}

void upshift()
{
    RA0 = 1;
    delay();
    RB7 = 1;
    RB7 = 0;
    delay();
    RA0 = 0;
}

void displayOutput(int shiftNum)
{
        RC0 = (shiftNum & 0b1);
        RC1 = (shiftNum >>1) & 0b1;
        RC2 = (shiftNum >>2) & 0b1;
        RC3 = (shiftNum >>3) & 0b1;
        RC4 = (shiftNum >>4) & 0b1;
        RC5 = (shiftNum >>5) & 0b1;
        RC6 = (shiftNum >>6) & 0b1;
        RC7 = (shiftNum >>7) & 0b1;
        RB4 = (shiftNum >>8) & 0b1;
        RB6 = (shiftNum >>9) & 0b1;
        RA2 = (shiftNum >>10) & 0b1;
}

void finalDisplay(int shiftNum)
{
    if(RB5 == 1)
    {
        RC0 = (shiftNum & 0b1);
        RC1 = (shiftNum >>1) & 0b1;
        RC2 = (shiftNum >>2) & 0b1;
        RC3 = (shiftNum >>3) & 0b1;
        RC4 = (shiftNum >>4) & 0b1;
        RC5 = (shiftNum >>5) & 0b1;
        RC6 = (shiftNum >>6) & 0b1;
        RC7 = (shiftNum >>7) & 0b1;
        RB4 = (shiftNum >>8) & 0b1;
        RB6 = (shiftNum >>9) & 0b1;
        RA2 = (shiftNum >>10) & 0b1;
    }
    else
    {
        RC0 = (shiftNum >>11)& 0b1;
        RC1 = (shiftNum >>12) & 0b1;
        RC2 = (shiftNum >>13) & 0b1;
        RC3 = (shiftNum >>14) & 0b1;
        RC4 = (shiftNum >>15) & 0b1;
        RC5 = (shiftNum >>16) & 0b1;
        RC6 = 0;
        RC7 = 0;
        RB4 = 0;
        RB6 = 0;
        RA2 = 1;
        duoDelay();
        RA2 = 0;
        duoDelay();
        RA2 = 1;
    }
}