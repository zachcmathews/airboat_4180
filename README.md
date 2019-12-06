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

## Setting up Bluetooth
In order to use your phone as a Bluetooth remote, download the Android app found here: https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller&hl=en_US

Then follow the steps below to pair your Raspberry Pi Zero W.

### Setup the Serial Port (SP) profile:
`sudo nano /etc/systemd/system/dbus-org.bluez.service`\
Add -C to the `ExecStart` statement to start the Bluetooth daemon in compatibility mode:\
`ExecStart=/usr/lib/bluetooth/bluetoothd -C`\
Add the following statement below the `ExecStart` line to enable the SP Profile:\
`ExecStartPost=/usr/bin/sdptool add SP`\
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
