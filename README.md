# teddy-clock

Light-based "alarm clock": an augmented plush toy that tell children if it's night or day.

## Setup
- a raspberry pi to run a python script
- 2 leds and their resistors: a yellow and a blue one
- RTC module,
- a plush toy to hide the rpi
- a table tennis ball, to act as light diffuser

## Hardware

![schematics](https://github.com/Bleuarff/teddy-clock/blob/master/schematics.png)

DS3231 is connected to the RPI using the I2C protocol (pink headers in the image below).

![GPIO_headers](https://github.com/Bleuarff/teddy-clock/blob/master/RPI_B_1.0_GPIOheaders.png)

## Software

A simple python script that switches leds on/off based on command line parameter.
Script is run by different cron tasks at different times of the day:

    # in the morning: switch to wakeup (yellow). Different start time during week and weekends
    */15 7-11 * * 1-5 /home/pi/teddy-clock/ledclock.py debout
    */15 8-11 * * 0,6 /home/pi/teddy-clock/ledclock.py debout

    # evening and night: set blue
    */15 0-6,20-23 * * * /home/pi/teddy-clock/ledclock.py dodo

    # during the day: switch off


### Timekeeping

Since RPI has no real-time clock, an external one is required since this project is not meant to be connected at all time, so ntp synchronization is not possible. I use a [DS3231](https://thepihut.com/products/adafruit-ds3231-precision-rtc-breakout). See this [adafruit tutorial](https://learn.adafruit.com/adding-a-real-time-clock-to-raspberry-pi/overview) on how to configure your RPI to use it.
