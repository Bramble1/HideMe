#include "functions.h"

/*Host host;
Map map;
*/
int8_t sfile[200];
int divisor;


void random_positions(const int64_t min,const int64_t max,int64_t range,int64_t *numbers)
{
	int random = 0;
	for(int i=0;i<range;i++)
	{
		random = rand()%(max-min)+min;
		numbers[i] = random;
	}
}

void map_file(int64_t *fd,int8_t *filename,int8_t *secret_file,Host *host)
{
	struct stat st;
	
	if((*fd=open(filename,O_RDWR))<0)
	{
		perror("open():");
		exit(EXIT_FAILURE);
	}

	if(fstat(*fd,&st)<0)
	{
		perror("fstat():");
		exit(EXIT_FAILURE);
	}

	int8_t *mem = mmap(NULL,st.st_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,*fd,0);
	if(mem==MAP_FAILED)
	{
		perror("mmap():");
		exit(EXIT_FAILURE);
	}

	host->host = mem;
	strcpy(sfile,secret_file);

	host->ehdr = (Elf64_Ehdr *)host->host;
	host->phdr = (Elf64_Phdr *)&host->host[host->ehdr->e_phoff];
//	host->shdr = (Elf64_Shdr *)&host->host[host->ehdr->e_shoff];
}

void write_roadmap(Host *host , Map *map)
{
	//Elf64_Ehdr *ehdr; Elf64_Phdr *phdr; Elf64_Shdr *shdr;
	
	//ehdr = (Elf64_Ehdr *)host->host;
	//phdr = (Elf64_Phdr *)&host->host[ehdr->e_phoff];
	//shdr = (Elf64_Shdr *)&host->host[ehdr->e_shoff];

	/*write to the program header table code caves instead
	 * and then see if we are able to write_divide_file into
	 * section header table code caves instead.*/

	/*let's just store in pt_note like we originally did, it works, except for the warning, and in the next prototype
	 * find a better place to hide the struct*/

	for(int i=0;i<host->ehdr->e_phnum;i++)
	{
		if(host->phdr[i].p_type==PT_NOTE)
		{
			printf("FOUND PT_NOTE!\n");

			Elf64_Addr off = host->phdr[i].p_offset + host->phdr[i].p_filesz;
			//int size = host->phdr[i+1].p_offset - off;

			if((int)sizeof(Map)<=host->phdr[i].p_filesz)
			{
				printf("[+]Identified map locaiton and injection.\n");
				for(int z=0;z<divisor;z++)
					printf("map.offset[%d]=%d\n",z,map->offset[z]);
				printf("map.divisor=%d\n",map->divisor);
				printf("map.average=%d\n\n",map->average_size);

				memset((void*)host->host+host->phdr[i].p_offset,0x00,host->phdr[i].p_filesz);
				memcpy(host->host+host->phdr[i].p_offset,map,sizeof(Map));

				//memcpy(host->host+off,map,sizeof(Map));
				break;
			}
			else
			{
				printf("[-]Map is to large!\n");
				exit(EXIT_FAILURE);
			}
		} 
	}
//	return;
	exit(EXIT_SUCCESS);
}


void read_roadmap(Host *host,Map *map)
{
	Map *structure = malloc(sizeof(Map));

	for(int i=0;i<host->ehdr->e_phnum;i++)
	{
		if(host->phdr[i].p_type==PT_NOTE)
		{
		//	Elf64_Addr offset = host->phdr[i].p_offset + host->phdr[i].p_filesz;
			
			memcpy(structure,host->host+host->phdr[i].p_offset,sizeof(Map));
			break;
		}	
	}

	map->divisor = divisor = structure->divisor;
	map->average_size = structure->average_size;

	for(int i=0;i<divisor;i++)
	{
		map->offset[i] = structure->offset[i];
		map->order[i] = structure->order[i];
	}

	printf("[+]Obtained map from Note segment's code cave\n");
	free(structure);

	printf("[*]Read Map information:\n");
	for(int i=0;i<divisor;i++)
		printf("map.offset[%d]=%d bytes\n",i,map->offset[i]);
	printf("map.divisor=%d\n",map->divisor);
	printf("map.average=%d\n",map->average_size);

}

