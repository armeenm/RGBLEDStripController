#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static int handle;
struct termios oldtio;

int open_port(char *port) {
  struct termios newtio;
  int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd >= 0) {
    printf("Opening port successful...\n");
    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &oldtio);
    bzero(&newtio, sizeof(newtio));
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 2;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
  }

  return fd;
}

void *read_serial(void *vargp) {
  printf("Reading...\n");
  const int ret = read(handle, NULL, 1);
  printf("Result: %i\n", ret);

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t reader;
  int err;
  char *write_str;

  if (argc != 3) {
    printf("Usage: %s [COMMAND] [PORT]\n", argv[0]);
    return -1;
  }

  handle = open_port(argv[2]);
  write_str = (char *)malloc(strlen(argv[1]) + 1);

  if (handle < 0) {
    fprintf(stderr, "Opening port failed: %i", handle);
    free(write_str);
    return handle;
  }

  sprintf(write_str, "%s\n", argv[1]);
  pthread_create(&reader, NULL, read_serial, NULL);
  err = write(handle, write_str, strlen(write_str));
  pthread_join(reader, NULL);

  printf("Writing Result: %i\n", err);

  if (err < 0)
    fprintf(stderr, "Writing bytes failed!");
  else
    err = 0;

  close(handle);
  free(write_str);
  tcsetattr(handle, TCSANOW, &oldtio);
  return err;
}
