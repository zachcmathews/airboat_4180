#include "simpleio.h"
DigitalOut::DigitalOut(unsigned int pin)
	: pin(pin)
{
	gpioSetMode(pin, PI_OUTPUT);
}

void DigitalOut::operator=(bool l)
{
	level = l;
	gpioWrite(pin, level);
}

void DigitalOut::operator!()
{
	gpioWrite(pin, !level);
	level = !level;
}

PWMOut::PWMOut(unsigned int pin)
	: pin(pin), freq(100), range(255)
{
	// Setup PWM
	gpioSetMode(pin, PI_OUTPUT);
	gpioSetPWMfrequency(pin, freq);
	gpioSetPWMrange(pin, range); 
}

PWMOut::PWMOut(unsigned int pin, unsigned int freq)
	: pin(pin), freq(freq), range(255)
{
	// Setup PWM
	gpioSetMode(pin, PI_OUTPUT);
	if (freq <= 100000)
	{
		gpioSetPWMfrequency(pin, freq);
	}
	gpioSetPWMrange(pin, range); 
}

PWMOut::PWMOut(unsigned int pin, unsigned int freq, unsigned int range)
	: pin(pin), freq(freq), range(range)
{
	// Setup PWM
	gpioSetMode(pin, PI_OUTPUT);

	if (freq <= 100000)
	{
		gpioSetPWMfrequency(pin, freq);
	}

	if (range <= 1000000)
	{	
		gpioSetPWMrange(pin, range); 
	}
}

void PWMOut::operator=(float d)
{
	if (d >= 0 && d <= 1.0)
	{
		duty = (unsigned int) float(d * range);
		gpioPWM(pin, duty);
	}
}
