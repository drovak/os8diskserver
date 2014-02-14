#ifndef __OS8DISKSERVER_COMM_H
#define __OS8DISKSERVER_COMM_H

#include <unistd.h>

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))


int init_comm(char *, long, int);

static inline int ser_read(int fd, void *buf, int len)
{
	return read(fd, buf, len);
}

static inline int ser_write(int fd, void *buf, int len)
{
	return write(fd, buf, len);
}

struct baud_lookup {
	char *baud_str;
	long bits_per_sec;
	long baud_val;
};


/* This is a workaround - should be structured differently */

static const struct baud_lookup baud_lookup[] = {
	{"50",50,B50},
	{"75",75,B75},
	{"110",110,B110},
	{"134",134,B134},
	{"150",150,B150},
	{"200",200,B200},
	{"300",300,B300},
	{"600",600,B600},
	{"1200",1200,B1200},
	{"1800",1800,B1800},
	{"2400",2400,B2400},
	{"4800",4800,B4800},
	{"9600",9600,B9600},
	{"19200",19200,B19200},
	{"38400",38400,B38400},
	{"57600",57600,B57600},
	{"115200",115200,B115200},
	{"230400",230400,B230400},
	{"460800",460800,B460800}
};



#endif
