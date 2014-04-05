## Train Track Controller

Software to control and detect toy train track conditions using an Arduino Mega 2560.

A Mega is used to interface with the large number of track inputs and outputs, although
any Arduino should be capable with the right external interfaces.

As it stands, the track is a O-27 gauge track with two lines that connect together
into a short straight track, i.e.:
```
/------\
|      |
/----\ |
|    | |
\----/ |
|      |
\------/
```
The two points, or switches allow a train to change lines easily. Each line has
six relays to control the power to a portion of a the track. Additionally, each
approach to the points (of which there are four), has two sensors to detect a 
train approaching and determine its direction, for a total of eight analog inputs.
The initial version of this software only displays the analog values, it does
nothing with them.

Speaking of displays - the design relies on a 2x16 LCD display for informing the
user about the state of the system and displays prompts for commands.

It also uses a 4x4 keyboard matrix to accept user input. The normal input scenario
is to switch the points, thus a keyboard input of 2 followed by an up arrow would
move the point 2 servo to switch the track so the train can go on the upper (outside)
track. The red 2nd Function button turns on the entire track and the clear button
turns off all the track. A number followed by either of those buttons enables
or disables that track segment of which there are 12.

Various libraries are used to provide the needed functionality.

First checked-in code represents the same code used to demonstrate the 
train at the 2014 Seattle Mini Maker Faire.

For more info zapmaker.org

Code is Apache 2.0 license.
