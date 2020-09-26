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

	struct Note_information note;

	//write_to_note(&note);
	
//	read_from_note(&note);

	write_string();

	read_string();




		


}
