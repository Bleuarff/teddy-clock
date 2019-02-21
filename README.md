# Teddy-clock

An alarm clock that does not display the time, but lights itself depending on the time of the day.

## Overview

An esp8266 microcontroller lights an RGB led in different colors depending whether the children must stay in their room or they are allowed to wake their parents.  
A full-on led in the dead of the night is a sure way to for kids to not go back to sleep if they ever wake up. To avoid this, a photoresistor is used to sense ambient light and adjust luminosity accordingly.  
The setup uses an RTC clock with backup battery to help keep time reliably, and daylight saving is taken into account.

The clock uses 3 parameters:
- sleep time: time at which children must go to bed. Led becomes blue,
- wake-up times: times at which children may leve their room at wake their parents up. One for week days, and another one for weekends (one can dream of a Sundy sleep in). Led goes orange for 2 hours.

To update these times, the clock offers a web interface. It's not connected to wifi for various reasons but instead creates its own SSID when you need it.

## Schema

![schema](https://raw.githubusercontent.com/Bleuarff/teddy-clock/master/diagram.png)

## Wifi

When you press the button, the clock creates its own SSID. You must connect to network **Hérisson** (public, no authentication). Ease of use trumps everything in such a simple, low-risk project. SSL and auth (WPA2-PSK) are not worth the hassle, both for dev and use. Wifi is off most of the time anyway.  
Go to **http://192.168.14.1** to access the web interface. There, you can edit any of the times (sleep time, wake-up times for weekdays and weekends) as well as the current time and date. Web ui is a single html file uploaded to esp8266's internal file system (SPIFFS).  
The led blinks green when wifi is on. Just press the button again to switch it off.

Why not having it connected permanently? Here are a few reasons:
- home wifi is flaky in the room,
- very sporadic usage (I won't edit the alarms 5 times a day) does not need 100% connection,
- lower power usage when modem is off


## EEPROM Layout

esp8266 has 4k EEPROM. We use it to store the alarms (4 bytes each) and daylight saving flag (1 if winter time has been done, LSB bit in first byte of the memory bank). Alarm times are stored in minutes since 00:00.

````
Address |  value   | Desc  
--------|----------|------
00h     | 00000001 | winter time change done  
01h-04h |          | sleep time  
05h-08h |          | weekdays wakeup time  
09h-12h |          | weekend wakeup time
````

## Conventions

- Day of week (dow): 1: Monday, 2: Tuesday, (...), 7: Sunday

## Calibration

Light output is proportional to ambiant light. The relation is not the simple identity function f(x) = x.
To create the ideal function, we want to plot values.
The sketch `calibrage` creates a web server (public SSID Hérisson, 192.168.14.1) with endpoints:  
- `GET /sense` to retrieve ambiant light value (theoretically 0-1024),  
- `POST /led?r=R&g=G&b=B` to set led output ; R, G and B being the PWM values for each channel.

We try to find the ideal output in each color (sleep blue and wakeup orange) for multiple ambiant light values. Results are in `resultats_calibrage.ods` file.  
For each channel, we want to write a function that more or less reflects the calibration data points. The ideal function should be superimposed if plotted along the data points. We do that by considering that the values between 2 data points should observe an affine function (with data range extremities being minimum & maximum values).  
`tests/pwm_function/pwm_function.cpp` implements the affine functions and compares with calibration data to verify that's ok ; and `herisson/lightModulator.ino` simply copies this code.

## Misc

- DS3231 handles leap years correctly
- Daylight saving time change is checked and performed on sunday in the last 7 days of march and october. Update time and set winter time flag in eeprom.
