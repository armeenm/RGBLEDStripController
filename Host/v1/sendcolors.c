#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>

static int handle;
struct termios oldtio;

int openPort(char* port) {
	int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd >= 0) {
		fcntl(fd, F_SETFL, 0);
		struct termios newtio;
		tcgetattr(fd, &oldtio);
		bzero(&newtio, sizeof(newtio));
		newtio.c_cc[VTIME] = 0;
		newtio.c_cc[VMIN] = 2;
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd, TCSANOW, &newtio);
	}
	return fd;
}

void *readSerial(void *vargp) {
	read(handle, NULL, 1);
}

int main(int argc, char **argv) {
	pthread_t reader;
	int err;
	char *writeString;

	handle = openPort(argv[2]);
	writeString = (char *) malloc(strlen(argv[1]) + 1);

	if (handle < 0) {
		fprintf(stderr, "Opening port failed!");
		free(writeString);
		return handle;
	}

	sprintf(writeString, "%s\n", argv[1]);
	pthread_create(&reader, NULL, readSerial, NULL);
	err = write(handle, writeString, strlen(writeString));
	pthread_join(reader, NULL);

	if (err < 0)
		fprintf(stderr, "Writing bytes failed!");
	else
		err = 0;

	close(handle);
	free(writeString);
	tcsetattr(handle, TCSANOW, &oldtio);
	return err;
}
