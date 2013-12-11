Arduino library for a simple command interface over serial

See LICENSE.txt


This code implements the slave end.  See:

https://github.com/RyanSchuster/ControlBoard.git

for an example of its use. See:

https://github.com/RyanSchuster/AzIncHost.git

for an example of the master end of the interface and its use.



Overview of the protocol:

The protocol works by transferring values between the master
and the slave's internal "registers."

The master sends a one-byte command to the slave, followed by
a one-byte "register" ID.  The command can be either a read or
a write, and registers are up to the software using the library.

If the command is a write, the master follows the register ID
with a fixed number of bytes to write, and the slave responds
with an ACK or NACK byte.  Then the command is complete.

If the command is a read, the slave responds after the register
ID with a fixed number of bytes, and the master responds with
an ACK or NACK byte.  Then the command is complete.



Protocol specifics:

The fixed number of bytes to read/write is called the BURST,
and is currently fixed at 32.

The read and write commands are 'r' and 'w', respectively.
The ACK and NACK bytes are 'k' and '!', respectively.



Software structure:

This library uses the serialEvent() function for its polling
of the serial port, and relies on the user of the library to
implement their own commandRead(), commandWrite(), and
commandError() functions.



API:

commandInit()
	Library implemented, user calls to initialize the
	command interface.  No arguments, no return value.

serialEvent()
	Library implemented, called automagically after each
	iteration of the arduino's main program loop.  Polls
	the serial port for commands and data.

commandRead()
	User implemented, called by the library when a read
	command is completed.  Gets the register ID and a
	pointer to a buffer of size BURST as arguments.
	Here, the user handles the read command, writes data
	to return into the buffer, and returns True for
	success and False for error.  The return value is
	currently ignored.

commandWrite()
	User implemented, called by the library when a write
	command is completed.  Gets the register ID and a
	pointer to a buffer of size BURST as arguments.
	Here, the user handles the write command, reads data
	from the buffer, and returns True for to respond
	with ACK or False for NACK.

commandError()
	User implemented, called by the library when a
	protocol error occurs.  Can be used to clean things
	up and whatnot.



TODO:

Handle errors and timeouts gracefully
Give the user more freedom in terms of serial interfaces