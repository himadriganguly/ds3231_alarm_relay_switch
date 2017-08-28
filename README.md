# DS3231 RTC Alaram Clock To Switch On Relay

 * The DS3231 is a low-cost, extremely accurate I2C real-time clock (RTC) with an integrated temperature-compensated crystal oscillator (TCXO) and crystal.

 ## Library
* The library used by this project can be found at [DS3231 Library](https://github.com/himadriganguly/Himadri_DS3231).

## Features

* This sketch will make an interrupt at Alarm set time on PIN 2 of arduino uno which will then activate the NPN transistor to switch on the LED which is connection on the NO(Normally Open) connection.
* The schematic and the wiring is present within this repo

## Installation

1. Before downloading the sketch install the DS3231 library that is specified previously.
1. Download zip archive from the github repository at [https://github.com/himadriganguly/ds3231_alarm_relay_switch](https://github.com/himadriganguly/ds3231_alarm_relay_switch).
2. Do the wiring using the breadboard diagram or the schematic diagram.
3. Upload the code and the RED LED will turn on every 10th second.

## Author

1. Himadri Ganguly [https://github.com/himadriganguly/](https://github.com/himadriganguly/)
