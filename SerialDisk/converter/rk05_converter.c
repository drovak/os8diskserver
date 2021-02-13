#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DJG_TO_MAC
#define NUM_IN 4
#define NUM_OUT 3
#endif

#ifdef MAC_TO_DJG
#define NUM_IN 3
#define NUM_OUT 4
#endif

/*
 * convert from Mac to dumprest
 * convert from dumprest to Mac
 */

FILE *input;
FILE *output;

char in_file[256];
char out_file[256];

char in_buf[6];
char out_buf[6];

int c, count_in, count_out = 0;

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		strcpy(in_file, argv[1]);
		strcpy(out_file, argv[2]);
	}
	else
	{
		printf("Usage: %s [input] [output]\n", argv[0]);
		exit(1);
	}
	input = fopen(in_file, "r");
	output = fopen(out_file, "w");
	
	if (input == NULL)
	{
		fprintf(stderr, "On file %s ", in_file);
		perror("open failed");
		exit(1);
	}
	if (output == NULL)
	{
		fprintf(stderr, "On file %s ", out_file);
		perror("open failed");
		exit(1);
	}

	while (!feof(input))
	{
		int num_in = NUM_IN;
		int num_out = NUM_OUT;
		
		if ((c = fread(in_buf, 1, num_in, input)) < 0)
		{
			perror("file read failed");
			exit(1);
		}
		count_in += c;
		
		if (!feof(input) && c != num_in)
		{
			fprintf(stderr, "Failed to read %d bytes! Got %d bytes.\n", num_in, c);
			exit(1);
		}
		else if (feof(input))
			break;

//Mac: ABCD EFGH = aaabbbcc cdddeeef ffggghhh
//DJG: ABCD EFGH = bbcccddd 0000aaab ffggghhh 0000eeef

#ifdef DJG_TO_MAC
		out_buf[0] = ((in_buf[1] << 4) & 0xF0) | ((in_buf[0] >> 4) & 0x0F);
		out_buf[1] = ((in_buf[0] << 4) & 0xF0) | (in_buf[3] & 0xF);
		out_buf[2] = in_buf[2];
#elif defined(MAC_TO_DJG)
		out_buf[0] = ((in_buf[0] << 4) & 0xF0) | ((in_buf[1] >> 4) & 0x0F);
		out_buf[1] = (in_buf[0] >> 4) & 0x0F;
		out_buf[2] = in_buf[2];
		out_buf[3] = in_buf[1] & 0x0F;
#endif
		
		if ((c = fwrite(out_buf, 1, num_out, output)) < 0)
		{
			perror("file write failed");
			exit(1);
		}
		count_out += c;
		if (c != num_out)
		{
			fprintf(stderr, "Failed to write %d bytes! Got %d bytes.\n", num_in, c);
			exit(1);
		}
	}
	printf("Read %d bytes and wrote %d bytes\n", count_in, count_out);
	return 0;
}
