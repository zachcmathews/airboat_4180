#ifndef MOTOR
#define MOTOR

#include "simpleio.h"

class Motor 
{
	public:
	Motor(unsigned int pwm, unsigned int fwd, unsigned int rev);
	void speed(float speed);
	void stop();

	private:
	PWMOut pwm;
	DigitalOut fwd;
	DigitalOut rev; 
};

#endif
