import RPi.GPIO as GPIO
import subprocess
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(23, GPIO.IN)
GPIO.setup(24, GPIO.IN)

while True:
	if GPIO.input(23) == GPIO.HIGH:
		subprocess.run(["sh", "/home/yong/Projects/CECOM4CUT/cecom4cut_main.sh"])
	if GPIO.input(24) == GPIO.HIGH:
		subprocess.run(["sh", "/home/yong/Projects/CECOM4CUT/cecom4cut_reprint.sh"])
	time.sleep(1)
