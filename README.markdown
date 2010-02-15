
## ProcrasDoCoder Overview

The ProcrasDoCoder Ring consists of a bright RGB LED with long wires to
an arduino unit hidden in one's pocket or belt loop. The LED lights up
colorfully and brilliantly for a few seconds whenever a user of
ProcrasDonate donates to a charity, makes a pledge, downloads our
software or participates in some user test.

<img src="http://github.com/diN0bot/ProcrasDoCoder/raw/master/docs/circuit2.JPG" width="25%" />

## Motivation

The ProcrasDoCoder Ring is meant to:
 *     be a conversation starter for my startup, [ProcrasDonate](http://procrasdonate.com)
 *     give the team a warm fuzzy feeling while we're working 
 *     be a fun introduction to microcontrollers

## How It Works

This is a simple mashup that stands on the shoulders of 4 giants:

    Arduino + WiShield + BlinkM + ProcrasDonate web server

In case it's not clear: much of the WiShield and BlinkM code is copied
from relevant examples.

1. Arduino - Arduino starter pack from [AdaFruit](http://adafruit.com), snap.

2. WiShield - from [AsyncLabs](http://asynclabs.com/wiki/index.php?title=WiShield_1.0)

    Nice library. Needed to hack a few config lines in the C code for WEP
    5 byte passcode (see section below), but otherwise comes with good
    documentation.

3. BlinkM - RGB LED kit from [ThingM](http://blinkm.thingm.com/)

    Comes with a nice arduino library that abstracts fading the LED to a
    specific RGB or HSB.

4. [ProcrasDonate](http://ProcrasDonate.com) web server - python [Django](http://djangoproject.com) website

    Two different ways this could work:

    Arduino registers itself with ProcrasDonate site, which then pushes
    information to the arduino using HTTP web requests to the WiShield
    webpage server.

    Instead, arduino regularly pings a small Django app running on
    ProcrasDonate. The app has a [single
    webpage](http://procrasdonate.com/procrasdocoder), which returns the
    donation counts, etc.


## WiSheild Notes

0. Place WiShield folder in Arduino's libraries folder:

     ~/Documents/Arduino/libraries/WiShield

1. Edit app-conf.h so that WISERVER is defined:

       //Here we include the header file for the application(s) we use in our project.
       //#define APP_WEBSERVER
       //#define APP_WEBCLIENT
       //#define APP_SOCKAPP
       //#define APP_UDPAPP
       #define APP_WISERVER

Delete any .o files and applet directories, if any (I never had any).

When add a new library, restart Arduino application.

When edit library files, simply compile ("verify") the Arduino app to compile new library files.

2. WPA worked out of the box. WEP may require editing g2100.c keyLen:

WEP passwords are in hex.
 *    64-bit, up to 10 hex values = 5 bytes
 *    128-bit, up to 26 hex values = 13 bytes

For example, the password "12345abcde" goes to { 0x12, 0x34, 0x5a, 0xbc, 0xde }

The keyLen variant in g2100.c must be set to 5 or 13 as necessary:

     cmd->keyLen = 5; // Key length: 5 bytes (64-bit WEP); 13 bytes (128-bit WEP)

3. The Arduino app must specify specific wireless configuration parameters. There is not dynamic detection, though one could cycle through possibilities.

"ssid" is the network name

The gateway IP address is often 192.168.1.1. In order to not conflict
without machines on the local network, use a high number for the
WiShield's local IP address, eg 192.168.1.222.

The subnet mask specifies the range of available local IP
addresses. Only the part of the IP address that filter through a 0
rather than a 1, when both are written in binary, can be
changed. Essentially, the 192.168.1 are constant, and the last number can
range from 0 to 255.

     11000000.10101000.00000001.00000001  192.168.1.1 gateway ip in binary
     11111111.11111111.11111111.00000000  255.255.255.0 subnet mask in binary
     -----------------------------------
     11000000.10101000.00000001.xxxxxxxx  x's can be 1 or 0, your choice

## Django app

This code base is actually a Django app sliced from the ProcrasDonate
code base. You can learn more about ProcrasDonate-specific Django
integration on [my blog](http://proudly.procrasdonate.com).

## Hardware

The three boards, from left to right, are Arduino, wifi shield and
prototype shield with BlinkM, red debug LED and debug switch. The three
boards stack right on top of each other so that the Arduino's pins, like
columns, pop through the layers.

<img src="http://github.com/diN0bot/ProcrasDoCoder/raw/master/docs/layers.JPG" width="25%" />

Close up of the top layer, the prototype shield. 

 *    The BlinkM is plugged directly into Analogue pins 2,3,4,5. This is
      just for development. The real ring, or hat pin, will separate the
      BlinkM across a few feet of wires.

 *    The red LED is plugged into Digital pin 7 and a 1 KOhm resistor to ground.

      It is used for debugging to show, for instance, when a successful
      HTTP Reresponse is sent or received.

 * The switch is plugged into Digital pin 6 across a 100 Ohm
      resistor, with a 10 KOhm pull-down resistor to ground.

<img src="http://github.com/diN0bot/ProcrasDoCoder/raw/master/docs/circuit2.JPG" width="25%" />

[This
video](http://www.youtube.com/watch?v=yvCtAjzHpDE&feature=youtube_gdata)
shows the out-of-the-box BlinkM fading sequence. I'll post an update
once I make the proper ring and housing unit.

## Feedback

What do you think? Bugs, ideas, corrections?
