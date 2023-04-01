# digital_clock_arduino
Design of a Digital Clock Using Arduino


1. The LCD should display a running digital clock aligned to the center of the screen
with hours, minutes, seconds in 24-hour format (military time format)
2. The clock display format should be of the format HH:MM:SS for showing the 2-digit
hours, 2-digit minutes, and 2-digit seconds
3. The LCD screen should be initialized as follows: background color should be
green, the display text color should be black, and the text size should be 6
4. The blue LED should do a quick blink (fast turn on and then fast turn off) for every
change in second
5. The push button WIO_KEY_B should be used to only increase the hour of the
running digital clock and the hour should roll over to 00 after 23
6. The push button WIO_KEY_A should be used to only increase the minute of the
running digital clock and the minute should roll over to 00 after 59
7. The minute should not roll over to the hour when the push buttons WIO_KEY_A is
used to change the minutes
Page 2 of 2
8. When the hour or minute is changed using the push buttons WIO_KEY_A or
WIO_KEY_B it should generate a beep tone
9. The LCD screen’s backlight should be turned off when the Wio Terminal board is
still and stationary for five seconds
10.When the MKII board is rotated about the x-axis (~45° rotation), the LCD screen’s
backlight should be turned on and running clock should be displayed again on the
LCD
11.Do not use any delay loops in the code and make sure all the push button inputs
(WIO_KEY_A and WIO_KEY_B) are configured as interrupt-based inputs.
