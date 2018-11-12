# Teddy-clock

An alarm clock that does not display the time, but lights itself depending on the time of the day.

# Overview

An esp8266 microcontroller lights an RGB led in different colors depending whether the children must stay in their room or they are allowed to wake their parents.  
A full-on led in the dead of the night is a sure way to for kids to not go back to sleep if they ever wake up. To avoid this, a photoresistor is used to sense ambient light and adjust luminosity accordingly.  
The setup uses an RTC clock with backup battery to help keep time reliably, and daylight saving is taken into account.

The clock uses 3 parameters:
- sleep time: time at wich children must go to bed. Led becomes blue,
- wake-up times: times at which children may leve their room at wake their parents up. One for week days, and another one for weekends (one can dream of a Sundy sleep in). Led goes orange for 2 hours.

To update these times, the clock offers a web interface. It's not connected to wifi for various reasons but instead creates its own SSID when you need it.

# Wifi

When you press the button, the clock creates its own SSID. You must connect to Hérisson network (public, no authentication) and go to http://192.168.14.1 to access the web interface.  
There, you can edit any of the times (sleep time, wake-up times for weekdays and weekends) as well as the current time and date.  
The led blinks green when wifi is on. Just press the button again to switch it off. Web ui is a single html file uploaded to esp8266's internal file system (SPIFFS).

Why not having it connected permanently? Here are a few reasons:
- home wifi is flaky in the room,
- very sporadic usage (I won't edit the alarms 5 times a day) does not need 100% connection,
- lower power usage when modem is off


# EEPROM Layout

esp8266 has 4k EEPROM. We use it to store the alarms (4 bytes each) and daylight saving flag (1 if winter time has been done, LSB bit in first byte of the memory bank).

| Address | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 | Description
|-
|00h <td colspan="7"> | winter time change done (1) or not (0) | some flags
|01h <td rowspan="2" colspan="8"> dodo time <td rowspan="2"> (int) dodo time, in minutes since 00:00
|02h
|03h <td colspan="8" rowspan="2">week wakeup time <td rowspan="2"> (int) week wakeup time time, in minutes since 00:00
|04h
|05h <td colspan="8" rowspan="2">weekend wakeup time <td rowspan="2"> (int) weekend wakeup time time, in minutes since 00:00
|06h


# Conventions

- Day of week (dow): 1: Monday, 2: Tuesday, (...), 7: Sunday

# Misc

- DS3231 handles leap years correctly
