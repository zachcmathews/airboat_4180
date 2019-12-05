#define SIMPLEIO

#ifndef PIGPIO
#include <pigpio.h>
#define PIGPIO
#endif

class DigitalOut
{
	public:
	DigitalOut(unsigned int pin);
	void operator=(bool l);
	void operator!();

	private:
	unsigned int pin;
	bool level;
};

class PWMOut
{
	public:
	PWMOut(unsigned int pin);
	PWMOut(unsigned int pin, unsigned int freq);
	PWMOut(unsigned int pin, unsigned int freq, unsigned int range);
	void operator=(float d);

	private:
	unsigned int pin;
	unsigned int freq;
	unsigned int range;
	unsigned int duty;
};
