#!/usr/bin/env python
import serial
import sys

ser = serial.Serial("/dev/serial0", baudrate=115200, timeout=1)
while (True):
    while (ser.in_waiting >= 9):
        if (('Y' == ser.read()) and ('Y' == ser.read())):
            Dist_L = ser.read()
            Dist_H = ser.read()
            Dist_Total = (ord(Dist_H)*256) + (ord(Dist_L))

            for i in range(0,5):
                ser.read()

        print Dist_Total
