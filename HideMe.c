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

/*Try and hide a file into a binary file and then read it again*/
int main(int argc, char **argv)
{

	/*open and map file*/
	int fd;
	open_map_file(&fd,argv[1]);

	write_roadmap(1376,162);

	read_roadmap();

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
