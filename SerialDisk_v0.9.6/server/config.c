/* 
	config.c: based on David Gesswein's dumprest/config.c

	Config file: disk.cfg or $HOME/.disk.cfg

	Baud rate:
	0 if 1 stop bit or 1 if two stop bits
	serial device to use

	Example:
	9600
	0
	/dev/ttyS1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "config.h"
#include "comm.h"


/* 
 * This should probably go anywhere else or use an environment variable!
 */
#define CONFFILE "os8disk"


void setup_config(long *baud, int *two_stop, char *serial_dev)
{
	FILE *conffile;
	char homeloc[256];
	char *home;
	char baud_rate[32];
	int cntr;
	
	
	conffile = fopen(CONFFILE ".cfg", "r");
	if (conffile == NULL) {
		home = getenv("HOME");
		if (home != NULL) {
			strcpy(homeloc, home);
			strcat(homeloc, "/");
			strcat(homeloc, "." CONFFILE);
			conffile = fopen(homeloc, "r");
		}
	}

	fscanf(conffile,"%s", baud_rate);

	*baud = B0;
	
	
	for (cntr = 0; cntr < ARRAYSIZE(baud_lookup) && *baud == B0; cntr++)
		if (strcmp(baud_lookup[cntr].baud_str, baud_rate) == 0)
			*baud = baud_lookup[cntr].baud_val;
	
 	if (*baud == B0) {
 		printf("Unknown baud rate %s\n",baud_rate);
 		exit(1);
 	}

	fscanf(conffile, "%d", two_stop);
	fscanf(conffile, "%s", serial_dev);
	fclose(conffile);
}
