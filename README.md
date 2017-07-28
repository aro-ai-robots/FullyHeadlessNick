# FullyHeadlessNick
Robokind Head Control with RaspberryPi 

Head Control Read Me

Summer AEOP Internship July 2017
Sierra Dacey and Molly Nehring

Wiring:
Each section of the head has 6 wires, 3 for the motor (Yellow, Blue, Black) and 3 for the potentiometer (Red, Green, Orange).
Each bundle of wires is labeled with a piece of tape as to what they control. Make note that the mouth open motor is currently 
non functioning. The remaining motors control the eyebrows, moving the eyes from left to right, opening and closing the eyes, 
making the mouth smile or frown, and controlling the neck.

For the motor, the yellow and blue are used for power and ground and should be attached to the respected orangutan in the M1 
or M2 spot depending. Black does not get wired to anything. 

For the potentiometer, red and green are used as a power and ground connected in some fashion to the 5V output and ground 
from the i2c bus (we simply used a breadboard for this). The orange wire is used to send information from the potentiometer 
to the orangutan and therefore should be wired to the PC1 or PC2 pin.

In the code for each orangutan, you can set which motors and pin will be used for. As of now it is set up as the following:

	orangutan 1:
		M1=Neck		(b,y)
		M2=Eyebrow	(y,b)
		PC1=Neck
		PC2=Eyebrow
	orangutan 2:
		M1=Open Eye 	(y,b)
		M2=Side to Side	(b,y)
		PC1=Open 
		PC2=Side
	orangutan 3:
		M1=Mouth Expression (y,b)
		M2=Open Mouth 
		PC1=Open 
		PC2=Expression

In the (), b respresents the blue wire and y the yellow and in order from the top down on the orangutan where they should be placed 
on the motor pin.

To get the orangutan to receive information sent from the raspberry pi, for each orangutan connect PC4 to SDA on the bus and 
PC5 to SCL (to check the connection, in the raspberry pi terminal type sudo i2cdetect -y 1 to check connection. If properly 
connected you should see each orangutan’s address) . 
 
To power the orangutan, you must connect the VIN and GND to a separate power supply. The orangutan can only be programmed if it has 
power from a separate power supply. It does not get power from the USB connection.



Raspberry Pi:
To get the raspberry pi working, do the following in the command terminal in the OS.
	sudo raspi-config
	with the down arrow and enter key select advanced options
	select i2c 
	select yes (the raspberry pi may then reboot)
	select finish with the side arrow key

When you have reterned to the normal terminal, install the i2c tools using
	sudo apt-get install -y i2c-tools

For the orangutans and pi to communicate successfully on the I2C bus SDA, SCL, and Ground wires on each orangutan must be connected 
to the SDA, SCL, and Ground on the PI.

 
	
Programing:
To connect to an orangutan from your computer you must install drivers and libraries downloaded from Pololu (the folks who make 
the orangutan). https://www.pololu.com/docs/0J17/all

Each orangutan has separate code: MouthControl, eyeControlFinal, brow_neckControl. If you make any modifications to this code, 
you will need to update the orangutan by plugging in the orangutan programmer to the orangutan you would like to change and into your computer and 
upload the new code. Note: to upload new code, the orangutan must be connected to a battery through the VIN and GND pins.

On the raspberry pi side, once the raspberry pi is plugged into a power source and has a HDMI connection, in python load and 
run the HeadControl-final code to manually change the position of each motor using the GUI.
