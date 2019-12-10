#include "serial.h"

Serial::Serial(char* port, int baudrate)
	: _port(port), _baudrate(baudrate)
{
	connect();
}

void Serial::connect()
{
	_handle = open(_port, O_RDWR | O_NOCTTY | O_NDELAY);			// open with R/W, no tty control, nonblocking I/O
	if (_handle == -1)
	{
		_isOpen = false;
	}
	else
	{
		memset(&_buf, '\0', sizeof(_buf));

		fcntl(_handle, F_SETFL, 0);					// turn off blocking for reads
		tcgetattr(_handle, &_options);					// get current settings
		cfsetispeed(&_options, _baudrate);				// set baudrate
		_options.c_cflag &= ~CSTOPB;					// use only one stop bit
		_options.c_cflag |= CLOCAL;					// ignore status lines
		_options.c_cflag |= CREAD;					// enable receiver
		cfmakeraw(&_options);						// apply options
		tcsetattr(_handle, TCSANOW, &_options);				// update settings

		sleep(1);
		tcflush(_handle,TCIOFLUSH);					// flush buffer

		_isOpen = true;
	}
}

bool Serial::isOpen()
{
	if (!_isOpen)
	{
		connect();
	}

	return _isOpen;
}

char* Serial::readLine()
{
	memset(&_buf, '\0', sizeof(_buf));
	unsigned i = 0;
	int n = 0;
	do {
		n = read(_handle, &_buf[i], sizeof(char));
	} while (n > 0 && _buf[i] != '\r'  && _buf[i] != '\n' && _buf[i] != '#' && ++i < sizeof(_buf));
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
