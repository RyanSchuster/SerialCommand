/*

Arduino library for a simple command interface over serial

See README.txt and LICENSE.txt

*/

#include "SerialCommand.h"


/* command interface constants */
#define COMSTATE_COM 0		/* waiting for a command */
#define COMSTATE_REG 1		/* waiting for a register */
#define COMSTATE_WRITE 2
#define COMSTATE_WAITACK 3

#define COM_READ 0
#define COM_WRITE 1

#define ACK 'k'
#define NACK '!'

#define LED 2


/* command interface state variables */
byte comState;
byte command;
byte count;
byte reg;
byte buffer[BURST];
byte errorCode;


/* command events */
void commandInit()
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

	comState = COMSTATE_COM;
	count = 0;
	Serial.begin(BAUD);
	Serial.println("Ready!");
}


void serialEvent()
{
	while (Serial.available())
	{
		char inChar = Serial.read();

		switch (comState)
		{
		case COMSTATE_COM:
			command = inChar;
			comState = COMSTATE_REG;
			break;
		case COMSTATE_REG:
			reg = inChar;
			count = 0;
			comState = COMSTATE_COM;
			if (command == 'r')
			{
				commandRead(reg, buffer);
				Serial.write(buffer, BURST);
				comState = COMSTATE_WAITACK;
			}
			else if (command == 'w')
			{
				comState = COMSTATE_WRITE;
			}
			else
			{
				// wat
				comState = COMSTATE_COM;
			}
			break;
		case COMSTATE_WAITACK:
			if (inChar == ACK)
			{
				// acked
			}
			else if (inChar == NACK)
			{
				// nacked
			}
			else
			{
				// this is really bad
			}
			comState = COMSTATE_COM;
			break;
		case COMSTATE_WRITE:
			buffer[count] = inChar;
			if (++count == BURST)
			{
				if (commandWrite(reg, buffer))
					Serial.write(ACK);
				else
					Serial.write(NACK);
				comState = COMSTATE_COM;
			}
			break;
		default:
			comState = COMSTATE_COM;
			break;
		}
	}
}