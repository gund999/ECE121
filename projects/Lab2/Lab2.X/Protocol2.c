
/* Template ver 1.0 W2022
 * File:   Protocol2.c
 * Author: Evan Draeger>
 * Brief: 
 * Created on <2> <9>, <2023>, <4> <pm>
 * Modified on <month> <day>, <year>, <hour> <pm/am
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#include "Protocol2.h" // The header file for this source file. 
#include "uart.h" //
#include <stdio.h> // 
#include "BOARD.h" //
#include <xc.h> //
#include "sys/attribs.h" //
#include "MessageIDs.h" //
#include <stdio.h> // to use sprintf
#include <stdint.h> // for uints
#include <string.h> // to use strlen

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define HEAD 0xCC
#define TAIL 0xB9
#define baudRate 115200

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    STANDBY, READ_len, READ_ID, READ_payload, READ_checksum, BUILD
} STATES;

static int PktReady = 0;
static rxpADT ProtocolPacket;

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/


/*******************************************************************************
 * GLOBAL DATA                                                                 *
 ******************************************************************************/
static int state = STANDBY;
static packBuff packets;
static unsigned char cksum; // pass by ref
// Initializes cksums for sending and recieving packets
static unsigned char cksumSend;
static unsigned char cksumRecieve;

unsigned char BSDChecksum(unsigned char cksum, unsigned char str) { // also needs an input aray
    //static unsigned char cksum;
    //static int j; // checksum counter
    cksum = (cksum >> 1) + (cksum << 7); //  circular rotation 
    cksum += str; //add byte
    //j++;
    return cksum;
}

int Protocol_Init(unsigned long baudrate) {
    Uart_Init(baudrate);
    packets.head = 0; // need to use "." instead of "->" b/c struct w/in struct?
    packets.tail = 0;
    LEDS_INIT(); // init LEDS
    LEDS_SET(0x00); // makes LEDS 0000 0000 default(all off)
    state = STANDBY; // make sure state machines are back in right spot
    cksumSend = 0;
    cksumRecieve = 0;
    //char message[MAXPAYLOADLENGTH];
    //sprintf(message, "Evan Draeger's protocol.c");
    //Protocol_SendDebugMessage(message);
}

// UNUSED FUNCS
// Saw this after I made BSD checksum then didnt want to change vars
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum);
uint8_t Protocol_QueuePacket(void); //Unused, done in build state manually
int Protocol_GetInPacket(uint8_t *type, uint8_t *len, unsigned char *msg); //Unused, part of state machine, different for each
unsigned char Protocol_ReadNextPacketID(void); //unused, idk why use this
void flushPacketBuffer(void); //unused, i change state or lose packet when error

int Protocol_SendDebugMessage(char *Message){
    char debugArray[MAXPAYLOADLENGTH];
    sprintf(debugArray, "%s  -- Compiled on %s %s\n", Message, __DATE__, __TIME__);
    unsigned char msgLen = strlen(debugArray);
    Protocol_SendPacket(msgLen, ID_DEBUG, debugArray);
    return SUCCESS;
}


int Protocol_SendPacket(unsigned char len, unsigned char ID, unsigned char *Payload) {    
    // Sending a Full Message: Head, Length, ID, Payload, Tail, Checksum, END:
    if (PutChar(HEAD)==ERROR){
        return ERROR;
    }
    if (PutChar(len)==ERROR){
        return ERROR;
    }
    if (PutChar(ID)==ERROR){
        return ERROR;
    }
    cksumSend = BSDChecksum(cksumSend, ID);
    
    int check;
    for (int i = 0; i < len-1; i++) {
        if (PutChar(Payload[i])==ERROR){
            return ERROR;
        }
        cksumSend = BSDChecksum(cksumSend, Payload[i]);
        check = i;
    }
    
    if (PutChar(TAIL)==ERROR){
        return ERROR;
    }
    if (PutChar(cksumSend)==ERROR){
        return ERROR;
    }
    PutChar('\r');
    PutChar('\n');
    cksumSend = 0; // Clear checksum
    if (len = check + 1) {
        return SUCCESS;
    } else {
        return ERROR;
    }
}
/*
 swaps bytes, unused tho,, because the way i formatted the arguments means when
 * it swaps in the formatting and this eqn becomes useless
 * array a[8] of 0000 0000 becomes
 * int of value 00000000
 * which is converted back to array, but the way i did ordering makes this 
 * pointless to call
 */
unsigned int convertEndian(unsigned int num) {
    unsigned int swapped =((num>>24)&0xff) | // move byte 3 to byte 0
            ((num<<8)&0xff0000) | // move byte 1 to byte 2
            ((num>>8)&0xff00) | // move byte 2 to byte 1
            ((num<<24)&0xff000000); // byte 0 to byte 3
    return swapped;
    
}

