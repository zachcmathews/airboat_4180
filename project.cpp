#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <pigpio.h>
#include "simpleio.h"
#include "motor.h"
#include "servo.h"

#define STANDBY 19

int bt_handle;
int lidar_handle;
int gps_handle;

void sigHandler(int sig_num)
{
	serClose(bt_handle);		// close bluetooth serial connection
	serClose(lidar_handle);		// close lidar serial connection
	serClose(gps_handle);		// close gps serial connection
	gpioTerminate();		// Terminate use of GPIO library

	signal(SIGINT, SIG_DFL);	// exit program
	kill(getppid(), SIGINT);	// kill program
	kill(getpid(), SIGINT);	
	exit(0);
}

static void exit_handler(void)
{
	gpioTerminate();
}

int main(int argc, char *argv[])
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

	// Handle errors
	signal(SIGQUIT, sigHandler);
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
	signal(SIGABRT, sigHandler);
	atexit(exit_handler);
	
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
	
	// Test Bluetooth
	bt_handle = serOpen("/dev/serial1", 9600, 0);
	while (bt_handle >= 0)
	{
		if (serDataAvailable(bt_handle))
		{
			std::cout << serReadByte(bt_handle) << std::endl;
		}
	}
	serClose(bt_handle);

	// Test LiDAR sensor
	lidar_handle = serOpen("/dev/ttyS0", 115200, 0);
	while (lidar_handle >= 0) 
	{
		std::cout << serReadByte(lidar_handle) << std::endl;
	}
	serClose(lidar_handle);

	gpioTerminate();				// Terminate use of GPIO library
	return 0;
}
