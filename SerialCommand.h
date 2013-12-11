/*

Arduino library header for a simple command interface over serial

See README.txt and LICENSE.txt

*/

#ifndef __SERIALCOMMAND_H__
#define __SERIALCOMMAND_H__


#include "Arduino.h"


#define BURST 32
#define BAUD 19200


/* this comes with it */
void commandInit();
void serialEvent();

/* make these your damn self */
boolean commandRead(byte reg, byte *buffer);
boolean commandWrite(byte reg, byte *buffer);
void commandError();


#endif /*__SERIALCOMMAND_H__*/