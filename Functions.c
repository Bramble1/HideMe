#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<elf.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "Functions.h"

struct Host host;
struct secret_map map;
char sfile[100];

/*Open and map binary into memory*/
void open_map_file(int *fd,char *filename,char *secret_file)
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
	
	strcpy(sfile,secret_file);
}

/*Save secret file coordinates in struct and write as a roadmap to .Note
 * @offset = address of where secret file is located in binary
 * @length = length of bytes of secret file in binary*/
void write_roadmap()
{
	Elf64_Ehdr *ehdr ; Elf64_Phdr *phdr; Elf64_Shdr *shdr;

	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];

	for(int i=0;i<ehdr->e_phnum;i++)
	{
		if(phdr[i].p_type==PT_NOTE)
		{
		/*	printf("Note segment:\n");
			printf("File offset of segment:%d bytes\n",phdr[i].p_offset);
			printf("Virtual address offset of segment in memory:%d bytes\n",phdr[i].p_vaddr);
			printf("Physical address:%d bytes\n",phdr[i].p_paddr);
			printf("Segment size on hdd:%d\n",phdr[i].p_filesz);
			printf("Segment size in memory:%d\n",phdr[i].p_memsz);
			printf("Segment flags:%d\n",phdr[i].p_flags);
			printf("Segment allignment in memory:%d bytes\n",phdr[i].p_align);
		*/
		
			if(sizeof(struct secret_map)<=phdr[i].p_filesz)
			{
				printf("[+]Secret roadmap struct can fit in .Note\n");
	
				/*Malloc struct*/
				struct secret_map *structure = malloc(sizeof(struct secret_map));
				structure->offset = map.offset;
				structure->size = map.size;
					
				/*memcopy struct to start of note*/
				memcpy((host.host+phdr[i].p_offset),structure,sizeof(struct secret_map));

				/*save offset information*/
		/*		note->offset = phdr[i].p_offset;
				note->size = sizeof(struct secret_map);*/
				
				free(structure);

				printf("[+]Secret roadmap injected into .Note segment\n");
			}
			else
			{
				printf("[-]Secret roadmap struct to small for .NOTE\n");
			}
			
			break;
		}	
	}
}

/*Read structure from .Note*/
void read_roadmap(/*struct secret_map *map*/)
{
	Elf64_Ehdr *ehdr; Elf64_Phdr *phdr;
	
	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];

	struct secret_map *structure = malloc(sizeof(struct secret_map));

	for(int i=0;i<ehdr->e_phnum;i++)
	{
		if(phdr[i].p_type==PT_NOTE)
		{
			memcpy(structure,host.host+phdr[i].p_offset,sizeof(struct secret_map));
			break;
		}
	}

//	printf("structure->offset = %d\nstructure->size = %d\n",structure->offset,structure->size);

	map.offset = structure->offset;
	map.size = structure->size;

//	free(structure);

	printf("[+]Obtained roadmap from .NOTE segment\n");
	
	free(structure);	
}


/*write and read char bytes, string*/
void write_string()
{
	Elf64_Ehdr *ehdr; Elf64_Phdr *phdr;
	
	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];

//	char string[100]="\0";
//	strcpy(string,"SECRET");

	char *string = malloc(sizeof(char)*100);
	strcpy(string,"SECRET MESSAGE!\0");

	int size = strlen(string);

	for(int i=0;i<ehdr->e_phnum;i++)
	{
		if(phdr[i].p_type==PT_NOTE)
		{
			memcpy(host.host+phdr[i].p_offset,string,size);
			break;	
		}
	}

	free(string);	
}

void read_string()
{
	Elf64_Ehdr *ehdr; Elf64_Phdr *phdr;

	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];

	char *string = malloc(sizeof(char)*100);
	
	for(int i=0;i<ehdr->e_phnum;i++)
	{
		if(phdr[i].p_type==PT_NOTE)
		{
			memcpy(string,host.host+phdr[i].p_offset,100);
			break;	
		}
	}

	printf("string = %s\n",string);
	free(string); 

}

void write_secret_file()
{
	Elf64_Ehdr *ehdr; Elf64_Phdr *phdr;

	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];


	/*open key file and mmap to heap*/
	int fd = open(sfile,O_RDONLY);

	struct stat st;
	lstat(sfile,&st);

//	uint8_t *secret_file = (int8_t *)malloc(st.st_size);
	char *secret_file = malloc(sizeof(char)*st.st_size);

	/*read file into allocated memory*/
	read(fd,secret_file,st.st_size);


	int cave_offset,cave_length; cave_offset = cave_length = 0;
	for(int i=0;i<ehdr->e_phnum;i++)
	{
		/*calculate start*/
		cave_offset = phdr[i].p_offset + phdr[i].p_filesz;
		/*next calculate the gap*/
		cave_length = phdr[i+1].p_offset - cave_offset;

		if(cave_length>=st.st_size)
		{
			printf("[+]Found cave to fit secret file\n");
			printf("cave offset = %d and length = %d\n",cave_offset,cave_length);


			/*memcpy secret_file into code cave*/
			memcpy((host.host+cave_offset),secret_file,st.st_size);
			printf("[+]Secret file injected into binary\n");

			map.offset = cave_offset;
			map.size = st.st_size;
		//	map.size = cave_length;	

			break;
		}

	}
	free(secret_file);
}

void read_secret_file()
{
	/*useing map to obtain the location of secret file*/
	read_roadmap();

	/*mmap some memory enough on the heap to get file from binary into allocated memory*/
	char *buf = malloc(sizeof(char)*map.size);

	/*memcpy into buf*/
	memcpy(buf,host.host+map.offset,map.size);
	
	/*write to a temp "key" file*/
	FILE *fd = fopen(sfile,"a+");

	fwrite(buf,map.size,1,fd); 

	fclose(fd);	

}

