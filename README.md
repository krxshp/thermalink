# thermalink

Thermalink is an alpha prototype designed for kitchen safety. It is targeted for the use of a patient with Usher’s syndrome and features an alert system via companion app, buzzer, and LEDs. A demo is attached to this repo.


## Hardware

* 1x Arduino Uno (MCU)
* 1x MLX90614-BAA (Thermal sensor)
* 1x HC-05 (Bluetooth chip)
* 1x Push Button
* 1x Buzzer
* 2x LEDs of different colour (White and Red were used)

## Software

* MIT App Inventor
* C++

## Notes

* The MLX90614-BAA detects temperatures accurately from a distance, yet still pretty close due to high FOV. In the future, this can be upgraded with a lens to make the FOV smaller, similar to the MLX90614-DCI variant.
* Features a cooking mode through the app or from the press of a button to ensure it does not alert when the patient is cooking.
* Set to ring if the stove is above 50 degrees Celcius for an extended period of time when not in cooking mode.
