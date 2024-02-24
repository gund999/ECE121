/* Template ver 1.0 W2022
 * File:   FreeRunningTimer.c
 * Author: Evan Draeger
 * Brief: 
 * Created on <March> <5>, <2023>, <11> <pm>
 * Modified on <month> <day>, <year>, <hour> <pm/am
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "BOARD.h"
#include "FreeRunningTimer.h" // The header file for this source file. 
#include <sys/attribs.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Protocol2.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define SYSTEM_CLOCK 80000000L
#define PB_CLOCK SYSTEM_CLOCK/2
#define BAUDRATE 115200
#define test

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES                                                            *
 ******************************************************************************/
static unsigned int millitime;
static unsigned int microtime;
/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
void FreeRunningTimer_Init(void) {
    BOARD_Init();
    T5CONbits.ON = 0; // clear Timer 5
    //T3CON.ON = 1;
    T5CONbits.TCKPS = 0b011; // 1:8 Prescalar
    TMR5 = 0x0; // Load/Clear the timer register TMRx.
    PR5 = 0x1387; // Load the period register PRx with the desired 16-bit match value
    IFS0bits.T5IF = 0; //  Clear the TxIF interrupt flag bit in the IFSx register.
    IPC5bits.T5IP = 0b011; // Configure the interrupt priority to 3(below protocol) 
    IPC5bits.T5IS = 0b0; //and subpriority levels in the IPCx register(sub = 3)
    IEC0bits.T5IE = 1; // Set the TxIE interrupt enable bit in the IECx register.
    
    T5CONbits.ON = 1; // enable Timer 5
}

//// Tests if Interrupt Flag is up, rolls over each 
//if (IFS0bits.T5IF == 1) {
//    
//}


unsigned int FreeRunningTimer_GetMilliSeconds(void){     
    return millitime;
}

unsigned int FreeRunningTimer_GetMicroSeconds(void) {
    microtime = millitime + (TMR5/5);
    return microtime;
}

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/


void __ISR(_TIMER_5_VECTOR) Timer5IntHandler(void) {
    // Tests if Interrupt Flag is up, rolls over each 
    if (IFS0bits.T5IF == 1) {
        millitime++;       // Increment millisecond counter when ISR Flag = 1
        //microtime += 1000; // Increment microsecond by 1k when ISR Flag = 1
        IFS0bits.T5IF = 0; // Clear Flag
    }
    
}
#ifdef test
int main() {
    int x = 0;
    FreeRunningTimer_Init();
    Protocol_Init(BAUDRATE);
    char message[128];
    unsigned int lastTime = 0;
    int LEDswap = 0;
    int timeCount = 2;
    sprintf(message, "Evan Draeger's FreeRunningTimer");
    Protocol_SendDebugMessage(message);
    while (1) {
        //LEDS_SET(0xFF);
        if ((FreeRunningTimer_GetMilliSeconds() - lastTime) >= 2000) { // each 2 [s]
            char milliarray[128];
            // Is it a problem that there's a 3[ms] delay each time it prints?
            // Prob not b/c takes a LOT of time for it to cause errors
            sprintf(milliarray, "Time: %i milliseconds, %i microseconds", FreeRunningTimer_GetMilliSeconds(), FreeRunningTimer_GetMicroSeconds());
            Protocol_SendDebugMessage(milliarray);
            timeCount += 2;
            lastTime = FreeRunningTimer_GetMilliSeconds();
            // Toggles LEDS and External Pin each time it passes through
            if (LEDswap == 1) {
                LEDS_SET(0x00);
                LEDswap--;
            } else if (LEDswap == 0) {
                LEDS_SET(0xFF);
                LEDswap++;
            }
        }

        //Protocol_SendDebugMessage("Hewo");
        runProtocol();
    }
    return (EXIT_SUCCESS);
}
#endif 