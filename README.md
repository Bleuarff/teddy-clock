# Teddy-clock

# EEPROM Layout


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

- Date: 1: Monday, 2: Tuesday, ... 7: Sunday

# Misc

- DS3231 handles leap years correctly
