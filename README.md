# Teddy-clock

# EEPROM Layout


| Address | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 | Function
|-
|00h <td colspan="7"> | winter time change done | some flags
|01h <td colspan="8">dodo time | sleep start time: hour part
|02h <td colspan="8">dodo time | sleep start time: minute part

# Misc

- DS3231 handles leap years correctly
