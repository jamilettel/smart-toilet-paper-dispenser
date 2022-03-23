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
- 1 × continuously rotating servomotor (for unrolling the toilet paper roll). I ordered the ds04-nfc motor.

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

## 08/03/2022

Trying to get the servo motor to work. Done!

Using the ESP32Servo library and pin 12/D13 as the data pin for the servo (because it's a PWM pin), I can initialize the Servo object, attach it, and then manipulate the servo using Servo::writeMicroseconds with 1500 being stop, 1000 being 100% backwards and 2000 100% forwards (got it from [this](https://elektro.turanis.de/html/prj036/index.html) website).

Now I will go to the shed and try to make the mount for the toilet paper roll.

It has been done. I've used an empty coca cola plastic bottle.

I have also somewhat calibrated my PIR sensor.

## 09/03/2022

Trying to design the box in which the device goes into.

Decided not to go with a very sophisticated design for the initial prototype, will just stick some cardboard together to barely make it work. Will need to buy some scissors and duct tape tomorrow.

## 10/03/2022

Trying to build the first prototype out of cardboard.

What I need to have place for:
- the servo (middle)
- the PIR (on top)
- IR1 (under servo, where we will detect toilet paper presence)
- IR2 (1 toilet paper sheet height under IR1)

I managed to build everything out of empty Coca Cola bottles and carboard, but kind of burned the PIR sensor. Will need to replace it tomorrow.

Now trying to get everything (exept the PIR) to work together, and hopefully be done with most of the arduino code today.

Managed to start working on the calibration of the servo.

## 11/03/2022

Trying to finish the calibration and hopefully start working on the connectivity part of the object.

Calibration done and tested. Works pretty well, but not very accurate; probably due to the flimsy nature of the build.
You can now calibrate, and then find out the perimeter of the current toilet paper. Using the perimeter, I can estimate the percentage left.

Will now try to start working on the webserver that uses a websocket to communicate with the IoT device, and normal http to communicate
with the interactive webapp.

Did not get much done.

## 14/03/2022

Working on the design of the IoT device. I must be able to 3d print it afterwards.

Found a website for creating the model, Tinkercad. Will work on that.

Good progress today on the design. The main housing of the toilet paper is done. Remains the under part of the object containing the IR sensors and the ESP32.

<img src="images/3d model start.png">

## 18/03/2022

Today, I finished the design of the IoT device on Tinkercad. Will need to see in the shed next week to print it.

<img src="images/3d model end.png">

## 22/03/2022

Going to the shed to try to 3d print the device.

Talked with Keith, we should probably lazer cut the device as it only has flat surfaces. Went back home to get everything in the right format, and now going back to the shed.

Lazer cut it and assembled it. Worked out great, everything fits nicely. Added empty bottles to hold the toilet paper.

Calibration now works a lot better.

<img src="images/assembled iot 1.jpg">

TODO list:
- Make any servo actions (e.g., calibration) non-blocking so I can easily handle errors (e.g., toilet paper runs out).
- Add the PIR sensor
- Work on the server & connectivity part

Made the servo non blocking, need to handle tpr errors next time I work on the project.