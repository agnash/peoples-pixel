// Copyright (c) 2015 Aaron Nash
//
// This source file is licensed under the terms of the "GPL (v2)" license.
// Please see the file LICENSE.md included in this distribution for licensing
// terms.
//

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>

// specify the device name of the i2c bus
static const char *DEVICE = "/dev/i2c-1";

// the name given to whatever image was last fetched from camera
static const char *CURRENT_IMAGE = "current_image";

// specify the arduino slave address
static const int ADDRESS = 0x04;

// seconds controller waits for image viewer to finish buffering image on screen
static const int SLEEP_TIME = 10;

// function prototypes
static void ctx_error_func (GPContext *context,
	const char *format,
	va_list args,
	void *data);

static void ctx_status_func (GPContext *context,
	const char *format,
	va_list args,
	void *data);

int takePicture(GPContext *context, Camera *camera, const char *filename);

// main
int main(int argc, char **argv) {

	int file, photoGo = 0;
	int exitCode = 0;
	int status;
	int ret;
	unsigned char command[16];
	unsigned char response[1];
	char *owner;
	const char *errMsg;
	GPContext *context;
	Camera *camera;
	CameraText text;

	context = gp_context_new();

	gp_context_set_error_func (context, ctx_error_func, NULL);
	gp_context_set_status_func (context, ctx_status_func, NULL);

	gp_camera_new(&camera);

	// try to find a camera, terminate if none found
	ret = gp_camera_init(camera, context);
	if (ret < GP_OK) {
		printf("No camera auto detected\n");
		gp_camera_free(camera);
		exit(-1);
	}

	// return the found camera's configuration info, terminate if error
	ret = gp_camera_get_summary(camera, &text, context);
	if (ret < GP_OK) {
		printf("Camera failed retrieving summary\n");
		gp_camera_free(camera);
		exit(-1);
	}
	printf("Summary:\n%s\n", text.text);

	// try to open i2c device, terminate if no active i2c device
	if ((file = open(DEVICE, O_RDWR)) < 0) {
		printf("Failed to open i2c device");
		exit(-1);
	}

	// try to connect to bus, terminate on failed connection
	if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
		printf("Failed to connect to slave at address %d\n", ADDRESS);
		exit(-1);
	}

	// main controller loop
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

		// test whether countdown timer has finished
		if (photoGo == 1) {
			photoGo = 0;
			printf("Taking picture!\n");

			// take the picture
			takePicture(context, camera, CURRENT_IMAGE);

			int pid;
			pid = fork();

			if (pid >= 0) {
				// fork was successful
				if (pid == 0) {
					// first child
					
					// start fbi image viewer
					printf("\tfbi: ");
					int err = execl("/usr/bin/fbi",
					"fbi",
					"-T",
					"2",
					"-d",
					"/dev/fb0",
					"-noverbose",
					"-a",
					CURRENT_IMAGE);
					
					if (err < 0) {
						errMsg = "Failed to start image viewer";
						exitCode = -1;
						break;
					}
				} else {
					// parent

					// give first child time to buffer image to screen
					printf("Controller sleeping for %d seconds...\n", SLEEP_TIME);
					sleep(SLEEP_TIME);

					// upon wake, try to kill the fbi process by forking another process
					// to killall (the fbi child forks another fbi process which does
					// run in the same group as the child and parent)
					printf("We're back! Terminating image viewer now...\n");
					int* killStatus;
					pid = fork();

					if (pid >= 0) {
						// fork was successful
						if (pid == 0) {
							// second child

							// kill fbi processes by command name
							int err = execl("/usr/bin/killall", "killall", "/usr/bin/fbi");
							
							if (err < 0) {
								errMsg = "Failed to terminate fbi processes";
								exitCode = -1;
								break;
							}
						} else {
							// parent

							// wait for second child to kill fbi processes (sends SIGTERM)
							if (pid != waitpid(pid, killStatus, 0)) {
								errMsg = "Error during image viewer termination process";
								exitCode = -1;
								break;
							}

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
					} else {
						// second child failed to fork
						errMsg = "No process to terminate image viewer";
						exitCode = -1;
						break;
					}
				}
			} else {
				// first child failed to fork
				errMsg = "No process for image viewer";
				exitCode = -1;
				break;
			}
		}
	}

	gp_camera_exit(camera, context);
	gp_camera_free(camera);

	if (exitCode < 0) {
		printf("%s\n", errMsg);
	}

	return exitCode;
}

static void ctx_error_func(GPContext *context,
	const char *format,
	va_list args,
	void *data) {
        fprintf  (stderr, "\n");
        fprintf  (stderr, "*** Contexterror ***              \n");
        vfprintf (stderr, format, args);
        fprintf  (stderr, "\n");
        fflush   (stderr);
}

static void ctx_status_func(GPContext *context,
	const char *format,
	va_list args,
	void *data) {
        vfprintf (stderr, format, args);
        fprintf  (stderr, "\n");
        fflush   (stderr);
}

int takePicture(GPContext *context, Camera *camera, const char *filename) {
	int fd, ret;
	CameraFile *file;
	CameraFilePath cameraFilePath;

	ret = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &cameraFilePath, context);

	if (ret < GP_OK) {
		printf("Camera failed to capture image\n");
		gp_camera_free(camera);
		exit(-1);
	}
	printf("Image captured was %s/%s\n", cameraFilePath.folder, cameraFilePath.name);
	fd = open(filename, O_CREAT | O_WRONLY, 0644);
 	ret = gp_file_new_from_fd(&file, fd);

 	if (ret < GP_OK) {
		printf("Couldn't create destination file on Raspberry Pi\n");
 	}

	ret = gp_camera_file_get(camera, cameraFilePath.folder, cameraFilePath.name,  GP_FILE_TYPE_NORMAL, file, context);

 	if (ret < GP_OK) {
		printf("Couldn't copy image from camera\n");
 	}

	gp_file_free(file);

	return 0;
}
