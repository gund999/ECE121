/* 
 * File:   part1.c
 * Author: eadra
 *
 * Created on January 12, 2023, 3:00 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include <xc.h>

/*
 * 
 */
int main(int argc, char** argv) {
    // TRISx Enables all LEDs by setting pins in port E to outputs
    // LEDs 0 to 7 from LtR
    TRISE = 0x00; // 0000 0000
    // LATx sets vales for pins
    // Turns off all LEDs(0 is off 1 is on)
    // Hex in Binary represents which lights on and off
    LATE = 0x00;  // 0000 0000
   
   while (1) {
        // PORTF bit 2 is btn1
        if (PORTF & 0x02) {
            LATE = 0b00000011;
        }
        // PORTD bit 5 is btn2
        else if (PORTD & 0x20) {
            LATE = 0b00001100;
        }
        // PORTF bit 6 is btn3
        else if (PORTD & 0x40) {
            LATE = 0b00110000;
        }
        // PORTF bit 7 is btn4
        else if (PORTD & 0x80) {
            LATE = 0b11000000;
        }
        else {
            LATE = 0b00000000;
        }
    }
    return (EXIT_SUCCESS);
}

