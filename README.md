# Thermo-Clock
Complete project for the creation of an electrical circuit (with Arduino Nano) for a clock that measures temperature and humidity.

## Description
This project enables you to build a digital clock that can measure the surrounding environment's temperature and humidity using an Arduino. The clock
An Arduino Nano was chosen due to its small size and the lower number of pins required for use compared to an Arduino Uno or Mega.
The clock has the following navigation features:
- Date and time in HH:mm format
- Date and time in HH:mm:ss format
 - Temperature (in degrees Celsius) and humidity (as a percentage).

You can set the time to a specific date by following these steps:
 - The hours in 24-hour format
 - the minutes
 - reset the seconds

In addition, the clock has a function for consulting and modifying the time drift compensation value. This compensation is necessary because Arduino boards (and their Chinese counterparts) often lack a precise quartz crystal for real-time clock measurement. However, compensation can be avoided by including an external RTC board (e.g. DS1307 or DS3231), which provides a more stable and accurate clock. To omit the drift compensation, set the value to 0.

## Bill of Material
 - Arduino Nano (_1_ pcs)
 - Display SH1106 OLED I2C (128x64) (_1_ pcs)
 - DHT11 temperature-and-humidity sensor (_1_ pcs)
 - Pushbuttons (_2_ pcs)
 - 10k resistor (_1_ pcs)

## References
This project uses some files previously built as library for DHT11 sensor. If you want to deep dive, please follows [this link](https://github.com/andrea-deri/DHT11Reader).
