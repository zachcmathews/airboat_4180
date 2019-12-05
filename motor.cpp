#include "motor.h"

Motor::Motor(unsigned int pwm, unsigned int fwd, unsigned int rev)
	: pwm(pwm,1000), fwd(fwd), rev(rev)
{
}

void Motor::speed(float speed)
{
	if (speed > 0)
	{
		fwd = 1;
		rev = 0;
		pwm = speed;
	}
	else
	{
		fwd = 0;
		rev = 1;
		pwm = -speed;
	}
}

void Motor::stop()
{
	fwd = 1;
	rev = 1;
	pwm = 0;
}
