#ifndef SERIAL
#define SERIAL

#include <stdio.h>
#include <unistd.h>		// read(), write(), close()	
#include <cstring>
#include <iostream>		// standard I/O
#include <signal.h>		// handle errors
#include <fcntl.h>		// File controls
#include <errno.h>		// Error integer and strerror()
#include <termios.h>		// POSIX terminal control definition


class Serial {
	public:
	Serial(char* port, int baudrate);
	char* readLine();
	char readByte();
	~Serial();

	private:
	int _handle;
	struct termios _options;
	char _buf[256];
};

#endif
