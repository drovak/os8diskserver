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

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

struct
{
	char *baud_str;
	long bits_per_sec;
	long baud_val;
} baud_lookup[] =
{
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
	{"230400",230400,B230400}//,
	//{"460800",460800,B460800}
};

void setup_config(long* baud, int* two_stop, char* serial_dev)
{
	FILE *config;
	char homeloc[256];
	char *home;
	char baud_rate[32];
	int cntr;

	config = fopen("disk.cfg", "r");
	if (config == NULL)
	{
		home = getenv("HOME");
		if (home != NULL)
		{
			strcpy(homeloc, home);
			strcat(homeloc, "/");
			strcat(homeloc, ".disk.cfg");
			config = fopen(homeloc, "r");
		}
		if (config == NULL)
		{
			printf("Unable to open disk.cfg or $HOME/.disk.cfg\n");
			exit(1);
		}
	}

	fscanf(config,"%s", baud_rate);
	*baud = B0;
	for (cntr = 0; cntr < ARRAYSIZE(baud_lookup) && *baud == B0; cntr++)
	{
		if (strcmp(baud_lookup[cntr].baud_str, baud_rate) == 0)
		{
			*baud = baud_lookup[cntr].baud_val;
			break;
		}
	}
	if (*baud == B0)
	{
		printf("Unknown baud rate %s\n",baud_rate);
		exit(1);
	}
	*baud = cntr;

	fscanf(config, "%d", two_stop);
	fscanf(config, "%s", serial_dev);
	fclose(config);
}
