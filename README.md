# Airboat 4180
The Airboat Model 4180 was designed and built for the final embedded design project in Embedded Systems Design (ECE4180) at Georgia Tech by Zachary Mathews. It makes use of a GPS and waterproof Li-DAR module to map the bottom of a body of water. It works best in non-turbid water. The following instructions will guide you to creating your own.


## Parts List
[CanaKit Raspberry Pi Zero W Kit](https://www.amazon.com/gp/product/B072N3X39J/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1)\
[Brushless DC Motor](https://www.amazon.com/gp/product/B00BG9K2VQ/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)\
[Waterproof Servo](https://www.amazon.com/gp/product/B076CNKQX4/ref=ppx_yo_dt_b_asin_title_o00_s01?ie=UTF8&psc=1)\
[Waterproof TFmini Plus Li-DAR](https://www.amazon.com/gp/product/B07PWW3FWJ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)\
[NEO-6M GPS Module](https://www.amazon.com/gp/product/B01D1D0F5M/ref=ppx_yo_dt_b_asin_title_o04_s01?ie=UTF8&psc=1)\
[USB to TTL Adapter](https://www.amazon.com/gp/product/B075N82CDL/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)\
[8" Propeller](https://www.amazon.com/gp/product/B007AFYNZG/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)


## Building the boat
Use the tables below to ensure proper connection of each part of your boat.

##### Brushless DC Motor (Propeller)
| 5V/2A Power Supply | Raspberry Pi Zero W | H-Bridge Motor Driver | Brushless DC Motor |
|--------------------|---------------------|-----------------------|--------------------|
|                    | 3.3V                | Vcc                   |                    |
|                    | GPIO 5              | PWMA                  |                    |
|                    | GPIO 6              | AIN1                  |                    |
|                    | GPIO 13             | AIN2                  |                    |
|                    | GPIO 19             | STBY                  |                    |
| 5V                 |                     | VM                    |                    |
|                    |                     | AOUT1                 | POSITIVE           |
|                    |                     | AOUT2                 | NEGATIVE           |
| GND                | GND                 | GND                   |                    |

##### Servo
| 5V/2A Power Supply | Raspberry Pi Zero W | Servo |
|--------------------|---------------------|-------|
|                    | 5V                  | 5V    |
|                    | GPIO 18             | PWM   |
| GND                | GND                 | GND   |

##### TFmini Plus Li-DAR
| 5V/2A Power Supply | Raspberry Pi Zero W | TFmini Plus Li-DAR |
|--------------------|---------------------|--------------------|
|                    | 5V                  | 5V                 |
|                    | TXD (GPIO 14)       | RX (WHITE/BLUE)    |
|                    | RXD (GPIO 15)       | TX (GREEN)         |
| GND                | GND                 | GND                |


## Setting up Bluetooth
In order to use your phone as a Bluetooth remote, download the Android app found here: https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller&hl=en_US

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

Open another console to run the project code. I suggest [ConEmu](https://conemu.github.io/).

### Using the remote:
1.) Click the settings (gear) icon\
2.) Click "Connect to car"\
3.) Select `raspi0` from the list\
4.) Use the direction buttons to control the boat\
5.) Or click the settings (gear) icon and click "Accelerometer control" to control the boat with your phone's gyroscopic sensors.