void check_divisor(Host *host ,int64_t div)
{
	struct stat st;
	stat(sfile,&st);
	int average = st.st_size/div;

	Elf64_Ehdr *ehdr; Elf64_Phdr *phdr;
	ehdr = (Elf64_Ehdr *)host->host;
	phdr = (Elf64_Phdr *)&host->host[ehdr->e_phoff];

	int cave_offset,cave_length;
	cave_offset = cave_length = 0;
	int no_limit = divisor; int inc=0;

	for(int i=0;i<ehdr->e_phnum;i++)
	{
		cave_offset = phdr[i].p_offset+phdr[i].p_filesz;
		cave_length = phdr[i+1].p_offset - cave_offset;

		if(cave_length>=average && inc<no_limit)
			inc++;
	}

	if(inc==divisor)
	{
		printf("[+]Enough code caves available.\n");
		//proceed=1;
		divisor = div;
	}
	else
	{
		printf("[-]Not enought code caves available.\n");
		exit(EXIT_FAILURE);
	}
}

int64_t random_value(int64_t size)
{
	int random = rand()%size;
	return random;
}

void write_divide_file(Host *host , Map *map)
{
	int fd = open(sfile,O_RDONLY);
	if(fd==-1)
	{
		perror("write_divide_file()->open():");
		exit(EXIT_FAILURE);
	}

	struct stat st;
	stat(sfile,&st);

	char *secret_file = malloc(sizeof(char)*st.st_size);
	read(fd,secret_file,st.st_size);

	int average = st.st_size/divisor;

	char(*d_matrix)[average+1] = malloc(sizeof(char[divisor][average+1]));
	
	for(int i=0;i<divisor;i++)
	{
		memset(d_matrix[i],'\0',average+1);
		if(i==0)
			memcpy(&d_matrix[i],secret_file,average);
		else
			memcpy(&d_matrix[i],&secret_file[(average*i)],average);
	}

	for(int i=0;i<divisor;i++)
	{
		printf("%s\n",d_matrix[i]);
		printf("________________________\n");
	}
	
	for(int i=0;i<divisor;i++)
		for(int y=0;y<average;y++)
			d_matrix[i][y] += 2;

	
	int offset; int gap; int inc;
	offset = gap = inc = 0;
	int no_limit=divisor;

	for(int i=0;i<host->ehdr->e_phnum;i++)
	{
		offset = host->phdr[i].p_offset + host->phdr[i].p_filesz;
		gap = host->phdr[i+1].p_offset - offset;

		if(average<=gap && inc<no_limit)
		{
			memcpy(host->host+offset,d_matrix[inc],average);
			map->offset[inc]=offset;
			printf("[*]Cave offsets:\n");
			printf("cave offset=%d\n",offset);
			printf("map.offset[%d]=%d\n\n",inc,map->offset[inc]);

			inc++;
		}
	}
	map->average_size = average;
	map->divisor = divisor;
	free(d_matrix);
	write_roadmap(host,map);

	close(fd);
}

void read_divide_file(Host *host , Map *map)
{
	read_roadmap(host,map);

	char matrix[divisor][map->average_size+1];

	for(int i=0;i<divisor;i++)
	{
		memset(matrix[i],'\0',map->average_size+1);
		memcpy(matrix[i],host->host+map->offset[i],map->average_size);
	}

	for(int i=0;i<map->divisor;i++)
		for(int y=0;y<map->average_size;y++)
			matrix[i][y]-=2;

	for(int i=0;i<divisor;i++)
	{
		printf("%s\n",matrix[i]);
		printf("_______________________________\n");
	}

	remove(sfile);
	FILE *fd = fopen(sfile,"a+");
	if(fd==NULL)
	{
		perror("fopen():");
		exit(EXIT_FAILURE);
	}
	for(int i=0;i<divisor;i++)
		fputs(matrix[i],fd);
	fputc('\n',fd);
	fclose(fd);

	exit(EXIT_SUCCESS);
}

