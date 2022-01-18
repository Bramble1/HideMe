#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<elf.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>

typedef struct
{
	uint8_t *host;
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
}Host;

typedef struct
{
	Elf64_Addr offset;
	int64_t size;
}Note_info;

typedef struct
{
	uint8_t static_segment[1000];
	int order;
}Segment;

/*typedef struct
{
	Elf64_Addr offset[4]; //used to be 3
	int8_t order[3];
	int64_t average_size;
	int8_t divisor;
}Map; */

typedef struct __attribute__((packed))
{
	Elf64_Addr offset[3];
	int8_t order[3];
	int64_t average_size;
	int8_t divisor;
}Map;


/*FUNCTIONS*/
void random_positions(const int64_t min,const int64_t max,const int64_t range,int64_t *numbers);
//void map_file(int64_t *fd,int8_t *filename,int8_t *secret_file,Host *host);
void map_file(int64_t *fd,int8_t *filename,int8_t *secret_file,Host *host);
void write_roadmap(Host *host,Map *map);
void read_roadmap(Host *host,Map *map);
void check_divisor(Host *host,int64_t div);
int64_t random_value(int64_t size);
void write_divide_file(Host *host,Map *map);
void read_divide_file(Host *host,Map *map);
void write_randomised_file(Host *host,Map *map);
void read_randomised_file(Host *host,Map *map);

void verify_pgp(char *filename);

