# KClight
Arduino code for intelligent light control.

The software main purpose is to control hardware elements through arduino platform. 
The hardware consist of:
- Arduino nano v3,
- electrical self made board with 3 x IRL540 for current light control,
- 2 x motion detection - PIR HC-SR501,
- 1 x photoresistor
- 1 x Real Time Clock DS1302

For RTC control an external library i used - RTC by Makuna

Light is being activated when motion sensors detect motion. 
Software uses Real Time Clock to activate light at dusk. On cloudy day light can be activated earlier if photoresistor detects lover level of light. 
Due to sunrise and sunset time changes everyday, an approximated function was used to follow that pattern. 
Additional function is being used to activate 'night mode' where white ligh will be changed to red for late hours.
Additional function is being used for manual light activation with switch.
