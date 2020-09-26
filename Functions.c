#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<elf.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "functions.h"

struct Payload file;
struct Host host;

struct secret_file
{
	int64_t offset_address;
	int64_t size;
};

/*Open and map binary into memory*/
void open_map_file(int *fd,char *filename)
{
	struct stat st;

	if((*fd = open(filename,O_RDWR))<0)
	{
		perror("open():");
		exit(EXIT_FAILURE);
	}

	if(fstat(*fd,&st)<0)
	{
		perror("fstat():");
		exit(EXIT_FAILURE);
	}

	uint8_t *mem = mmap(NULL,st.st_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,*fd,0);

	if(mem==MAP_FAILED)
	{
		perror("mmap:");
		exit(EXIT_FAILURE);
	}
	
	host.host = mem;
}
