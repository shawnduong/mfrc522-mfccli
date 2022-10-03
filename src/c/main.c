#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
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
	uint8_t setup;
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

	setup = 1;
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
				if (setup) { puts(" done."); setup = 0; }
				send_command(fd, command, BUFFER_SIZE, DEBUG);
				break;

			/* Reading the UID. */
			case STATUS_READ_UID_SUCCESS:
				usleep(10000);
				read(fd, buffer+0, 1);
				read(fd, buffer+1, buffer[0]);
				DBG_PRINTF("DBG: UID length: %d\n", buffer[0]);
				for (int i = 0; i < buffer[0]; i++)  printf("%02X ", (uint8_t)buffer[1+i]);
				printf("\n");
				break;

			/* Reading the ATQA. */
			case STATUS_READ_ATQA_SUCCESS:
				usleep(10000);
				read(fd, buffer, 2);
				for (int i = 0; i < 2; i++)  printf("%02X ", (uint8_t)buffer[i]);
				printf("\n");
				break;

			/* Reading the SAK. */
			case STATUS_READ_SAK_SUCCESS:
				usleep(10000);
				read(fd, buffer, 1);
				printf("%02X\n", buffer[0]);
				break;

			/* Reading a block. */
			case STATUS_READ_BLOCK_SUCCESS:
				usleep(10000);
				read(fd, buffer, 1);
				DBG_PRINTF("DBG: Block: %02X\n", buffer[0]);
				usleep(10000);
				read(fd, buffer, 16);
				for (int i = 0; i < 16; i++)  printf("%02X ", (uint8_t)buffer[i]);
				printf("\n");
				break;

			case STATUS_READ_BLOCK_FAILURE:
				usleep(10000);
				read(fd, buffer, 1);
				printf("Failed to read block %d. Try authenticating first.\n", buffer[0]);
				break;

			/* Authenticating to a block. */
			case STATUS_AUTHENTICATE_SUCCESS:
				puts(" done.");
				break;

			case STATUS_AUTHENTICATE_A_FAILURE:
				puts(" failure in key A.");
				break;

			case STATUS_AUTHENTICATE_B_FAILURE:
				puts(" failure in key B.");
				break;

			/* Detecting a card. */
			case STATUS_DETECT_CARD_SUCCESS:
				puts(" done.");
				break;

			/* Resetting crypto. */
			case STATUS_RESET_SUCCESS:
				puts(" done.");
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
