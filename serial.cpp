#include "serial.h"

Serial::Serial(char* port, int baudrate)
{
	_handle = open(port, O_RDWR | O_NOCTTY | O_NDELAY);		// open with R/W, no tty control, open immediately
	if (_handle == -1)
	{
		perror("Unable to open port ______");
	}

	memset(&_buf, '\0', sizeof(_buf));

	fcntl(_handle, F_SETFL, 0);						// turn off blocking for reads

	tcgetattr(_handle, &_options);					// get current settings
	cfsetspeed(&_options, baudrate);				// set baudrate
	_options.c_cflag &= ~CSTOPB;					// use only one stop bit
	_options.c_cflag |= CLOCAL;					// ignore status lines
	_options.c_cflag |= CREAD;					// enable receiver
	cfmakeraw(&_options);						// apply options
	tcsetattr(_handle, TCSANOW, &_options);				// update settings
	sleep(1);

	read(_handle, nullptr, 256*1000);
}

char* Serial::readLine()
{
	memset(&_buf, '\0', sizeof(_buf));
	int n = read(_handle, &_buf, sizeof(_buf));
	return _buf;
}

char Serial::readByte()
{
	memset(&_buf, '\0', sizeof(_buf));
	int n = read(_handle, &_buf, sizeof(char));
	return _buf[0];
}

Serial::~Serial()
{
	close(_handle);
}
