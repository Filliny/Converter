#include <iostream>
#include <stdio.h>
#include <locale.h>

#define CONV = 10

#include "Header.h"

int main(int argc, char* argv[])
{
	FILE* fp = NULL; //input file pointer


	const char* filename = argv[1]; //first cmd line argument taken
	const char* opt = argv[2]; //second cmd line argument taken
	int tc = NULL ;
	if (argc == 4)
	{
		tc = atoi(argv[3]);
	}

	//command line args  process
	if (filename == NULL && argc == 1)
	{
		std::cout << "No arguments! type -h for help\n"; //if no arguments given
		return 0;
	}
	else if (!strcmp(filename, "-h"))
	{
		std::cout << "this is help\ntype <file name.tcx> + <gpx> to convert tcx file\n example converter.exe test.tcx gpx \n"; //need some help strings add)
		std::cout << "type <file name.tcx> + <-t> <2> to correct time to 2 hr\n example converter.exe test.tcx -t 2 \n"; //need some help strings add)

		return 0;

	}
	else if (filename != NULL && opt == NULL)
	{

		printf("What u want to do with %s ? \n Specify option ( see help -h)", filename);
		return 0;
	}
	else if (filename != NULL && !strcmp(opt, "gpx"))
	{
		std::cout << "Gpx convert started\n"; //making conversion
		gpxconv(filename, fp);

	}
	else if (filename != NULL && !strcmp(opt, "-t") && tc != NULL)
	{
		std::cout << "Time correction started\n"; //making conversion
		time(fp, filename, tc);
	}
}


