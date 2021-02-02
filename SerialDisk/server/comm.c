/*
	comm.c: based on David Gesswein's dumprest/comm.c
*/

#define ser_read(a,b,c) read(a,b,c)
#define ser_write(a,b,c) write(a,b,c)

#ifdef _STDC_
int init_comm(char *, long, int);
#endif

int init_comm(char* port, long baud, int two_stop)
{
	struct termios tios;					/* Serial port TERMIO structure */
	int port_fd;

	port_fd = open(port, O_RDWR, 0);
	
	if (port_fd < 0)
	{
		fprintf (stderr, "init_comm: Open failed on port '%s': ", port);
		perror("");
		exit(1);
	}

	/* Set to 8 bit selected baud no parity with no special processing */
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

	if (tcsetattr(port_fd,TCSANOW,&tios) < 0)
	{
		perror("init_comm: tcsetattr failed");
		exit(1);
	}
	
	tcflush(port_fd,TCIOFLUSH);
	
	return(port_fd);
}

void flush(int fd)
{
	tcflush(fd, TCIOFLUSH);
}
