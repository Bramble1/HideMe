#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<elf.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>


/*Used for pointing to the mapped binary in memory*/
struct Host
{
	uint8_t *host;
};

struct Note_information
{
	Elf64_Addr offset;
	int64_t size;
};

/*used as a guide to where the secret file is located in the binary,
 * this will be embedded into the .NOTE section for easy reference*/
struct secret_map
{
	Elf64_Addr offset;
	int64_t size;
};


void open_map_file(int *fd,char *filename);
void write_to_note();
