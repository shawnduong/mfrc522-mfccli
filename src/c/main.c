#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"
#include "send.h"
#include "serial.h"

#define BUFFER_SIZE 128

#define DEBUG 1
#define DBG_PRINTF(s, ...)  if (DEBUG) printf(s, ##__VA_ARGS__);

int main(int argc, char *argv[])
{
	int8_t fd;
	char stat;
	char buffer[BUFFER_SIZE];
	char command[BUFFER_SIZE];

	if (argc != 2)
	{
		printf("Usage: %s <DEVICE>\n", argv[0]);
		return -1;
	}

	/* Open the serial device. */
	if ((fd = open_serial(argv[1])) < 0)
	{
		puts("Error opening device.");
		DBG_PRINTF("DBG: open_serial returned %d\n", fd);
		return -1;
	}

	memset(buffer, 0, BUFFER_SIZE);
	memset(command, 0, BUFFER_SIZE);

	printf("Waiting for worker ready signal...");
	fflush(stdout);
	usleep(1000);

	do
	{
		stat = -1;

		/* Read a byte from serial. */
		read(fd, &stat, 1);

		switch (stat)
		{
			/* Serial may need time to send status code. */
			case -1:
				usleep(1000);
				break;

			/* Device is ready to receive a command. */
			case STATUS_READY:
				puts(" done.");
				send_command(fd, command, BUFFER_SIZE);
				break;

			/* Else case. */
			default:
				DBG_PRINTF("DBG: Misunderstood protocol code from device: 0x%02X\n", stat);
				close(fd);
				return -1;
		}
	}
	while (strcmp(command, "exit") != 0);

	/* Close the serial device. */
	close(fd);

	return 0;
}
