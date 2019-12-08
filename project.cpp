#include <stdio.h>
#include <unistd.h>		// read(), write(), close()	
#include <cstring>
#include <iostream>		// standard I/O
#include <signal.h>		// handle errors
#include <fcntl.h>		// File controls
#include <errno.h>		// Error integer and strerror()
#include <termios.h>		// POSIX terminal control definition
#include <pigpio.h>		// Raspberry PI GPIO library

#include "simpleio.h"
#include "motor.h"
#include "servo.h"
#include "serial.h"

#define STANDBY 19

int lidar_handle;
pthread_t *gpsThread;
pthread_t *lidarThread;

void sigHandler(int sig_num)
{
	serClose(lidar_handle);		// close lidar serial connection
	gpioWrite(STANDBY, 0);		// turn off dc motor
	gpioStopThread(gpsThread);	// close serial connection and stop
	gpioStopThread(lidarThread);	// close serial connection and stop`
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

void* gpsHandler(void *arg)
{
	std::cout << "Handling GPS" << std::endl;
	Serial gps("/dev/ttyUSB0", 9600);
	while (true)
	{
		std::string line = gps.readLine();
		if (!line.empty())
		{
			std::cout << line << std::endl;
		}
	}
}

void* lidarHandler(void *arg)
{
	std::cout << "Handling LiDAR" << std::endl;
	lidar_handle = serOpen("/dev/ttyS0", 115200, 0);
	while (lidar_handle >= 0) 
	{
		if (serDataAvailable(lidar_handle))
		{
			std::cout << serReadByte(lidar_handle) << std::endl;
		}
	}
	serClose(lidar_handle);
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

	// Test servo motor
	Servo servo(18, 333, 500, 2500, 270);		// pin, freq, minWidth, maxWidth, degRotation
	servo.pos(0);
	sleep(1);
	servo.pos(270);
	sleep(2);
	servo.pos(135);
	sleep(1);

	gpsThread = gpioStartThread(gpsHandler, nullptr); sleep(3);
	lidarThread = gpioStartThread(lidarHandler, nullptr); sleep(3);
	
	// Test Bluetooth
	std::cout << "Testing Bluetooth" << std::endl;
	Serial bt("/dev/rfcomm0", 9600);
	while (true)
	{
		std::string line = bt.readLine();
		if (!line.empty())
		{
			//float x = std::stof(line.substr(line.find(":")+1,line.find("Y:")));
			//float y = std::stof(line.substr(line.find(":")+1,line.find("Y:")));
			try {
				float x = std::stof(line.substr(line.find("X:")+2,line.find("Y:")));
				if (x > 10) {
				}
				else
				{
				}
				std::cout << x << std::endl;
			} catch (std::invalid_argument& ia) {
				std::cerr << "Invalid argument: " << ia.what() << std::endl;
			}
		
			try {
				float y = std::stof(line.substr(line.find("Y:")+2,line.find("Z:")));
				if (y > 10) {
					servo.pos(180);
				}
				else if (y < -10) {
					servo.pos(90);
				}
				else
				{
					servo.pos(135-(int)45*y/10.0);
				}
				std::cout << y << std::endl;
			} catch (std::invalid_argument& ia) {
				std::cerr << "Invalid argument: " << ia.what() << std::endl;
			}
			try {
				float z = std::stof(line.substr(line.find("Z:")+2,line.find("#")));
				if (z > 10) {
					motor.speed(1.0);
				}
				else
				{
					motor.speed(z/10.0);
				}
				std::cout << z << std::endl;
			} catch (std::invalid_argument& ia) {
				std::cerr << "Invalid argument: " << ia.what() << std::endl;
			}
		}
	}
	
	gpioTerminate();				// Terminate use of GPIO library
	return 0;
}
