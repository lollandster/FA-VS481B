#!/usr/bin/python

#Includes code from http://www.varesano.net/blog/fabio/serial%20rs232%20connections%20python

import serial

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 19200
ser.close()
ser.open()
#ser.write("read Command OK\n\r Input:port 2\n\r Output:ON\n\r Mode:Next\n\r Goto:OFF\n\r F/W:V1.0.062\n\r")
#ser.close()
inputport = "Input:port 2"
Output = "Output:ON"

while True:
    x = ser.readline()
    x = ":".join("{:02x}".format(ord(c)) for c in x)
    print(x)
    if x == "72:65:61:64:0d:0a":
        print("ok")
        ser.write("read Command OK\n\r " + inputport + "\n\r " + Output + "\n\r Mode:Next\n\r Goto:OFF\n\r F/W:V1.0.062\n\r")

    if x == "73:77:20:6f:6e:0d:0a":
        Output = "Output:ON"

    if x == "73:77:20:6f:66:66:0d:0a":
        Output = "Output:OFF"

    if x == "73:77:20:69:30:31:0d:0a":
        inputport = "Input:port 1"

    if x == "73:77:20:69:30:32:0d:0a":
        inputport = "Input:port 2"
