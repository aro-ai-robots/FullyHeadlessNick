#!/usr/bin/python
# Head Control
#Summer Internship July 2017
#Molly Nehring and Sierra Dacey


import time
from Tkinter import *
import smbus

# Connecting the SMBUS to the pi
# sudo i2cdetect -y 1      in terminal to get the orangutan's addresses
bus = smbus.SMBus(1)
DEVICE_ADDRESS_EYE = 0x08
DEVICE_ADDRESS_MOUTH = 0x09
DEVICE_ADDRESS_REST = 0x0a

DEVICE_REG_MODE1 = 0x0
DEVICE_REG_LEDOUT0 = 0x1d

# This starts the GUI graphics
app = Tk()
app.title('Face Adjustment app')
app.geometry('450x400+200+100')

# Default settings for all of the motors (initial face configuration)
eyeCloseSetting = 5
eyeSideSetting = 5
mouthCloseSetting = 5
mouthExpressionSetting = 5
eyebrowSetting = 5
neckSetting = 5


#non-working code to limit information sent within afew seconds

#eyeTime = int(round(time.time() * 1000))
##def eyeControl(val):
##    global eyeTime
##    eyeCloseSetting = w.get()
##    eyeSideSetting = w2.get()
##    ledout_values = [0x0f, eyeCloseSetting, 0x05, eyeSideSetting, 0x05, 0x0f, 0x00]
##    nowTime = int(round(time.time() * 1000))
##    if nowTime > eyeTime :
##        bus.write_i2c_block_data(DEVICE_ADDRESS, DEVICE_REG_LEDOUT0, ledout_values)
##        eyeTime = int(round(time.time() * 1000)) + 100
##    print nowTime
##    print eyeTime
##


# These are functions to controll the orangutans
def eyeControl(val):
    eyeCloseSetting = eyesOpenSlider.get()
    eyeSideSetting = sideSlider.get()
    ledout_val_eye = [0x0f, eyeCloseSetting, 0x05, eyeSideSetting, 0x05, 0x0f, 0x00]
    bus.write_i2c_block_data(DEVICE_ADDRESS_EYE, DEVICE_REG_LEDOUT0, ledout_val_eye)
    print val
    


def mouthControl(val):
    mouthCloseSetting = mouthOpenSlider.get()
    mouthExpressionSetting = expSlider.get()
    ledout_val_mouth = [0x0f, mouthCloseSetting, 0x05, mouthExpressionSetting, 0x05, 0x0f, 0x00]
    bus.write_i2c_block_data(DEVICE_ADDRESS_MOUTH, DEVICE_REG_LEDOUT0, ledout_val_mouth)
    print val


def neckAndBrow(val):
    eyebrowSetting = browSlider.get()
    neckSetting = neckSlider.get()
    ledout_val_rest = [0x0f, eyebrowSetting, 0x05, neckSetting, 0x05, 0x0f, 0x00]
    bus.write_i2c_block_data(DEVICE_ADDRESS_REST, DEVICE_REG_LEDOUT0, ledout_val_rest)
    print val    



# This is the code for the sliders
# Eye Open
eyeOpenLabel = Label(app, text = 'Eye Open or Shut')
eyeOpenLabel.pack()

eyeOpenSlider = Scale(app, from_=0, to=10, orient = HORIZONTAL, command=eyeControl)
eyeOpenSlider.set(5)
eyeOpenSlider.pack()

#Eye Positioning
sideLabel = Label(app, text = 'Eye Side')
sideLabel.pack()

sideSlider = Scale(app, from_=0, to=10, orient = HORIZONTAL, command=eyeControl)
sideSlider.set(5)
sideSlider.pack()

#Mouth Open/Close (not working)
mouthOpenLabel = Label(app, text = 'Mouth Open (in construction)')
mouthOpenLabel.pack()

mouthOpenSlider = Scale(app, from_=0, to=10, orient = HORIZONTAL, command=mouthControl)
mouthOpenSlider.set(5)
mouthOpenSlider.pack()

#MouthPositioning
expLabel = Label(app, text = 'Mouth Expression')
expLabel.pack()

expSlider = Scale(app, from_=0, to=10, orient = HORIZONTAL, command=mouthControl)
expSlider.set(5)
expSlider.pack()

#Neck
neckLabel = Label(app, text = "Neck Posision")
neckLabel.pack()

neckSlider= Scale(app, from_=0, to=10, orient = HORIZONTAL, command=neckAndBrow)
neckSlider.set(5)
neckSlider.pack()


#Eyebrow
browLabel = Label(app, text = "Eyebrow")
browLabel.pack()

browSlider = Scale(app, from_=0, to=10, orient = HORIZONTAL, command=neckAndBrow)
browSlider.set(5)
browSlider.pack()

app.mainloop()



