#include <iostream>
#include <stdio.h>
#include <locale.h>

#define CONV = 10

#include "Header.h"

int main(int argc, char* argv[])
{
	FILE* fp = NULL; //input file pointer
	;
	//filename = "test.tcx";
	const char *filename = argv[1]; //first cmd line argument taken
	const char *opt = argv[2]; //first cmd line argument taken

	//command line args  process
	if (filename == NULL && argc == 1)
	{
		std::cout << "No arguments! type -h for help\n"; //if no arguments given
		return 0;
	}
	else if (!strcmp(filename, "-h"))
	{
		std::cout << "this is help\n type <file name.tcx> + <gpx> to convert tcx file\n example converter.exe test.tcx gpx "; //need some help strings add)
		return 0;

	}
	else if (filename != NULL && opt == NULL)
	{
		
		printf("What u want to do with %s ? \n Specify option ( see help -h)", filename );
		return 0;
	}
	else if (filename != NULL && !strcmp(opt, "gpx"))
	{
		std::cout << "Gpx convert started\n"; //making conversion
		gpxconv(filename, fp);
		//time(filename, 2);
	}

}


