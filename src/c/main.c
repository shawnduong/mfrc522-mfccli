#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"
#include "serial.h"

#define BUFFER_SIZE 128

#define DEBUG 1
#define DBG_PRINTF(s, ...)  if (DEBUG) printf(s, ##__VA_ARGS__);

int main(int argc, char *argv[])
{
	int serfd;
	char stat;
	char buffer[BUFFER_SIZE];
	char command[BUFFER_SIZE];

	if (argc != 2)
	{
		printf("Usage: %s <DEVICE>\n", argv[0]);
		return -1;
	}

	/* Open the serial device. */
	if ((serfd = open_serial(argv[1])) < 0)
	{
		puts("Error opening device.");
		DBG_PRINTF("DBG: open_serial returned %d\n", serfd);
		return -1;
	}

	printf("Waiting for worker ready signal...");
	fflush(stdout);
	usleep(1000);

	do
	{
		stat = -1;

		/* Read a byte from serial. */
		read(serfd, &stat, 1);

		switch (stat)
		{
			/* Serial may need time to send status code. */
			case -1:
				usleep(1000);
				break;

			/* Device is ready to receive a command. */
			case STATUS_READY:
				puts(" done.");
				break;

			/* Else case. */
			default:
				DBG_PRINTF("DBG: Misunderstood protocol code from device: 0x%02X\n", stat);
				close(serfd);
				return -1;
		}
	}
	while (strcmp(command, "exit") != 0);

	/* Close the serial device. */
	close(serfd);

	return 0;
}
