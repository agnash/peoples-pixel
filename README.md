Copyright (c) 2015, Aaron Nash, Dominic Schira, and James Cuadros. This file is licensed under the terms of the "GPL (v2)" license. Please see the file LICENSE.md included in this distribution for licensing terms.

Contact and contributing information is at the bottom of this readme file.

# The People's Pixel

This project combines a Raspberry Pi and Arduino Uno, along with necessary peripherals, to form a compact
system capable of performing user-initiated image capture and presentation, such as would be typical in a "photo-booth" machine. The system currently supports the use of a segment display as a countown timer and a button as the trigger.

This take on a DIY, portable photobooth solution is unique in that it allows builders to make use of the amateur or professional camera they may already have (compatable camera list is linked to below). Since the system separately handles all hardare accessory control via a microntroller, folks with some programming and electronics experience can extend the machine to make use of even more 'gizmos' that may enhance the user experience (i.e. a flash bulb, or print-to-tape device).

The system has been built into an initial working prototype. Our version represents only one possible enclosure design and equipment configuration. For some pictures of construction check out our [short photo blog](https://github.com/agnash/peoples-pixel/wiki/Blog). <add picture of startup splash screen to blog>

# Disclaimer
This project may involve some basic work with electronics. While the methods provided here have been tested under certain circumstances the authors of this repository are not responsible for any damage you cause to your equipment or to yourself or others. You agree to this by running any of the code or following any of the instructions included in this repository.

Please see LICENSE.md for the warrantly disclaimer.

# Platform Compatibility
The software here is only expected to run on the Raspberry-Pi (running Raspbian Wheezy) and Arduino microcontroller. Do not expect code within the raspberry-pi directory to run or even compile on other linux computers without extra configuration. The Raspberry-Pi code has been tested on the Model A+, B+, and the Version 2 Model B. The arduino code has been tested on the Arduino Uno Rev 3 and the Arduino Mega 2560 models.

# Dependencies
(installation instructions included below)

libgphoto2

Linux frame buffer application(FBI)

# Parts List

Raspberry Pi (With at least 2 USB ports)

micro-USB power cable

Ethernet cable or HDMI cable (To use the pi) 

Arduino Uno Rev 3 or Arduino Mega 2560

SparkFun Logic Level Converter - Bi-Directional

Large or Massive Arcade Button with LED - 60mm or 100mm Red

7-segment common cathode LED display 

1x Full size breadboard

Large jumper cable pack (male to male)

Female to female or female to male jumper cable pack

1/4 watt resistor kit

Digital camera from the following list: http://www.gphoto.org/proj/libgphoto2/support.php


# Set up Raspberry Pi
After [installing](https://www.raspberrypi.org/documentation/installation/installing-images/README.md) Raspbian Wheezy ...

1: Run the command:
``` $ df -h ```
to make sure your file system is roughly size the of your sdcard (less the size of the OS. You can confirm the size of the card with $ sudo fdisk -l). If for some reason the file system is really small compared what you were expecting, run

``` $ sudo raspi-config ```
and select the option for increasing the size of the filesystem. Exit the config app.

2: Run the command:

``` $ sudo apt-get update ```

3: Run the command:

``` $ sudo raspi-config ```
and select advanced options. Then select the option for enabling i2c hardware at startup. Exit the config app.

4: Run the command:

``` $ sudo nano /etc/modprobe.d/raspi-blacklist.conf ```

'i2c-bcm2708' shouldn't be blacklisted so make sure that line is commented out. i.e.
``` #blacklist i2c-bcm2708 ```

Save the file if changed and exit nano. (If the file is empty then just move on)


5: Run the command:

``` $ sudo nano /etc/modules ``` Append the lines...
```
i2c-bcm2708 
i2c-dev 
```
if they're not already there. Save the file if changed and exit nano.

6: Reboot the raspberry-pi

7: Install git (It might already be installed - confirm with $ git --version):

``` $ sudo apt-get install git ```

8: Get libgphoto2 and development files

``` $ sudo apt-get install libgphoto2-2 libgphoto2-2-dev```

9: Get the Linux frame buffer application

```$ sudo apt-get install fbi```

10: Clone the peoples-pixel repo:

```$ cd ~ ```
```$ git clone https://github.com/agnash/peoples-pixel.git```

11: Run...
```
$ cd peoples-pixel/raspberry-pi
$ sudo ./setup.bash
$ pp 
```
(the actual program...will fail unless camera and Arduino is properly configured, powered on and connected)

# Set up Arduino
Follow these steps to setup the Arduino microcontroller...

1: [Download and install](https://www.arduino.cc/en/main/software) the Arduino IDE
<AARON ADD ARDUINO STUFF HERE>

# Contact and Contributing
Please see the profile pages for [Aaron Nash](https://github.com/agnash) and [Dominic Schira](https://github.com/domshyra) and [James Cuadros](https://github.com/jamescuadros) for contact information.

See the issues section for current issue tracking. If you'd like to contribute first fork your own copy of the repository. After commiting the feature branch containing your changes to your own repository create a pull request targeting this one. If approved, all changes should go into the integration branch.
