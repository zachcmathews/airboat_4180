#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include "simpleio.h"
#include "motor.h"

#define STANDBY 19

class Servo
{
	public:
	Servo(unsigned pin, unsigned freq, unsigned minWidth, unsigned maxWidth, unsigned degRotation)
		: _pin(pin), _minWidth(minWidth), _maxWidth(maxWidth), _degRotation(degRotation)
	{
		gpioSetMode(_pin, PI_OUTPUT);
	}

	void pos(unsigned pos)
	{
		unsigned pulseWidth = (unsigned) ( ((float) pos / (float) _degRotation) * (_maxWidth - _minWidth) + _minWidth );		// pulse width in microseconds
		gpioServo(_pin, pulseWidth);
	}

	private:
	unsigned _pin;
	unsigned _minWidth;
	unsigned _maxWidth;
	unsigned _degRotation;
};

int main()
{
	// Initialize GPIO library
	if (gpioInitialise() < 0) 
	{
		std::cout << "PIGPIO initialization failed." << std::endl;
	}
	else
	{
		std::cout << "PIGPIO initialized." << std::endl;
	}

	// Test brushless DC motor
	gpioSetMode(STANDBY, PI_OUTPUT);
	gpioWrite(STANDBY, 1);
	Motor motor(5, 6, 13);
	motor.speed(1.0);
	sleep(1);
	motor.speed(0.5);
	sleep(1);
	motor.stop();
	sleep(1);
	motor.speed(-1.0);
	sleep(1);
	motor.stop();
	sleep(1);
	gpioWrite(STANDBY, 0);

	// Test servo motor
	Servo servo(18, 333, 500, 2500, 270);		// pin, freq, minWidth, maxWidth, degRotation
	servo.pos(0);
	sleep(1);
	servo.pos(270);
	sleep(2);
	servo.pos(135);
	sleep(1);

	// Test LiDAR sensor
	unsigned handle = serOpen("/dev/ttyS0", 115200, 0);
	while (handle >= 0) 
	{
		std::cout << serReadByte(handle) << std::endl;
	}
	serClose(handle);

	gpioTerminate();				// Terminate use of GPIO library
	return 0;
}
