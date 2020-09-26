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

/*Write structure to .Note*/
void write_to_note(struct Note_information *note)
{
	Elf64_Ehdr *ehdr ; Elf64_Phdr *phdr; Elf64_Shdr *shdr;

	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];

	for(int i=0;i<ehdr->e_phnum;i++)
	{
		if(phdr[i].p_type==PT_NOTE)
		{
			printf("Note segment:\n");
			printf("File offset of segment:%d bytes\n",phdr[i].p_offset);
			printf("Virtual address offset of segment in memory:%d bytes\n",phdr[i].p_vaddr);
			printf("Physical address:%d bytes\n",phdr[i].p_paddr);
			printf("Segment size on hdd:%d\n",phdr[i].p_filesz);
			printf("Segment size in memory:%d\n",phdr[i].p_memsz);
			printf("Segment flags:%d\n",phdr[i].p_flags);
			printf("Segment allignment in memory:%d bytes\n",phdr[i].p_align);


		//		printf("[+]secret map(%d bytes) structure can fit inside note (%d bytes)\n",sizeof(*map),phdr[i].p_filesz);
				/*Malloc struct*/
				struct secret_map *structure = malloc(sizeof(struct secret_map));
				structure->offset = 1000;
				structure->size = 30;
					
				/*memcopy struct to start of note*/
				memcpy((host.host+phdr[i].p_offset),structure,sizeof(struct secret_map));

				/*save offset information*/
				note->offset = phdr[i].p_offset;
				note->size = sizeof(struct secret_map);
				
				free(structure);
			
			
			break;
		}	
	}
}

/*Read structure from .Note*/
void read_from_note(struct Note_information *note)
{
	Elf64_Ehdr *ehdr; Elf64_Phdr *phdr;
	
	ehdr = (Elf64_Ehdr *)host.host;
	phdr = (Elf64_Phdr *)&host.host[ehdr->e_phoff];

	struct secret_map *structure = malloc(sizeof(struct secret_map));

	memcpy(structure,host.host+note->offset,sizeof(struct secret_map));

	printf("structure->offset = %d\nstructure->size = %d\n",structure->offset,structure->size);
		
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

	for(int i=0;i<ehdr->e_phnum;i++)
	{
		if(phdr[i].p_type==PT_NOTE)
		{
			memcpy(host.host+phdr[i].p_offset,string,sizeof(string));
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
			memcpy(string,host.host+phdr[i].p_offset,sizeof(string));
			break;	
		}
	}

	printf("string = %s\n",string);
	free(string); 

}


