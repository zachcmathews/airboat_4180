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
#define DIST_THRESHOLD 10.0

int lidar_handle;
pthread_t *gpsThread;
pthread_t *lidarThread;
char* logFile;

bool logDepth = false;
float logged_x = 0;
float logged_y = 0;
float logged_z = 0;

void sigHandler(int sig_num)
{
	serClose(lidar_handle);		// close lidar serial connection
	gpioWrite(STANDBY, 0);		// turn off dc motor
	gpioStopThread(gpsThread);	// close serial connection and stop
	gpioStopThread(lidarThread);	// close serial connection and stop`
	gpioTerminate();		// terminate use of GPIO library

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
	
	// Open log file
	std::ifstream sample_data;
	sample_data.open("./sample_gps_data_linux.txt");

	if (sample_data.is_open())
	{
		std::string sentence;

		while (!sample_data.eof() && std::getline(sample_data, sentence, '\n'))
		{
			if (!sentence.empty())
			{
				if (sentence.find("GGA") != std::string::npos)
				{
					// Just some logging
					std::cout << sentence << std::endl;
					
					// Parse NMEA sentence
					int utc_begin = sentence.find(",");
					int utc_end = sentence.find(",",utc_begin+1);
					int lat_begin = utc_end+1;
					int lat_end = sentence.find(",",utc_end+1);
					int lat_hemi_begin = lat_end+1;
					int lat_hemi_end = sentence.find(",",lat_end+1);
					int long_begin = lat_hemi_end+1;
					int long_end = sentence.find(",",lat_hemi_end+1);
					int long_hemi_begin = long_end+1;
					int long_hemi_end = sentence.find(",",long_end+1);
					int gps_fix = sentence.at(sentence.find(",", long_hemi_end+1)-1);

					if (gps_fix != '0')
					{
						// Extract latitude and longitude
						std::string latitude = sentence.substr(lat_begin,lat_end-lat_begin);
						std::string latitude_hemisphere = sentence.substr(lat_hemi_begin,lat_hemi_end-lat_hemi_begin);
						std::string longitude = sentence.substr(long_begin,long_end-long_begin);
						std::string longitude_hemisphere = sentence.substr(long_hemi_begin,long_hemi_end-long_hemi_begin);

						std::cout << latitude + " " + latitude_hemisphere << std::endl;
						std::cout << longitude + " " + longitude_hemisphere << std::endl;

						// Convert degrees, decimal minutes to degrees
						if (!latitude.empty() && !longitude.empty())
						{
							double lat_deg = stof(latitude.substr(0,2)) + (stof(latitude.substr(3,std::string::npos)) / 60.0); 
							double long_deg = stof(longitude.substr(0,3)) + (stof(longitude.substr(4,std::string::npos)) / 60.0);

							std::cout << lat_deg << std::endl;
							std::cout << long_deg << std::endl;

							// Convert degrees to radians
							double lat_rad = (PI/180.0) * lat_deg;
							double long_rad = (PI/180.0) * long_deg;

							// Convert to x,y coordinates
							double x = RADIUS_EARTH * long_rad * cos(lat_rad);
							double y = RADIUS_EARTH * lat_rad * cos(lat_rad);
						
							std::cout << "X: " << x << " Y: " << y << std::endl;
							
							// Calculate distance from last logged value
							// and log if greater than DIST_THRESHOLD
							double dist_from_logged = sqrt((x - logged_x)*(x - logged_x) + (y - logged_y)*(y-logged_y));
							if (dist_from_logged > DIST_THRESHOLD)
							{
								logged_x = x;
								logged_y = y;
								logDepth = true;
							}				
							std::cout << "Distance from logged: " << dist_from_logged << std::endl;
						}
					}
					else
					{
						std::cout << "No GPS fix" << std::endl;
					}
				}
			}

			usleep(16000);		// emulate GPS PPS
		}
	}
	std::cout << "Finished with sample data" << std::endl;
	sample_data.close();
}

void* lidarHandler(void *arg)
{
	std::cout << "Handling LiDAR" << std::endl;
	std::cout << logFile << std::endl;

	// Open log file
	std::ofstream log;
	log.open(logFile);		// init log file
	log << "x,y,z" << std::endl;
	log.close();

	char* uart = "/dev/serial0";
	lidar_handle = serOpen(uart, 115200, 0);
	while (true)
	{
		if (lidar_handle >= 0)
		{
			if (serDataAvailable(lidar_handle)>=9)
			{
				// Read data
				char header[2], dist_H, dist_L;
				serRead(lidar_handle, header, 2);
				if (header[0] == 'Y' && header[1] == 'Y')
				{
					serRead(lidar_handle, &dist_L, 1);
					serRead(lidar_handle, &dist_H, 1);
					serRead(lidar_handle, nullptr, 4);			// get rid of excess
				}

				// Parse data
				float z = -1 * (256 * (int) dist_H + (int) dist_L) / 100.0;
				//std::cout << z << std::endl;

				// Log depth in file when flag set
				if (logDepth) {
					log.open(logFile, std::ofstream::app);
					logged_z = z;
					log << logged_x << "," << logged_y << "," << logged_z << std::endl;
					logDepth = false;
					log.close();

					std::cout << "Logged data: " << logged_x << ", " << logged_y << ", " << logged_z << std::endl;
				}
			}
		}
	}
	serClose(lidar_handle);
}

int main(int argc, char *argv[])
{
	if (false) 
	{
		logFile = argv[argc-1];
	}
	else
	{
		logFile = "/var/www/html/data.csv";
	}

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
	
	// Handle bluetooth
	std::cout << "Handling Bluetooth" << std::endl;
	char* rfcomm = "/dev/rfcomm0";
	Serial bt(rfcomm, 9600);
	
	while (true)
	{	
		if (bt.isOpen())
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
					if (scaled_y > 1.0 && scaled_z > 0.3) 
					{
						servo.pos(180);
					}
					else if (scaled_y < -1.0 && scaled_z > 0.3) 
					{
						servo.pos(270);
					}
					else if (scaled_z > 0.3)
					{
						servo.pos(225-(int)45*scaled_y);
					}
					else
					{
						servo.pos(225-(int)45*y/10.0);
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
				motor.speed(0);
				motor.stop();
				servo.pos(225);
			}

			usleep(32000);		// update ~30Hz
		}
		else
		{
			// Stop propeller and set rudder to neutral position when
			// Bluetooth drops connection
			motor.speed(0);
			motor.stop();
			servo.pos(225);
	
			std::cout << "Bluetooth not connected. Attempting connection..." << std::endl;
			sleep(1);		// sleep longer when not connected
		}

	}

	serClose(lidar_handle);			// close lidar serial connection
	gpioWrite(STANDBY, 0);			// turn off dc motor
	gpioStopThread(gpsThread);		// close serial connection and stop
	gpioStopThread(lidarThread);		// close serial connection and stop`
	gpioTerminate();			// terminate use of GPIO library
	
	return 0;
}
