import smbus

# create i2c bus object on i2c port 1
bus = smbus.SMBus(1)

# specify the same address as used for the Arduino Uno slave
address = 0x04

def writeCode(value):
    bus.write_byte(address, value)
    return -1

def readCode():
    number = bus.read_byte(address)
    return number
