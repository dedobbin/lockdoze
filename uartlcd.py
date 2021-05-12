#!/usr/bin/python
# uart LCD

import serial
import subprocess
import time
from time import sleep, strftime
from datetime import datetime
import RPi.GPIO as GPIO
import os


def run_cmd(cmd):
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        output = p.communicate()[0]
        return output

port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=3.0)
cmd = "ip addr show eth0 | grep inet | awk '{print $2}' | cut -d/ -f1"

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.IN,pull_up_down=GPIO.PUD_UP)
while True:
		ip = run_cmd(cmd)
		port.write("CL")
		#port.write("TT"+datetime.now().strftime('%H:%M:%S\n'))
		port.write("TTIP: \0")
		port.write("TRT");
		port.write("TT"+ip + "\0");
        port.write("CSB");
        #short pins 9 and 11 for shutdown
		print GPIO.input(17)
		if(GPIO.input(17) == False):
			run_cmd("halt")		
		time.sleep(1)
#Add wlan0 check, if is up, also print that IP

