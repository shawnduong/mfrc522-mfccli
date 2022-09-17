#ifndef SERIAL_H
#define SERIAL_H

/* Open a serial device. Return the fd, or a negative if errored. Takes a
   device name. */
int8_t open_serial(char *device)
{
	int8_t fd;
	struct termios tty;

	/* Open the serial device. */
	if ((fd = open(device, O_RDWR | O_NONBLOCK)) < 0)  return -1;

	/* Get tty attributes. */
	if (tcgetattr(fd, &tty) != 0)  return -2;

	/* Set baud rate to 9600. */
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	/* 8 data bits. */
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;

	/* No parity bit. */
	tty.c_cflag &= ~PARENB;

	/* 1 stop bit. */
	tty.c_cflag &= ~CSTOPB;

	/* Raw I/O. */
	tty.c_cflag |= CLOCAL | CREAD;
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_oflag &= ~OPOST;

	/* 1 second timeout. */
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 10;

	/* Apply the changes. */
	tcsetattr(fd, TCSANOW, &tty);
	tcsetattr(fd, TCSAFLUSH, &tty);

	return fd;
}

#endif
