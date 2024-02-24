/* 
 * File:   maintest.c
 * Author: eadra
 *
 * Created on January 25, 2023, 4:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include <xc.h>
#include "Protocol2.h"
#include "BOARD.h"
#include "MessageIDs.h"
/*
 * 
 */
#define baudRate 115200
#define pt2



int main() {
    BOARD_Init();
    
    Uart_Init(BAUD);


// FOR PT 1
#ifdef pt1
    while (1) {
        if (U1STAbits.URXDA) {
            int x = 0;
            U1TXREG = U1RXREG;

        }
    }
#endif
    
// FOR PT 2
#ifdef pt2
    unsigned char foo2;
    unsigned char foo3;
    
    while(1) {
        if (GetChar(&foo2) == SUCCESS) {

            PutChar(foo2);
        }
    }
#endif

// FOR PT 3    
#ifdef pt3
    Protocol_Init(baudRate);
    while(1) {
        
        runProtocol();
    }
    
#endif

    return (EXIT_SUCCESS);
}
