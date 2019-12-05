#!/usr/bin/env python
import serial
import time

ser = serial.Serial("/dev/serial0", baudrate=115200, timeout=1)
ser.write(chr(0x47))
ser.write(chr(0x57))
ser.write(chr(0x02))
ser.write(chr(0x00))
ser.write(chr(0x00))
ser.write(chr(0x00))
ser.write(chr(0x01))
ser.write(chr(0x06))

while (True):
    while (ser.in_waiting >= 9):
        if (('Y' == ser.read()) and ('Y' == ser.read())):
            Dist_L = ser.read()
            Dist_H = ser.read()
            Dist_Total = (ord(Dist_H)*256) + (ord(Dist_L))

            for i in range(0,5):
                ser.read()

        print Dist_Total