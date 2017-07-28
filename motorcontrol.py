import smbus
import Tkinter
import time

bus = smbus.SMBus(1)

DEVICE_ADDRESS = 0x08
DEVICE_REG_MODE1 = 0x0
DEVICE_REG_LEDOUT0 = 0x1d

openEyes = 0x0a
closeEyes = 0x50

#for blah in range(1,3):

#ledout_values = [0x0a, 0x07, 0x08, 0x07, 0x01, 0x0f, 0x00]
#bus.write_i2c_block_data(DEVICE_ADDRESS, DEVICE_REG_LEDOUT0, ledout_values)



#second in list is to open and close, fourth in list is to make happy or sad
ledout_value = [0x0a, 0x02, 0x08, 0x01, 0x01, 0x0f, 0x00]
bus.write_i2c_block_data(0x09, DEVICE_REG_LEDOUT0, ledout_value)




    #time.sleep(1.5)

##    ledout_values = [0x0f, closeEyes, 0x04, 0x06, 0x08, 0x0f, 0x00]
##    bus.write_i2c_block_data(DEVICE_ADDRESS, DEVICE_REG_LEDOUT0, ledout_values)
##
##    time.sleep(1.5)

print 'test'
        