void write_randomised_file(Host *host,Map *map)
{
	int fd = open(sfile,O_RDONLY);
	if(fd==-1)
	{
		perror("open:");
		exit(EXIT_FAILURE);
	}

	struct stat st;
	stat(sfile,&st);

	char *secret_file = malloc(sizeof(char)*st.st_size);
	read(fd,secret_file,st.st_size);

	int average = st.st_size/divisor;

	char(*d_matrix)[average+1] = malloc(sizeof(char[divisor][average+1]));
	int random_pos ; int prev_pos;
	random_pos = prev_pos = 0;

	for(int i=0;i<divisor;i++)
	{
		memset(d_matrix[i],'\0',average+1);
		if(i==0)
			memcpy(&d_matrix[i],secret_file,average);
		else
			memcpy(&d_matrix[i],&secret_file[(average*i)],average);
	}
	
	Segment table[divisor];
	int64_t numbers[3];
	srand(time(NULL));

	while(1)
	{
		if(divisor==3)
		{
			random_positions(0,3,3,numbers);
			if(numbers[0]!=numbers[1] && numbers[0]!=numbers[2] && numbers[1]!=numbers[0] && numbers[1]!=numbers[2]
					&& numbers[2]!=numbers[0] && numbers[2]!=numbers[1])
			{
				if(numbers[0]!=0 && numbers[1]!=2 && numbers[2]!=2)
					break;
			}
		}
		else if(divisor==2)
		{
			random_positions(0,2,2,numbers);
			if(numbers[0]!=numbers[1])
			{
				if(numbers[0]!=0 && numbers[1]!=1)
					break;
			}
		}
		
		

		/*this makes sure they are all unique*/
	/*	if(numbers[0]!=numbers[1] && numbers[0]!=numbers[2] && numbers[1]!=numbers[0] && numbers[1]!=numbers[2]
				&& numbers[2]!=numbers[0] && numbers[2]!=numbers[1])
		{
			if(numbers[0]!=0 && numbers[1]!=1 && numbers[2]!=2)
				break;
		} */
			
	}

	for(int i=0;i<divisor;i++)
	{
		memset(table[i].static_segment,'\0',1000);
		table[i].order = i;
		strcpy(table[i].static_segment,d_matrix[i]);
	}

	Segment rtable[divisor];
	int test[divisor]; int cnt=0;
	for(int i=0;i<divisor;i++)
	{
		strcpy(rtable[numbers[i]].static_segment,table[i].static_segment);
		rtable[numbers[i]].order = table[i].order;
		test[cnt++]=rtable[i].order;

		printf("\trtable.order=%d\n",rtable[i].order);
	}

	for(int i=0;i<divisor;i++)
	{
		map->order[i] = rtable[i].order;
		printf("\t\tmap.order=%d\n",map->order[i]);
	}

	for(int i=0;i<divisor;i++)
	{
		printf("%s\n[*]order=%d\n",rtable[i].static_segment,rtable[i].order);
		printf("_____________________________________________________________\n");
	}

	for(int i=0;i<divisor;i++)
		for(int y=0;y<1000;y++)
			rtable[i].static_segment[y] +=2;

	Elf64_Addr offset; int gap; int inc;
	gap = inc = 0;

	for(int i=0;i<host->ehdr->e_phnum;i++)
	{
		offset = host->phdr[i].p_offset + host->phdr[i].p_filesz;
		gap = host->phdr[i+1].p_offset - offset;

		if(average<=gap && inc<divisor)
		{
			memcpy(host->host+offset,&rtable[inc].static_segment,average);
			map->offset[inc]=offset;
			printf("[*]Cave offsets:\n");
			printf("cave offset=%d\n",offset);
			printf("map.offset[%d]=%d\n\n",inc,map->offset[inc]);

			inc++;
						
		}
	}
	map->average_size = average;
	map->divisor = divisor;
	free(d_matrix);
	write_roadmap(host,map);
}

void read_randomised_file(Host *host ,Map *map)
{
	read_roadmap(host,map);
	

	Segment table[map->divisor];

	char matrix[divisor][map->average_size+1];
	for(int i=0;i<divisor;i++)
		memset(matrix[i],'\0',map->average_size+1);

	for(int i=0;i<divisor;i++)
	{
		if(map->order[i]==0)
		{
			printf("\n\t[+]Found segment.1[+]\n");
			memcpy(matrix[0],host->host+map->offset[i],map->average_size);
		}
		if(map->order[i]==1)
		{
			printf("\n\t[+]Found segment.2[+]\n");
			memcpy(matrix[1],host->host+map->offset[i],map->average_size);
		}
		if(map->order[i]==2)
		{
			printf("\n\t[+]Found segment.3[+]\n");
			memcpy(matrix[2],host->host+map->offset[i],map->average_size);
		}
	}

	for(int i=0;i<3;i++)
		for(int y=0;y<map->average_size;y++)
			matrix[i][y]-=2;

	printf("\t[*]OUTPUT FROM READ[*]\n\n");
	for(int i=0;i<divisor;i++)
	{
		printf("%s",matrix[i]);
		printf("\n_________________________________________________\n");
	}

	remove(sfile);
	FILE *fd = fopen(sfile,"a+");
	for(int i=0;i<divisor;i++)
		fputs(matrix[i],fd);
	fputc('\n',fd);
	fclose(fd);

	exit(EXIT_SUCCESS);

}

void verify_pgp(char *filename)
{
	int length = strlen(filename);

	if(filename[length-1]=='p' && filename[length-2]=='g' && filename[length-3]=='p' && filename[length-4]=='.')
	{
		printf("[+]valid pgp extension\n");
	}
	else
	{
		printf("[-]Invalid pgp extesion\n");
		exit(EXIT_FAILURE);
	}
}

