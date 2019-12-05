# airboat_4180
Lake floor LiDAR mapping airboat controlled via Bluetooth

## Brushless DC Motor (Propeller)
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

## Servo
| 5V/2A Power Supply | Raspberry Pi Zero W | Servo |
|--------------------|---------------------|-------|
| 5V                 |                     | 5V    |
|                    | GPIO 18             | PWM   |
| GND                | GND                 | GND   |

## TFmini Plus Li-DAR
| 5V/2A Power Supply | Raspberry Pi Zero W | TFmini Plus Li-DAR |
|--------------------|---------------------|--------------------|
| 5V                 |                     | 5V                 |
|                    | TXD (GPIO14)        | RX (WHITE/BLUE)    |
|                    | RXD (GPIO15)        | TX (GREEN)         |
| GND                | GND                 | GND                |
