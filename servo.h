#ifndef SERVO
#define SERVO

#include <pigpio.h>

class Servo
{
	public:
	Servo(unsigned pin, unsigned freq, unsigned minWidth, unsigned maxWidth, unsigned degRotation);
	void pos(unsigned pos);

	private:
	unsigned _pin;
	unsigned _minWidth;
	unsigned _maxWidth;
	unsigned _degRotation;
};

#endif
