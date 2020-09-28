#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include "Functions.h"
#include<string.h>

/*Try and hide a file into a binary file and then read it again*/
int main(int argc, char **argv)
{
	int fd;
	if(strcmp(argv[1],"i")==0)
	{
		/*binary target & name of encrypted .gpg file*/
		open_map_file(&fd,argv[2],argv[3]);

		write_secret_file();
		write_roadmap();	
	}
        if(strcmp(argv[1],"e")==0)
	{
		/*binary target and name to save extracted .gpg file as*/
		open_map_file(&fd,argv[2],argv[3]);
		
		read_roadmap();
		read_secret_file();

	}
	if(strcmp(argv[1],"h")==0)
	{
		printf("./HideMe <i/e> <target_binary> <file.gpg>\n");
		printf("./HideMe <i>(inject) <target_binary> <input_file.gpg>\n");
		printf("./HideMe <e>(extract) <target_binary> <output_file.gpg>\n");

		exit(EXIT_FAILURE);
	}

	close(fd);

	/*open and map file*/
//	int fd;
//	open_map_file(&fd,argv[1],sfile);


	/*write secret file
	 * then write roadmap*/
//	write_secret_file();
//	write_roadmap();


	/*get roadmap and read secret file*/
//	read_roadmap();
//	read_secret_file();


//	write_roadmap(1376,162);

//	read_roadmap();

	/*write secret key to available code cave
	 * and then call write_roadmap*/
				
	/*Storing
	 * ---------------------
	 * 1. write_secret_file()
	 * 2. write_roadmap()
	 *
	 * Retrieving
	 * ---------------------
	 * 1. read_roadmap()
	 * 2. read_secret_file()
	 * */

		


}