// copied from uart.c, returns state of packet buffer, ERROR means full
int packState(packBuff *buffer3) {
    if ((((buffer3->head) + 1) % PACKETBUFFERSIZE) == buffer3->tail) { 
        return ERROR;
    } else if ((buffer3->tail) == (buffer3->head)) { 
        return SUCCESS;
    }
    return SUCCESS;
}
/**
 * @Function someFunction(void)
 * @param rxPacket, pointer packet built by this func
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author Evan Draeger
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
uint8_t BuildRxPacket(rxpADT *rxPacket, unsigned char ch, unsigned char reset) {
    static int i;
    
    switch (state) {
    case STANDBY:
        i = 0; // reset counter for payload
        if (ch == HEAD) {
            state = READ_len;
            break;
        } else {
            break;
        }
    case READ_len: //read each of these in a dif state
        rxPacket->len = ch;
        state = READ_ID;
        break;
    case READ_ID:
        rxPacket->ID = ch;
        // Calculate checksum
        cksumRecieve = BSDChecksum(cksumRecieve, ch);
        state = READ_payload;
        break;

    case READ_payload:
        // what about OR encounter tail? 0x0D
        if (i < (rxPacket->len) - 1) {
            rxPacket->payLoad[i] = ch;
            // Calculate checksum
            cksumRecieve = BSDChecksum(cksumRecieve, ch);
            i++;
            break;
        }
        state = READ_checksum;

        break;

    case READ_checksum:
        rxPacket->checkSum = ch;
        // Verify built Checksum is the same as the received value
        if (rxPacket->checkSum == cksumRecieve) {
            state = BUILD;
        } else {
            state = STANDBY;
        }
        cksumRecieve = 0; // Clear checksum
        break;

    case BUILD:
        if (packState(&packets) != ERROR) {
            packets.packItems[packets.head] = *rxPacket;
            packets.head = ((packets.head)+1) % PACKETBUFFERSIZE;
            state = STANDBY;
            PktReady = 1;
            break;
        }
        
    }
    
}


// Parses the packet from BuildRxPacket(), and decides what to do
uint8_t Protocol_ParsePacket(void) {
    if (packets.tail == packets.head) {
        return ERROR;
    }
    // inside packet buffer, takes packet at tail and checks ID
    rxpADT parsePack = packets.packItems[packets.tail];
    packets.tail = ((packets.tail)+1) % PACKETBUFFERSIZE;
    uint8_t LEDS_ARE;
    
    switch (parsePack.ID) {
        case ID_INVALID:
            PktReady = 0;
            break;
        case ID_LEDS_SET:
            //uint8_t leds_on = parsePack.payLoad;
            LEDS_SET(parsePack.payLoad[0]);
            PktReady = 0;
            break;
        case ID_LEDS_GET:
            LEDS_ARE = LEDS_GET();
            uint8_t LED_array[1];
            LED_array[0] = LEDS_ARE;
            // 1 byte ID, 1 byte payload(LED states)
            Protocol_SendPacket(0x02, ID_LEDS_STATE, LED_array); 
            PktReady = 0;
            break;
        case ID_PING:
            ; // idk why this works but it fixes a declaration error in the next line
            // Put Payload Array into unsigned int 
            unsigned int preEndLoad;
            
            // Turns payload array into single value, eg: [0,0,0,1] to 0001
            for (int k = 0; k < (parsePack.len-1); k++) { // Also converts from Big to Little Endian
                preEndLoad *= 256; // Add a 00 in hex
                preEndLoad += parsePack.payLoad[k];
            }
            // vestige code from Endian conversion being removed
            unsigned int postEndLoad = preEndLoad; 
            // Right Shift to divide by 2
            postEndLoad = postEndLoad >> 1;
            // Make array again to send as payload out
            unsigned char pongArray[4];
            
            pongArray[3] = postEndLoad; // Also converts from Little to Big Endian
            pongArray[2] = postEndLoad >> 8;
            pongArray[1] = postEndLoad >> 16;
            pongArray[0] = postEndLoad >> 24;
            
            Protocol_SendPacket(0x5, ID_PONG, pongArray); // 1 byte ID, 4 bytes
            PktReady = 0;
            
            break;
    }
}

// Main function that uses protocol functions to recieve/send packets
void runProtocol() {
    //static rxpADT ProtocolPacket;
    static unsigned char reset;
    unsigned char ch;
    
    //    while (BuildRxPacket(ProtocolPacket, reset) == ERROR) {}
    //    Protocol_ParsePacket();


    if (GetChar(&ch) == SUCCESS) {
        BuildRxPacket(&ProtocolPacket, ch, reset);
    }
    while (PktReady == 1) {
        Protocol_ParsePacket();
    }

}

//int main() {
//    BOARD_Init();
//    Protocol_Init(baudRate);
//
//    while (1) {
//
//        runProtocol();
//    }
//    return (EXIT_SUCCESS);
//}