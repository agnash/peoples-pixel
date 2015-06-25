import time, smbus

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

while True:

    print "Arming the Arduino."

    writeCode(1)

    time.sleep(.1)

    while True:
        ready = readCode()
	print ready
	if ready == 1:
	    break

    print "Taking picture!"

    print "Disarming..."

    writeCode(2)

    time.sleep(.1)

    while True:
        ready = readCode()
	if ready == 2:
	    break

    print "Arduino disarmed."

    print "Arming again in 5 seconds..."

    time.sleep(5)

