#!/usr/bin/python

import RPi.GPIO as GPIO
import sys

if len(sys.argv) < 2:
  print 'Missing command argument'
  sys.exit()

command = sys.argv[1]

# setup
GPIO.setwarnings(False) # avoids warning if another script has set the same pins
GPIO.setmode(GPIO.BOARD) # sets pin numbering scheme

# name our pins...
blue = 18
yellow = 22

# ...and set them as outputs
GPIO.setup(blue, GPIO.OUT)
GPIO.setup(yellow, GPIO.OUT)

# defines high & low levels for each pin based on command line argument
if command == 'debout':
  blue_level = GPIO.LOW
  yellow_level = GPIO.HIGH
elif command == 'dodo':
  blue_level = GPIO.HIGH
  yellow_level = GPIO.LOW
else:
  blue_level = yellow_level = GPIO.LOW

# applies levels to each pin
GPIO.output(blue, blue_level)
GPIO.output(yellow, yellow_level)
