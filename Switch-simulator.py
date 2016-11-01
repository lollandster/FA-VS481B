#!/usr/bin/python

#Includes code from http://www.varesano.net/blog/fabio/serial%20rs232%20connections%20python

import serial

ser = serial.Serial('/dev/ttyS0')
ser.baudrate = 19200
ser.close()
ser.open()
ser.write("read Command OK\n\r Input:port 2\n\r Output:ON\n\r Mode:Next\n\r Goto:OFF\n\r F/W:V1.0.062\n\r")
ser.close()
