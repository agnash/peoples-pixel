#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

// specify the device name of the i2c bus
static const char *DEVICE = "/dev/i2c-1";

// specify the arduino slave address
static const int ADDRESS = 0x04;

// the amount of before rearming for the next photo
static const int CYCLE_DELAY = 5000000;

int main() {
	int file, photoGo = 0;
	int exitCode = 0;
	unsigned char command[16];
	unsigned char response[1];
	
	// try to open i2c device 
	if ((file = open(DEVICE, O_RDWR)) < 0) {
		cout << "Failed to open i2c device" << endl;
		return -1;
	}

	// try to connect to bus
	if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
		cout << "Failed to connect to slave at address " << ADDRESS << endl;
		return -1;
	}

	while (true) {
		
		// attempt to arm the arduino
		cout << "Arming the arduino..." << endl;
		
		command[0] = 1;
		if (write(file, command, 1) == 1) {
			
			// write was successful, give the arduino a second to
			// process before polling for response
			usleep(1000000);
			
			// now start polling for response
			
			while (read(file, response, 1) == 1) {
				if ((int) response[0] == 1) {
					photoGo = 1;
					break;
				}
			}
		} else {
			cout << "Failed to write code " << 1 << " to arduino" << endl;
			exitCode = -1;
		}
		
		// test whether countdown timer had finished
		if (photoGo == 1) {
			photoGo = 0;
			cout << "Taking picture!" << endl;
			// picture code...

			// now disarm
			cout << "Disarming..." << endl;
			
			command[0] = 2;
			if (write(file, command, 1) == 1) {
				
				// write was successful, give the arduino a second to
				// process before polling for response
				usleep(1000000);
				
				// now start polling for response	
				while (read(file, response, 1) == 1) {
					if ((int) response[0] == 2) {
						break;
					}
				}
				
				cout << "Arduino disarmed" << endl;
			} else {
				cout << "Failed to write code " << 2 << " to arduino" << endl;
				exitCode = -1;
			}
		}
		
		cout << "Trying again in " << CYCLE_DELAY / 1000000 << " seconds..." << endl;
		usleep(CYCLE_DELAY);
	}
	return 0;
}

