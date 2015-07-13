#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>

// specify the device name of the i2c bus
static const char *DEVICE = "/dev/i2c-1";

// specify the arduino slave address
static const int ADDRESS = 0x04;

// the amount of before rearming for the next photo
static const int CYCLE_DELAY = 5000000;

static void ctx_error_func (GPContext *context, const char *format, va_list args, void *data); 
static void ctx_status_func (GPContext *context, const char *format, va_list args, void *data);

int main(int argc, char **argv) {
	int file, photoGo = 0;
	int exitCode = 0;
	unsigned char command[16];
	unsigned char response[1];
	Camera *camera;
	int ret;
	char *owner;
	GPContext *context;
	CameraText text;

	context = gp_context_new();
	
	gp_context_set_error_func (context, ctx_error_func, NULL);
        gp_context_set_status_func (context, ctx_status_func, NULL);
		
	gp_camera_new(&camera);

	ret = gp_camera_init(camera, context);
	if (ret < GP_OK) {
		printf("No camera auto detected.\n");
		gp_camera_free(camera);
		return 0 ;
	}

	ret = gp_camera_get_summary(camera, &text, context);
	if (ret < GP_OK) {
		printf("Camera failed retrieving summary.\n");
		gp_camera_free(camera);
		return 0;
	}
	printf("Summary:\n%s\n", text.text);

	// try to open i2c device
	if ((file = open(DEVICE, O_RDWR)) < 0) {
		printf("Failed to open i2c device");
		exit(-1);
	}

	// try to connect to bus
	if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
		printf("Failed to connect to slave at address %d\n", ADDRESS);
		exit(-1);
	}

	while (1) {

		// attempt to arm the arduino
		printf("Arming the arduino...\n");

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
			printf("Failed to write code %d to arduino\n", 1);
			exitCode = -1;
		}

		// test whether countdown timer had finished
		if (photoGo == 1) {
			photoGo = 0;
			printf("Taking picture!\n");
			// picture code...

			// now disarm
			printf("Disarming...\n");

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

				printf("Arduino disarmed\n");
			} else {
				printf("Failed to write code %d to arduino\n", 2);
				exitCode = -1;
			}
		}

		printf("Trying again in %d seconds...\n", CYCLE_DELAY / 1000000);
		usleep(CYCLE_DELAY);
	}
	
	gp_camera_exit(camera, context);
	gp_camera_free(camera);
	
	return 0;
}

static void ctx_error_func (GPContext *context, const char *format, va_list args, void *data) {
        fprintf  (stderr, "\n");
        fprintf  (stderr, "*** Contexterror ***              \n");
        vfprintf (stderr, format, args);
        fprintf  (stderr, "\n");
        fflush   (stderr);
}

static void ctx_status_func (GPContext *context, const char *format, va_list args, void *data) {
        vfprintf (stderr, format, args);
        fprintf  (stderr, "\n");
        fflush   (stderr);
}
