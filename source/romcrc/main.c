/*
	This tool open the rom path given in command line and gaves the crc and country code from N64 Header.
	It will return "default" with an error level if file is not a valid N64 ROM.
*/

#include <stdio.h>
//#include <file.h>

char rom_header[0x40];	// rom header
int 	swapped;	// 0 = no, 1 = byte, 2 = word

int main(int argc, char** argv)
{
	if (argc!=2) {
		printf("romcrc v1.0\n Give a crc of N64 rom by reading it's header\nromcrc \"romfile.xxx\" to have crc in the form of\nXXXX-YYYY-CC\n z64, v64 and n64 supported\n");
		return 0;
	}
	FILE *f;
	f = fopen(argv[1], "rb");
	if (!f) {
		fprintf(stderr, "Unable to open file %s\n", argv[1]);
		printf("default\n");
		return -1;
	}
	fread(rom_header, 0x40, 1, f);
	fclose(f);
	
	char temp;
	
	int i;
	
	// now, check header to unswap things...
    /* Btyeswap if .v64 image. */
    if(rom_header[0]==0x37)
    {
        for (i = 0; i < 0x40; i+=2)
        {
            temp=rom_header[i];
            rom_header[i]=rom_header[i+1];
            rom_header[i+1]=temp;
        }
    }
    /* Wordswap if .n64 image. */
    else if(rom_header[0]==0x40)
    {
        for (i = 0; i < 0x40; i+=4)
        {
            temp=rom_header[i];
            rom_header[i]=rom_header[i+3];
            rom_header[i+3]=temp;
            temp=rom_header[i+1];
            rom_header[i+1]=rom_header[i+2];
            rom_header[i+2]=temp;
        }
    }
	// check it's a Valid rom
	if ((rom_header[0]!=0x80)||(rom_header[1]!=0x37)||(rom_header[2]!=0x12)||(rom_header[3]!=0x40)) {
		fprintf(stderr, "Unable to open file %s\n", argv[1]);
		printf("default\n");
		return -2;
	}
	

/*	printf("%02x%02x%02x%02x-%02x%02x%02x%02x-%02x\n", 
		rom_header[0x13], rom_header[0x12], rom_header[0x11], rom_header[0x10],
		rom_header[0x17], rom_header[0x16], rom_header[0x15], rom_header[0x14],
		rom_header[0x20] );*/
	printf("%02x%02x%02x%02x%02x%02x%02x%02x\n", 
		rom_header[0x13], rom_header[0x12], rom_header[0x11], rom_header[0x10],
		rom_header[0x17], rom_header[0x16], rom_header[0x15], rom_header[0x14] );

	
	return 0;
}
