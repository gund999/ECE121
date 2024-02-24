/* 
 * File:   part2.c
 * Author: eadra
 *
 * Created on January 12, 2023, 4:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include <xc.h>
// 3125 if you want a period for the lowest bit light to be 250ms, but its less
// than a minute for total counting to 256
#define NOPS_FOR_5MS 6000

/*
 * 
 */
// copied from notes, void b/c dont need smtn to return
void NOP_delay() {
    int i;
    for (i = 0; i < NOPS_FOR_5MS; i++) {
        asm("nop");
    }
}

int main(int argc, char** argv) {
    uint8_t LEDbit = 0b00000000;
    // TRISx Enables all LEDs by setting pins in port E to outputs
    // LEDs 0 to 7 from LtR
    TRISE = 0x00; // 0000 0000
    // LATx sets vales for pins
    // Turns off all LEDs(0 is off 1 is on)
    // Hex in Binary represents which lights on and off
    LATE = 0x00; // 0000 0000
    int loop_count = 0;
    while (1) {
        loop_count++;
        // Wait 50ms 50 times to reach 250ms per LED increment
        if (loop_count == 50) {
            LEDbit++;
            loop_count = 0;
        }
        LATE = LEDbit;
        // Buttons reset counter
        if ((PORTF & 0x02) | (PORTD & 0x20) | (PORTD & 0x40) | (PORTD & 0x80)) {
            LEDbit = 0x00;
        }
        // When LEDS all on(counter full), reset to zero
        if (LEDbit > 0xff) {
            LEDbit = 0x00;
        }
        NOP_delay();
    }


    return (EXIT_SUCCESS);
}

