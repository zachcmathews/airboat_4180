#!/usr/bin/env python
import serial

with serial.Serial('/dev/serial1', baudrate=9600, timeout=1) as ser:
    while True:
        print ser.read()
