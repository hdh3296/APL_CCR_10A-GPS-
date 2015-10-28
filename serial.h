#ifndef _SERIAL_H_
#define _SERIAL_H_

#define BAUD 19200      
//#define FOSC 8000000L
#define FOSC 32000000L
#define NINE 0     /* Use 9bit communication? FALSE=8bit */
#define OUTPUT 0
#define INPUT 1

#define DIVIDER ((int)(FOSC/(16UL * BAUD) -1))
#define HIGH_SPEED 1

#if NINE == 1
#define NINE_BITS 0x40
#else
#define NINE_BITS 0
#endif

#if HIGH_SPEED == 1
#define SPEED 0x4
#else
#define SPEED 0
#endif

/*
void init_comms(void);
void putch(unsigned char);
unsigned char getch(void);
unsigned char getche(void);
*/


//////////////////////////////////////////////////
//////////////////////////////////////////////////

#define         EOT             0x04
#define         ETX             0x03
#define         ENQ             0x05
#define         ACK             0x06


#define         STX_CHK         0
#define         ADDRESS_CHK     1
#define         COMMAND_CHK     2
#define         DATACNT_CHK     3
#define         DATA_CHK        4
#define         CHKSUM_CHK      5
#define         ETX_CHK         6
#define         RX_ERROR        7
#define         RX_GOOD         8
#define         TX_SET          9
#define         CHKSUM_LOW_CHK  10
#define         CHKSUM_HIGH_CHK 11
#define         TX_ALL          12

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

#endif
