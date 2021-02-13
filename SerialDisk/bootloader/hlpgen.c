#include <stdlib.h>
#include <stdio.h>

int boot2[] = {
	00000, 00000, 00000, 05033, 07032, 07012, 01003, 03037,
	04020, 03002, 04020, 03013, 05010, 00000, 00000, 00000,
	00000, 04033, 07006, 07006, 07006, 03000, 04033, 01000,
	03030, 05021, 05004
};
int boot3[] = {
	05420, 00000, 03402,	// Actually the required patch
	00042, 04020,		// BOOT3 proper
	00043, 03002,
	00044, 04020,
	00045, 03001,
	00046, 04020,
	00047, 03050,
	00050, 00000,
	00051, 04020,
	00052, 03402,
	00053, 02002,
	00054, 02001,
	00055, 05051,
	00056, 05042,
	00057, 05460,
	00060, 07605,
	00014, 05042,		// Kludge to start BOOT3
};

void
die(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	abort();
}

int
main()
{
	int i;

	if (boot2[0] & 04)
		die("Illegal initial word");
	for (i = 0; i < sizeof(boot2)/sizeof(*boot2); i++) {
		unsigned int intval;
		unsigned char byteval;
		int link;

		if (boot2[i] & 0740) {
			fprintf(stderr, "Illegal bit set in %04o at %04o\n",
					boot2[i], i);
			abort();
		}
		link = 0;
		if (i+1 < sizeof(boot2)/sizeof(*boot2))
			link = boot2[i+1] & 01000;
		intval = (link<<3) | boot2[i];
		byteval = (intval<<3) | (intval >> 10);
		printf("%03o %05o\n", byteval, intval);
	}
	for (i = 0; i < sizeof(boot3)/sizeof(*boot3); i++) {
		printf("%03o\n%03o\n", boot3[i]>>6, boot3[i]&077);
	}
}
