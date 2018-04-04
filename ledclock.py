#!/usr/bin/python

import RPi.GPIO as GPIO
import sys

if len(sys.argv) < 2:
  print 'Missing command argument'
  sys.exit()

command = sys.argv[1]

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

blue = 18
yellow = 22

GPIO.setup(blue, GPIO.OUT)
GPIO.setup(yellow, GPIO.OUT)

if command == 'debout':
  blue_level = GPIO.LOW
  yellow_level = GPIO.HIGH
elif command == 'dodo':
  blue_level = GPIO.HIGH
  yellow_level = GPIO.LOW
else:
  blue_level = yellow_level = GPIO.LOW


GPIO.output(blue, blue_level)
GPIO.output(yellow, yellow_level)
