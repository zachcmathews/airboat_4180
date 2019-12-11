# Airboat 4180
The Airboat Model 4180 was designed and built for the final design project in Embedded Systems Design (ECE4180) at Georgia Tech by Zachary Mathews. It makes use of a GPS and waterproof Li-DAR module to map the floor of a body of non-turbid water. Here's  how it's made.

![Topo Map](https://github.com/zachcmathews/airboat_4180/blob/master/documentation/map.png)

## Software
Multithreaded to handle GPS, LiDAR, and Bluetooth control separately. The Airboat Model 4180 uses PIGPIO as the underlying GPIO framework to enable software PWM for DC motor control. In the Git repository, you see a few different class and header files along with the demo and project executables. In order to facilitate use of the PIGPIO library for the purposes of this project, I've written the classes to provide operator overloading and helper functions (position, speed, connect, read, readline). These helped greatly with the organization of the main `project.cpp` file.

## Parts List
[Aluminum Paint Tray](https://www.walmart.com/ip/METAL-TRAY/17165827?wmlspartner=wlpa&selectedSellerId=0&wl13=3775&adid=22222222227016740642&wl0=&wl1=g&wl2=c&wl3=48112424552&wl4=pla-96451202432&wl5=9010937&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=17165827&veh=sem&gclid=CjwKCAiA8qLvBRAbEiwAE_ZzPfbbJKlvOlJf9ABkXKlE1sk-SRmC3CgY20eVEAUPCWGY1to3k5PSqBoCwb8QAvD_BwE)\
[Backer Rod](https://www.amazon.com/M-D-Building-Products-71480-20-Feet/dp/B000BQWOC0/ref=sr_1_2?keywords=1%2F2%22+backer+rod&qid=1576033121&sr=8-2)\
[Waterproof Glue](https://www.amazon.com/Gorilla-Clear-Contact-Adhesive-Waterproof/dp/B06WGSRM4Z/ref=sr_1_3?keywords=waterproof+glue&qid=1575601076&sr=8-3)\
[Duct Tape](https://www.amazon.com/Duck-Brand-241414-Single-Transparent/dp/B01F3H8M0E/ref=sr_1_13?keywords=clear+duct+tape&qid=1576035820&sr=8-13)\
[CanaKit Raspberry Pi Zero W Kit](https://www.amazon.com/gp/product/B072N3X39J/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1)\
[Mini-B USB Breakout](https://www.sparkfun.com/products/9966)\
[H-Bridge Dual TB6612FNG Motor Driver](https://www.sparkfun.com/products/14450)\
[DC Motor](https://www.amazon.com/gp/product/B00BG9K2VQ/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)\
[Waterproof Servo](https://www.amazon.com/gp/product/B076CNKQX4/ref=ppx_yo_dt_b_asin_title_o00_s01?ie=UTF8&psc=1)\
[Waterproof TFmini Plus Li-DAR](https://www.amazon.com/gp/product/B07PWW3FWJ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)\
[NEO-6M GPS Module](https://www.amazon.com/gp/product/B01D1D0F5M/ref=ppx_yo_dt_b_asin_title_o04_s01?ie=UTF8&psc=1)\
[USB to TTL Adapter](https://www.amazon.com/gp/product/B075N82CDL/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)\
[Portable Charger](https://www.amazon.com/Portable-RAVPower-22000mAh-External-Compatible/dp/B07YCC7MD2/ref=sr_1_1?keywords=2a+portable+charger+usb&qid=1576037773&sr=8-1)


## Building the boat

### The frame
![Airboat Frame](https://github.com/zachcmathews/airboat_4180/blob/master/documentation/frame.jpg)
#### Attaching the backer rod
Apply glue below outside rim of paint tray. Stick backer rod to it. Tape each corner to secure while glue dries.

#### Mounting the motors
The propeller motor should be mounted dead center in the rear of the boat above the rim of the paint tray.\
The rudder motor should be mounted directly below it with the screw dead center.

#### Mounting the LiDAR
The LiDAR may be mounted any where the lens is completely submerged and pointing nadir.

### The electronics
![The Guts](https://github.com/zachcmathews/airboat_4180/blob/master/documentation/electronics.jpg)

#### DC Motor (Propeller)
| Mini-B USB Breakout | Raspberry Pi Zero W | H-Bridge Motor Driver | Brushless DC Motor |
|---------------------|---------------------|-----------------------|--------------------|
|                     | 3.3V                | VCC                   |                    |
|                     | GPIO 5              | PWMA                  |                    |
|                     | GPIO 6              | AIN1                  |                    |
|                     | GPIO 13             | AIN2                  |                    |
|                     | GPIO 19             | STBY                  |                    |
| VCC                 |                     | VM                    |                    |
|                     |                     | AOUT1                 | POSITIVE           |
|                     |                     | AOUT2                 | NEGATIVE           |
| GND                 | GND                 | GND                   |                    |

#### Servo (Rudder)
| Mini-B USB Breakout | Raspberry Pi Zero W | Servo |
|---------------------|---------------------|-------|
| VCC                 |                     | 5V    |
|                     | GPIO 18             | PWM   |
| GND                 | GND                 | GND   |

#### TFmini Plus Li-DAR
| Mini-B USB Breakout | Raspberry Pi Zero W | TFmini Plus Li-DAR |
|---------------------|---------------------|--------------------|
|                     | 5V                  | 5V                 |
|                     | TXD (GPIO 14)       | RX (WHITE/BLUE)    |
|                     | RXD (GPIO 15)       | TX (GREEN)         |
| GND                 | GND                 | GND                |

#### NEO-6M GPS Module
| Raspberry Pi Zero W | USB to TTL Adapter | NEO-6M GPS Module |
|---------------------|--------------------|-------------------|
| USB                 | VCC                | VCC               |
| USB                 | TX                 | RX                |
| USB                 | RX                 | TX                |
| USB                 | GND                | GND               |


## Setting up the webserver
1.) Install Apache -> (https://www.raspberrypi.org/documentation/remote-access/web-server/apache.md)\
2.) In `airboat_4180` directory run `sudo cp -rfv ./html/ /var/www/html/` to copy the webpage and sample data over.\
3.) Navigate to the Raspberry Pi's IP address in a web browser. You should see a 3D plot.


## Setting up Bluetooth
In order to use your phone as a Bluetooth remote, download the Android app found here: https://play.google.com/store/apps/details?id=com.hamzaburakhan.btremotecontrolarduino&hl=en_US

Then follow the steps below to pair your Raspberry Pi Zero W.

### Setup the Serial Port (SP) profile:
`sudo nano /etc/systemd/system/dbus-org.bluez.service`

Add -C to the `ExecStart` statement to start the Bluetooth daemon in compatibility mode:\
`ExecStart=/usr/lib/bluetooth/bluetoothd -C`

Add the following statement below the `ExecStart` line to enable the SP Profile:\
`ExecStartPost=/usr/bin/sdptool add SP`

Reboot to make changes take effect:\
`sudo reboot`

### Make your Raspberry Pi discoverable:
`sudo bluetoothctl`\
`alias raspi0`\
`discoverable on`

Now, pair with the device `raspi0` in your phone's settings. If asked to verify pairing with a pin, do so.

Emulate a serial port over Bluetooth using the RFCOMM protocol:\
`sudo rfcomm watch hci0`

### Using the remote:
1.) Select `raspi0` from the paired devices list.\
2.) Watch the ad (bleh). Then select `Tilt Controller`.


## Launching the boat
1.) Make sure you're emulating a serial port over Bluetooth using `sudo rfcomm watch hci0`.\
2.) Connect Bluetooth remote app.\
3.) In the `airboat_4180` directory, build the project using the command `make`.\
4.) In the `airboat_4180` directory, run `sudo ./project` and wait for the initialization sequence to finish.\
5.) In order to use the mapping function, wait for the message `No GPS fix` to go away.\
6.) Navigate the area you wish to map.\
7.) Close the Bluetooth Controller app to stop the motors.\
8.) Shutdown the Airboat 4180 using Ctrl+C.\
9.) You may now navigate to your Raspberry Pi's IP address on a web browser and see a map of the lake floor.

## Running the demo
The demo simulates movement (GPS data). It is used to test the mapping function where GPS reception is weak to nonexistent.

1.) To run the demo, make sure you're emulating a serial port over Bluetooth using `sudo rfcomm watch hci0`.\
2.) Connect Bluetooth remote app.\
3.) In the `airboat_4180` directory, build the demo using the command `make demo`.\
4.) In the `airboat_4180` directory, run `sudo ./demo` and wait for the initialization sequence to finish.\
5.) Move the boat up and down to emulate a lake bed with varying depth.\
6.) When the `Finished with sample data` message appears, you may shutdown the Airboat 4180 with Ctrl+C.\
7.) You may now navigate to your Raspberry Pi's IP address on a web browser and see a map of the emulated lake floor.
