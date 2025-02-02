#include "msp430fr2355.h"
#include <msp430.h> 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define SLAVE_ADDR  0x22
char dataIn;

char sendPacket[] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};

void I2C_INIT();
void initGPIO();

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //For brownout reset

    UCB0CTLW0 = UCSWRST;                      // Enable SW reset
    UCB0CTLW0 |= UCMODE_3 + UCSYNC;                      // I2C Master, synchronous mode
    UCB0I2COA0 = 0x22 | UCOAEN;                   // 0x48
    UCB0CTLW0 &= ~UCSWRST;                     // Clear SW reset, resume operation

    P1OUT = 0x00;                             // P1 setup for LED & reset output
    P1DIR |= BIT0;

    P1SEL1 &= ~BIT2;
    P1SEL0 |= BIT2;
    P1SEL1 &= ~BIT3;                     // P3.1,2 option select
    P1SEL0 |= BIT3;

    __delay_cycles(1000);

    UCB0IE |= UCTXIE;
    UCB0IE |= UCRXIE;
    __enable_interrupt();
    _BIS_SR(GIE); //Enable global interrupts.

    while (1) {
        UCB0IE |= UCRXIE0; //Enable receive interrupt

        __delay_cycles(10000);

    }
    return 0;
}
//-----------------------------------END MAIN-------------------------------

//--------------------------------------------------------------
// I2C ISR
//--------------------------------------------------------------
#pragma vector = EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void) {
    P1OUT ^= 0x01;
    /*
    int i, k;

    for(k = 0; k < 1000; k++)
    {
        for(i = 0; i < 6; i++)
        {
            UCB0TXBUF = sendPacket[i];
            __delay_cycles(100);
        }
    }
    */

    switch(UCB0IV){
    case 0x16:
        dataIn = UCB0RXBUF;
        break;
    case 0x18:
        UCB0TXBUF = 0x11;
        break;
    }

    //UCB0TXBUF = sendPacket[1];
    //__delay_cycles(100);
}








