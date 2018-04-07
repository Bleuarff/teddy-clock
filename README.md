# teddy-clock

Light-based "alarm clock": an augmented plush toy that tell children if it's night or day.

## Setup
- a raspberry pi
- 2 leds and their resistors: a yellow and a blue one
- a plush toy to hide the rpi
- a table tennis ball, to act as light diffuser

## Software

A simple python scrip that switches leds on/off based on command line parameter.
Script is run by different cron tasks at different times of the day:

    # in the morning: switch to wakeup (yellow). Different start time during week and weekends
    */15 7-11 * * 1-5 /home/pi/teddy-clock/ledclock.py debout
    */15 8-11 * * 0,6 /home/pi/teddy-clock/ledclock.py debout

    # evening and night: set blue
    */15 0-6,20-23 * * * /home/pi/teddy-clock/ledclock.py dodo

    # during the day: switch off
    */30 12-19 * * * /home/pi/teddy-clock/ledclock.py off
    
## Hardware

(insert schematics here)
