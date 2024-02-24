///* 
// * File:   l2main.c
// * Author: eadra
// *
// * Created on March 5, 2023, 11:53 PM
// */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include "FreeRunningTimer.h"
//#include "BOARD.h"
//#include "Protocol2.h"
//#include <string.h>
//#define BAUDRATE 115200
//
///*
// * 
// */
//int main() {
//    int x = 0;
//    FreeRunningTimer_Init();
//    Protocol_Init(BAUDRATE);
//    char message[128];
//    unsigned int lastTime = 0;
//    int LEDswap = 0;
//    int timeCount = 2;
//    sprintf(message, "Evan Draeger's FreeRunningTimer");
//    Protocol_SendDebugMessage(message);
//    while (1) {
//        //LEDS_SET(0xFF);
//        if ((FreeRunningTimer_GetMilliSeconds() - lastTime) >= 2000) { // each 2 [s]
//            char milliarray[128];
//            // Is it a problem that there's a 3[ms] delay each time it prints?
//            // Prob not b/c takes a LOT of time for it to cause errors
//            sprintf(milliarray, "Time: %i milliseconds, %i microseconds", FreeRunningTimer_GetMilliSeconds(), FreeRunningTimer_GetMicroSeconds());
//            Protocol_SendDebugMessage(milliarray);
//            timeCount += 2;
//            lastTime = FreeRunningTimer_GetMilliSeconds();
//            // Toggles LEDS and External Pin each time it passes through
//            if (LEDswap == 1) {
//                LEDS_SET(0x00);
//                LEDswap--;
//            } else if (LEDswap == 0) {
//                LEDS_SET(0xFF);
//                LEDswap++;
//            }
//        }
//
//        //Protocol_SendDebugMessage("Hewo");
//        runProtocol();
//    }
//    return (EXIT_SUCCESS);
//}
//
