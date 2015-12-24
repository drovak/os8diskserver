//Installs the bootloader and handler from an assembled .bin OS/8 system handler
//Usage: ./handler_installer [handler.bin] [disk_image.rk05]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *handler;
FILE *disk;

unsigned char handler_buf[1024];
int handler_file_length;
unsigned char disk_buf[454];

int i;
int count;
int start_btldr;
int start_handler;

void bin_to_djg(char* buf_in, char* buf_out, int pos_in, int pos_out, int count);
int decode_word(char* buf, int pos);
int write_to_file(FILE* file, int offset, char* buf, int length);
int read_from_file(FILE* file, int offset, char* buf, int length);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s [handler] [disk]\n", argv[0]);
		exit(1);
	}
	
	handler = fopen(argv[1], "r");
	disk = fopen(argv[2], "r+");
	
	if (handler == NULL)
	{
		fprintf(stderr, "On file %s ", argv[1]);
		perror("open failed");
		exit(1);
	}
	if (disk == NULL)
	{
		fprintf(stderr, "On file %s ", argv[2]);
		perror("open failed");
		exit(1);
	}

	printf("Read %d bytes of disk\n", read_from_file(disk, 0, disk_buf, sizeof(disk_buf)));
	printf("Read %d bytes of handler\n", (handler_file_length = read_from_file(handler, 0, handler_buf, sizeof(handler_buf))));
	/*for (i = 0; i < handler_file_length; i += 2)
	{
		printf("%04o\n", decode_word(handler_buf, i));
	}
	for (i = 0; i < handler_file_length; i++)
	{
		printf("%03o\n", (unsigned char) handler_buf[i]);
	}*/
	i = 0;
	while (handler_buf[i] == 0200)
		i++;
	printf("Position after leader: %04o\n", i);
	i += 4; //skip past origin
	count = -decode_word(handler_buf, i) & 07777;
	printf("Number of devices: %d\n", count);
	count *= 16; //count number of device entries
	i += (4 + count); //skip past device entries, count, and size of bootloader
	start_btldr = i;
	printf("Position of bootloader: %04o\n", start_btldr);
	
	for (i; ((handler_buf[i] != 0102) && (i < sizeof(handler_buf))); i++); //get to start of handler
	i += 2;
	i += 14; //skip past zeros
	start_handler = i;
	printf("Position of handler: %04o\n", start_handler);
	
	bin_to_djg(handler_buf, disk_buf, start_btldr, 0, 39);
	bin_to_djg(handler_buf, disk_buf, start_handler, 0x10E, 92);
	
	write_to_file(disk, 0, disk_buf, sizeof(disk_buf));
	return 0;
}


int read_from_file(FILE* file, int offset, char* buf, int length)
{
	int c;
	fseek(file, offset, SEEK_SET);
	if ((c = fread(buf, 1, length, file)) < 0)
	{
		perror("File read failure");
		exit(1);
	}
	return c;
}

int write_to_file(FILE* file, int offset, char* buf, int length)
{
	int c;
	fseek(file, offset, SEEK_SET);
	if ((c = fwrite(buf, 1, length, file)) < 0)
	{
		perror("File write failure");
		exit(1);
	}
	else if (c != length)
	{
		//fprintf(stderr, MAKE_RED "Warning: failed to complete write!\n" RESET_COLOR);
		return -1;
	}
	else
	{
		fflush(file);
		return c;
	}
}

int decode_word(char* buf, int pos)
{
	return (((buf[pos] & 077) << 6) | (buf[pos + 1] & 077));
}

void bin_to_djg(char* buf_in, char* buf_out, int pos_in, int pos_out, int count)
{
	int i;
	for (i = 0; i < count; i++)
	{
		buf_out[pos_out + 1] = (buf_in[pos_in] >> 2) & 0x0F; //00aaabbb -> 0000aaab
		buf_out[pos_out] = ((buf_in[pos_in] << 6) & 0300) | (buf_in[pos_in + 1] & 077); //00aaabbb 00cccddd -> bbcccddd
		pos_in += 2;
		pos_out += 2;
	}
}
