/*
	comm.c: based on David Gesswein's dumprest/comm.c
*/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>

#include "comm.h"
#include "config.h"

int init_comm(char* port, long baud, int two_stop)
{
	struct termios tios; /* Serial port TERMIO structure */
	int port_fd;
	
	port_fd = open(port, O_RDWR, 0);
	
	if (port_fd < 0)
	{
		fprintf (stderr, 
			 "init_comm: Open failed on port '%s': ", port);
		perror("");
		exit(1);
	}

	/* 
	 * Set to 8 bit selected baud no parity 
	 * with no special processing
	 */
	if (tcgetattr(port_fd,&tios) < 0)
	{
		perror("init_comm: tcgetattr failed");
		exit(1);
	}
	
	tios.c_cflag = CS8 | CREAD | HUPCL | CLOCAL | baud;
	if (two_stop)
		tios.c_cflag |= CSTOPB;
	tios.c_iflag = 0;
	tios.c_lflag = 0;
	tios.c_oflag = 0;
	tios.c_cc[VMIN] = 0;
	tios.c_cc[VTIME] = 1;

	if (cfsetispeed(&tios,baud) != 0)
		printf("init_comm: set ispeed failed\n");

	if (cfsetospeed(&tios,baud) != 0)
		printf("init_comm: set ospeed failed\n");

	if (tcsetattr(port_fd,TCSANOW,&tios) < 0) {
		perror("init_comm: tcsetattr failed");
		exit(1);
	}
	
	tcflush(port_fd,TCIOFLUSH);

	printf("Using serial port %s at %s with %s\n", 
	       port, baud_lookup[baud - 1].baud_str, 
	       (two_stop ? "2 stop bits" : "1 stop bit"));
	
	return(port_fd);
}

void flush(int fd)
{
	tcflush(fd, TCIOFLUSH);
}
