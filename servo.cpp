#include "servo.h"

Servo::Servo(unsigned pin, unsigned freq, unsigned minWidth, unsigned maxWidth, unsigned degRotation)
	: _pin(pin), _minWidth(minWidth), _maxWidth(maxWidth), _degRotation(degRotation)
{
	gpioSetMode(_pin, PI_OUTPUT);
}

void Servo::pos(unsigned pos)
{
	unsigned pulseWidth = (unsigned) ( ((float) pos / (float) _degRotation) * (_maxWidth - _minWidth) + _minWidth );		// pulse width in microseconds
	gpioServo(_pin, pulseWidth);
}
