# Airboat 4180
The Airboat Model 4180 was designed and built for the final design project in Embedded Systems Design (ECE4180) at Georgia Tech by Zachary Mathews. It is Bluetooth controlled and makes use of a GPS and waterproof Li-DAR module to map the bottom of a body of non-turbid water. The following instructions will guide you to creating your own. This guide assumes familiarity with connecting your Raspberry Pi over ssh/VNC and with Git.

## Parts List
[Aluminum Paint Tray](https://www.walmart.com/ip/METAL-TRAY/17165827?wmlspartner=wlpa&selectedSellerId=0&wl13=3775&adid=22222222227016740642&wl0=&wl1=g&wl2=c&wl3=48112424552&wl4=pla-96451202432&wl5=9010937&wl6=&wl7=&wl8=&wl9=pla&wl10=8175035&wl11=local&wl12=17165827&veh=sem&gclid=CjwKCAiA8qLvBRAbEiwAE_ZzPfbbJKlvOlJf9ABkXKlE1sk-SRmC3CgY20eVEAUPCWGY1to3k5PSqBoCwb8QAvD_BwE)\
[(2) 1/4" Dowel Rods](https://www.amazon.com/Dowel-Rod-Wood-inches-pieces/dp/B004BQ0DGK)\
[Waterproof Glue](https://www.amazon.com/Gorilla-Clear-Contact-Adhesive-Waterproof/dp/B06WGSRM4Z/ref=sr_1_3?keywords=waterproof+glue&qid=1575601076&sr=8-3)\
[CanaKit Raspberry Pi Zero W Kit](https://www.amazon.com/gp/product/B072N3X39J/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1)\
[(2) Portable Chargers](https://www.amazon.com/Anker-PowerCore-Ultra-Compact-High-Speed-Technology/dp/B01CU1EC6Y/ref=sxin_2_osp14-4047d6fa_cov?ascsubtag=4047d6fa-238e-4577-9c12-f7bec948bd9f&creativeASIN=B01CU1EC6Y&cv_ct_id=amzn1.osp.4047d6fa-238e-4577-9c12-f7bec948bd9f&cv_ct_pg=search&cv_ct_wn=osp-search&keywords=portable+charger&linkCode=oas&pd_rd_i=B01CU1EC6Y&pd_rd_r=bd5d0c54-dbc5-4ed7-a680-144fbd5809bb&pd_rd_w=xnFut&pd_rd_wg=O0oFx&pf_rd_p=a23a388c-add5-49df-b293-a31ade89c6bf&pf_rd_r=ZV5XD1PPZ4TFX0RY6Y17&qid=1575874429&smid=A294P4X9EWVXLJ&tag=tgl0a3-20)\
[USB Type A to Mini B](https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/ref=pd_hpb_a2a_sims_1/143-2417332-2119622?_encoding=UTF8&pd_rd_i=B00NH13S44&pd_rd_r=64abae17-0a33-42c3-9077-bb67169ba604&pd_rd_w=h3995&pd_rd_wg=PWkKO&pf_rd_p=933ec658-4c08-4d06-8467-a9db8d161915&pf_rd_r=9M9SD3SY5NYNA3YJW5VQ&psc=1&refRID=9M9SD3SY5NYNA3YJW5VQ)
[USB Mini-B Breakout (5V/2A)](https://www.sparkfun.com/products/9966)\
[H-Bridge Dual TB6612FNG Motor Driver](https://www.sparkfun.com/products/14450)\
[Brushless DC Motor](https://www.amazon.com/gp/product/B00BG9K2VQ/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)\
[Waterproof Servo](https://www.amazon.com/gp/product/B076CNKQX4/ref=ppx_yo_dt_b_asin_title_o00_s01?ie=UTF8&psc=1)\
[Waterproof TFmini Plus Li-DAR](https://www.amazon.com/gp/product/B07PWW3FWJ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)\
[NEO-6M GPS Module](https://www.amazon.com/gp/product/B01D1D0F5M/ref=ppx_yo_dt_b_asin_title_o04_s01?ie=UTF8&psc=1)\
[USB to TTL Adapter](https://www.amazon.com/gp/product/B075N82CDL/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)\

#### Optional (for protoboard build)
[PCB & Header Pins](https://www.amazon.com/gp/product/B07CK3RCKS/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)\
[22AWG Wire](https://www.amazon.com/gp/product/B077HRKYB1/ref=ppx_yo_dt_b_asin_title_o03_s01?ie=UTF8&psc=1)\
And soldering supplies...

## Building the boat
Use the tables below to ensure proper electrical connection of each part of your boat.

#### Brushless DC Motor (Propeller)
| 5V/2A Power Supply | Raspberry Pi Zero W | H-Bridge Motor Driver | Brushless DC Motor |
|--------------------|---------------------|-----------------------|--------------------|
|                    | 3.3V                | VCC                   |                    |
|                    | GPIO 5              | PWMA                  |                    |
|                    | GPIO 6              | AIN1                  |                    |
|                    | GPIO 13             | AIN2                  |                    |
|                    | GPIO 19             | STBY                  |                    |
| 5V                 |                     | VM                    |                    |
|                    |                     | AOUT1                 | POSITIVE           |
|                    |                     | AOUT2                 | NEGATIVE           |
| GND                | GND                 | GND                   |                    |

#### Servo
| 5V/2A Power Supply | Raspberry Pi Zero W | Servo |
|--------------------|---------------------|-------|
|                    | 5V                  | 5V    |
|                    | GPIO 18             | PWM   |
| GND                | GND                 | GND   |

#### TFmini Plus Li-DAR
| 5V/2A Power Supply | Raspberry Pi Zero W | TFmini Plus Li-DAR |
|--------------------|---------------------|--------------------|
|                    | 5V                  | 5V                 |
|                    | TXD (GPIO 14)       | RX (WHITE/BLUE)    |
|                    | RXD (GPIO 15)       | TX (GREEN)         |
| GND                | GND                 | GND                |

#### NEO-6M GPS Module
| NEO-6M GPS Module | USB to TTL Adapter | Raspberry Pi Zero W |
|-------------------|--------------------|---------------------|
| VCC               | VCC                | USB                 |
| TX                | RXD                | USB                 |
| RX                | TXD                | USB                 |
| GND               | GND                | USB                 |


### Attaching the outfitting
PROVIDE PICS
PROVIDE PICS
PROVIDE PICS

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

Open another console to run the project code.

### Using the remote:
1.) Select `raspi0` from the paired devices list.\
2.) Watch the ad (bleh). Then select `Tilt controller`.


## Getting a fix with the GPS
In the project directory, run `./gps_test.py` and wait for the the $GPGLL sentence to resemble the following instead of a string of commas: https://www.gpsinformation.org/dale/nmea.htm#GLL. The fix may take up to 10 minutes and may fail in areas with poor GPS signal reception or without clear view of the sky.


## Launching the boat
1.) Be sure you've made a connection with Bluetooth using the instructions above.\
2.) Be sure you have a GPS fix using the instructions above.\
3.) Run `make all` in project directory.\
4.) Run `sudo ./project` in project directory.\
5.) Wait for the boat to test out the motors and then put it on the water. Stay close by to ensure Bluetooth doesn't drop.\
6.) Kill the Bluetooth controller app to drop the connection and kill the motors.\
7.) Ctrl+C to kill the program in the console.


## Serve the underwater topographical map:
1.) Install Apache. Instructions found here: https://www.raspberrypi.org/documentation/remote-access/web-server/apache.md\
2.) In project directory: `sudo cp -rf html/* /var/www/html/`\
3.) Collect data with airboat on body of water: `sudo ./project`\
4.) Visit website at http://localhost or the ip address you use to connect over VNC/ssh
