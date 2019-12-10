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
	bool isOpen();
	char* readLine();
	char readByte();
	~Serial();

	private:
	void connect();
	char* _port;
	int _baudrate;
	int _handle;
	bool _isOpen;
	struct termios _options;
	char _buf[10*1024];
};

#endif
