# Logbook
##### Jamil ETTEL je366

## 12/02/2022

Trying to find the idea for the project, here are some:

- RFID + Qr Code -> customer orders -> notification on phone -> scan tag -> pick up your order: Is not going to work because is unrelated to the home.
- More detailed tracker for sports using an accelerometer (too easy?)
- Radio monitoring device, detects all the signal available at all times and logs everything to a web server.
- Smart toilet paper. Will tell you how much paper you have left.

## 17/02/2022

After not finding any better ideas, I decided to go with the smart toilet paper dispenser project.

Did some sketches for the poster and now everything. The toilet paper dispenser will use the following:
- 1 × PIR sensor (for detecting presence)
- 1 × IR distance sensor (for detecting toilet paper)
- 1 × Ultrasound sensor (for detecting toilet paper)
- 1 × continuously rotating servomotor (for unrolling the toilet paper roll)

The toilet paper dispenser will be able to estimate the amount of toilet paper left by computing the diameter of the toilet paper roll. It will do that by checking the angle that is needed to unroll a single toilet paper roll (which has a known height).

The IR distance sensor will be placed to check for toilet paper presence, the ultrasound sensor will be placed a bit lower (1 toilet paper sheet's height lower).

## 18/02/2022

I presented the poster and got some feedback. Will abandon the ultrasound sensor in favor of a second IR sensor.
The idea overall is good.

I also went to the Shed today to bring some of the sensor and servo.
They didn't have a continuously rotating servo, so I ordered one online.

## 01/03/2022

I have tried plugging in the IR sensor and it works. This was my first time dealing with any electronics, so it took me some time to figure out everything, and make sure I wasn't going to fry any components today.

I had some trouble with the servomotor, will have to check that out another day.