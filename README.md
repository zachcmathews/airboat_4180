# airboat_4180
Lake floor LiDAR mapping airboat controlled via Bluetooth

## Building the boat
Use the tables below to ensure proper connection of each part of your boat.

### Brushless DC Motor (Propeller)
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

### Servo
| 5V/2A Power Supply | Raspberry Pi Zero W | Servo |
|--------------------|---------------------|-------|
|                    | 5V                  | 5V    |
|                    | GPIO 18             | PWM   |
| GND                | GND                 | GND   |

### TFmini Plus Li-DAR
| 5V/2A Power Supply | Raspberry Pi Zero W | TFmini Plus Li-DAR |
|--------------------|---------------------|--------------------|
|                    | 5V                  | 5V                 |
|                    | TXD (GPIO 14)       | RX (WHITE/BLUE)    |
|                    | RXD (GPIO 15)       | TX (GREEN)         |
| GND                | GND                 | GND                |

## Bluetooth Setup
In order to use your phone as a Bluetooth remote, download the Android app found here: https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller&hl=en_US

Then follow the steps below to pair your Raspberry Pi Zero W.

### Make your Raspberry Pi Discoverable:
`sudo bluetoothctl`
`discoverable on`
`alias raspi0`
Now, pair with the device `raspi0` in your phone's settings. If asked to verify pairing with a pin, do so.

### Using the Remote
1.) Click the settings (gear) icon
2.) Click "Connect to car"
3.) Select `raspi0` from the list
4.) Use the direction buttons to control the boat
5.) Or click the settings (gear) icon and click "Accelerometer control" to control the boat with your phone's gyroscopic sensors.
