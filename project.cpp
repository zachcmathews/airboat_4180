#include <stdio.h>
#include <unistd.h>		// read(), write(), close()	
#include <cstring>
#include <cmath>
#include <iostream>		// standard I/O
#include <fstream>		// file operations
#include <signal.h>		// handle errors
#include <fcntl.h>		// File controls
#include <errno.h>		// Error integer and strerror()
#include <termios.h>		// POSIX terminal control definition
#include <pigpio.h>		// Raspberry PI GPIO library
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simpleio.h"
#include "motor.h"
#include "servo.h"
#include "serial.h"

#define STANDBY 19

#define PI 3.14159
#define RADIUS_EARTH 6378100

int lidar_handle;
pthread_t *gpsThread;
pthread_t *lidarThread;

unsigned gpsReady;
unsigned lidarReady;

bool logDepth = true;
float logged_x = 0;
float logged_y = 0;
float logged_z = 0;

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
		std::string sentence(gps.readLine());
		if (!sentence.empty())
		{
			if (sentence.find("$GPGGA") != std::string::npos)
			{
				std::cout << sentence << std::endl;
				
				// Parse NMEA sentence
				int utc_begin = sentence.find(",")+1;
				int utc_end = sentence.find(",",utc_begin);
				int lat_begin = utc_end+1;
				int lat_end = sentence.find(",", lat_begin);
				int lat_hemi_index = lat_end+1;
				int long_begin = lat_hemi_index+2;
				int long_end = sentence.find(",", long_begin);
				int long_hemi_index = long_end+1;

				std::string latitude = sentence.substr(lat_begin,lat_end-lat_begin);
				char latitude_hemisphere = sentence.at(lat_hemi_index);
				std::string longitude = sentence.substr(long_begin, long_end-long_begin);
				char longitude_hemisphere = sentence.at(long_hemi_index);

				std::cout << latitude + " " + latitude_hemisphere << std::endl;
				std::cout << longitude + " " + longitude_hemisphere << std::endl;
				
				if (!latitude.empty() && !longitude.empty())
				{
					// Convert degrees, decimal minutes to degrees
					float lat_deg = stof(latitude.substr(0,2)) + (stof(latitude.substr(3,std::string::npos)) / 60.0); 
					float long_deg = stof(longitude.substr(0,3)) + (stof(longitude.substr(4,std::string::npos)) / 60.0);

					// Convert degrees to radians
					float lat_rad = (PI/180.0) * lat_deg;
					float long_rad = (PI/180.0) * long_deg;

					// Convert to x,y coordinates
					float x = RADIUS_EARTH * long_rad * cos(lat_rad);
					float y = RADIUS_EARTH * lat_rad * cos(lat_rad);
				
					std::cout << "X: " << x << " Y: " << y << std::endl;
					
					float dist_from_logged = sqrt(x*x - logged_x*logged_x + y*y - logged_y*logged_y);
					if (dist_from_logged > 5.0)
					{
						logged_x = x;
						logged_y = y;
						logDepth = true;
					}				

					std::cout << "Distance from logged: " << dist_from_logged << std::endl;
				}
			}
		}
	}

}

void* lidarHandler(void *arg)
{
	std::cout << "Handling LiDAR" << std::endl;
	lidar_handle = serOpen("/dev/ttyS0", 115200, 0);
	if (lidar_handle >= 0)
	{
		std::ofstream log;
		log.open("/var/www/html/data.csv");
		log << "x,y,z" << std::endl;

		while (true) 
		{
			unsigned bytes = 0;
			std::string data;
			if (serDataAvailable(lidar_handle) >= 9)
			{
				// Read data
				char header_1;
				serRead(lidar_handle, &header_1, 1);
				char header_2;
				serRead(lidar_handle, &header_2, 1);
				char dist_L;
				serRead(lidar_handle, &dist_L, 1);
				char dist_H;
				serRead(lidar_handle, &dist_H, 1);
				char extra[5];
				serRead(lidar_handle, extra, 5);
				
				// Parse data
				float z = (256 * (int) dist_H + (int) dist_L) / 100.0;

				// Log depth when >5m from last log
				if (logDepth) {
					logged_z = z;
					log << logged_x << "," << logged_y << "," << logged_z << std::endl;
					logDepth = false;

					std::cout << "Logged data: " << logged_x << ", " << logged_y << ", " << logged_z << std::endl;
				}
			}
		}
		log.close();
		serClose(lidar_handle);
	}
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
	Motor motor(5, 6, 13);				// pwm, fwd, rev
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
	servo.pos(270);
	sleep(1);
	servo.pos(180);
	sleep(2);
	servo.pos(225);
	sleep(1);

	gpsThread = gpioStartThread(gpsHandler, nullptr); 
	lidarThread = gpioStartThread(lidarHandler, nullptr);
	
	// Bluetooth
	std::cout << "Handling Bluetooth" << std::endl;
	Serial bt("/dev/rfcomm0", 9600);
	while (true)
	{
		std::string line = bt.readLine();
		if (!line.empty())
		{
			try {
				// Get position sensor data
				float x = std::stof(line.substr(line.find("X:")+2,line.find("Y:")));
				float y = std::stof(line.substr(line.find("Y:")+2,line.find("Z:")));
				float z = std::stof(line.substr(line.find("Z:")+2,line.find("#")));

				//std::cout << "Y: " << y << " Z: " << z << std::endl;
	
				// Scale propeller speed and rudder position based on sensor data
				// This could still be tweaked with more experimentation
				float scaled_z = z / sqrt(z*z + x*x);
				float scaled_y = sqrt(2)*y / sqrt(y*y + z*z);
				
				//std::cout << "Scaled Y: " << scaled_y << " Z: " << scaled_z << std::endl;

				// Control rudder (servo)
				if (scaled_y > 1.0) 
				{
					servo.pos(180);
				}
				else if (scaled_y < -1.0) 
				{
					servo.pos(270);
				}
				else
				{
					servo.pos(225-(int)45*scaled_y);
				}

				// Control propeller (dc motor)
				if (scaled_z > 1.0) 
				{
					motor.speed(1.0);
				}
				else if (scaled_z < -1.0) 
				{
					motor.speed(-1.0);
				}
				else
				{
					motor.speed(scaled_z);
				}
						

			} catch (std::exception& ia) {
				std::cerr << "Invalid argument: " << ia.what() << std::endl;
				motor.speed(0);
				motor.stop();
				servo.pos(225);
			}
		}
		else
		{
			// Stop propeller and set rudder to neutral position when
			// Bluetooth drops connection
			servo.pos(225);
			motor.speed(0);
			motor.stop();
		}
	}

	serClose(lidar_handle);			// close lidar serial connection
	gpioWrite(STANDBY, 0);			// turn off dc motor
	gpioStopThread(gpsThread);		// close serial connection and stop
	gpioStopThread(lidarThread);		// close serial connection and stop`
	gpioTerminate();			// terminate use of GPIO library
	return 0;
}
