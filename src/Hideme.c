#include "functions/functions.h"

int main(int argc,char **argv)
{
	if(argc==1)
	{
		printf("[?]write/read to binary:\n");
		printf("\t./Hideme <w> <divisor[1-3] > <Target_Binary> <file.pgp>\n");
		printf("\t./HideMe <r> <Target_binary> <extracted.pgp>\n");
		
		printf("__________________________________\n");

		printf("[?]randomised write/read to binary:\n");
		printf("\t./Hideme <rw> <divisor[2-3] > <Target_Binary> <file.pgp>\n");
		printf("\t./Hideme <rr> <Target_Binary> <extracted.pgp>\n\n");

		exit(EXIT_SUCCESS);
	}

	Host host; Map map; int divisor; int8_t sfile[200];

	if(strcmp(argv[1],"w")==0)
	{
		if(argc==5)
		{
			char text[2]="\0";
			strcpy(text,argv[2]);
			divisor = (int)text[0]-'0';
			
			if(divisor<1 || divisor>3)
			{
				printf("[!]Divisor range for this option:1-3\n");
				exit(EXIT_FAILURE);
			}

		

			int64_t fd;
			map_file(&fd,argv[3],argv[4],&host);
			
			check_divisor(&host,divisor);
			verify_pgp(argv[4]);

			write_divide_file(&host,&map);

			close(fd);
			
		}
	}
	if(strcmp(argv[1],"r")==0)
	{
		if(argc==4)
		{
			int64_t fd;
			map_file(&fd,argv[2],argv[3],&host);
			verify_pgp(argv[3]);

			read_divide_file(&host,&map);
			
			close(fd);
		}
	}

	if(strcmp(argv[1],"rw")==0)
	{
		if(argc==5)
		{
			char text[2]="\0";
			strcpy(text,argv[2]);
			divisor = (int)text[0]-'0';

			if(divisor<=1 || divisor >3)
			{
				printf("[!]Divisor range for this option:2-3\n");
				exit(EXIT_FAILURE);
			}

			int64_t fd;
			map_file(&fd,argv[3],argv[4],&host);
			
			check_divisor(&host,divisor);
			verify_pgp(argv[4]);

			
			write_randomised_file(&host,&map);
			
			close(fd);

		}
	}
	if(strcmp(argv[1],"rr")==0)
	{
		if(argc==4)
		{
			int64_t fd;

			map_file(&fd,argv[2],argv[3],&host);
			verify_pgp(argv[3]);
			
			read_randomised_file(&host,&map);

			close(fd);

		}
	}
	
	return 0;
}	
