#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



char *defoutfilename(const char*,char*);
void mkgpxheader(const char*, char*);
void mkgpxfooter(const char*);
void getstarttime(FILE*, const char*, char*);
void parsetcxtogpx(FILE*, const char*, char*);

void timecorr(const char* , int );


void gpxconverter(const char* filename, FILE* fp)
{


	int size = sizeof(filename);
	char* outfile = (char*)malloc(sizeof(char)*100); // take p to outfile name
	char* starttime = (char*)malloc(sizeof(char) * 38); //start time line

	if (outfile != NULL)
	{
		int t = 2;
		timecorr(filename, t);

		getstarttime(fp, filename, starttime);
		defoutfilename(filename, outfile); //generate output name
		mkgpxheader(outfile, starttime); //generate header

		parsetcxtogpx(fp, filename, outfile); //do main work

		mkgpxfooter(outfile);
	}
	else {
		printf("File name error!"); //if maybe... )
		exit(0);
	}
		
	free(outfile);
	free(starttime);
	printf("\nFile converted !\n");
	
}

char* defoutfilename(const char *filename, char *outfile) //function to generate output filename by adding "converted"
{
	    char* soutfile = outfile;

		while (*filename != '.') //remove file extension
		{
			*soutfile = *filename;
			filename++;
			soutfile++;
		}

		*soutfile = '\0'; //terminate string before concatenation
		strcat_s(outfile, 100, " converted.gpx");

		return outfile;
	
}

void mkgpxheader(const char* outfile, char *starttime)
{
	FILE* fwp = NULL;

	if (int err = fopen_s(&fwp, outfile, "w") == NULL && starttime != NULL)
	{
		printf("Generate footer....");

		fwrite("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", 39, 1, fwp);
		fwrite("<gpx xmlns=\"http:\/\/www.topografix.com\/GPX\/1\/1\" version=\"1.1\" creator=\"GPX Convertor\">\n", 86, 1, fwp);
		fwrite("\t<metadata>\n", 12, 1, fwp);
		fwrite("\t\t", 2, 1, fwp);
		fwrite(starttime, strlen(starttime), 1, fwp);
		fwrite("\n\t</metadata>\n", 14, 1, fwp);
		fwrite("\t<trk>\n", 7, 1, fwp);
		fwrite("\t\t<trkseg>\n", 11, 1, fwp);
		
	}
	else
	{
		printf("Cant create file. Error = %d\n startttime defined =%s", err, *starttime);

	}
	
	fclose(fwp);
}

void getstarttime( FILE *fp, const char *filename, char *starttime )
{
	char *pstarttime = starttime;
	char Tm[] = "<Time>";
	char tm[] = "<time>";
	char* start = NULL;
	char buff[100];

	if (!fopen_s(&fp, filename, "r"))
	{
		printf("File for time check openned!\n");
		while (!feof(fp))   //cycle thru file
		{
			if (fgets(buff, 100, fp)) //if line taken
			{
				
				if ((start = strstr(buff, tm)) != NULL || (start = strstr(buff, Tm)) != NULL) //search first time entry
				{

					//printf("Found %s\n", buff);

					while (*start != '\n')
					{
						*pstarttime = tolower(*start); 
						pstarttime++;
						start++;
					}
					*pstarttime = '\0';
				
					return ;
					
				}
					
			}
		}
	}
}

void parsetcxtogpx(FILE* fp, const char* filename, char* outfile)
{
	FILE* fwp = NULL;  //wrtite file pointer

	struct Coordinates
	{
		char *lat;
		char *lngt;
		char* timestmp;

	}  *pcoord = (struct Coordinates*)malloc(sizeof(struct Coordinates));


	
	char Tm[] = "<Time>";
	char tm[] = "<time>";
	char lat[] = "<LatitudeDegrees>";
	char lon[] = "<LongitudeDegrees>";
	int havedata = 0;
	
	char* start = NULL;
	char buff[100];

	if (!fopen_s(&fp, filename, "r") && !fopen_s(&fwp, outfile, "a") )
	{
		printf("Parsing File....\n");
		while (!feof(fp))   //cycle thru file
		{
			if (fgets(buff, 100, fp)) //if line taken
			{
				char* ptime = (char*)malloc(sizeof(char) * 38);
				char* pdat = (char*)malloc(sizeof(char) * 38);

				if ((start = strstr(buff, tm)) != NULL || (start = strstr(buff, Tm)) != NULL) //search first time entry
				{
					//printf("time %s\n", buff);

					
					char* sptime = ptime;

					while (*start != '\n')
					{
						*sptime = tolower(*start);
						sptime++;
						start++;
					}
					*sptime = '\0';

					pcoord->timestmp = ptime;
					
					char* start = NULL;
					//return;
					

				}
				else if ((start = strstr(buff, lat)) != NULL ) //search latitude entry
				{
					//printf("lat %s\n", buff);
					start = start + 17;

					
					char* spdat = pdat;

					while (*start != '<')
					{
						*spdat = *start;
						spdat++;
						start++;
					}
					*spdat = '\0';

					pcoord->lat = pdat;

					char* start = NULL;
					
				}
				else if ((start = strstr(buff, lon)) != NULL) //search latitude entry
				{
					//printf("long %s\n", buff);
					start = start + 18;

					//char* pdat = (char*)malloc(sizeof(char) * 38);
					char* spdat = pdat;

					while (*start != '<')
					{
						*spdat = *start;
						spdat++;
						start++;
					}
					*spdat = '\0';

					pcoord->lngt = pdat;

					char* start = NULL;
					
					havedata = 1;

				}


				// write from structure to file
				

				if (havedata == 1  )
				{
					char buffer[20];
					
					fwrite("\t\t\t<trkpt lat=\"", 15, 1, fwp);
					fwrite(pcoord->lat, strlen(pcoord->lat), 1, fwp);
					fwrite("\" lon=\"", 7, 1, fwp);
					fwrite(pcoord->lngt, strlen(pcoord->lngt), 1, fwp);
					fwrite("\">\n\t\t\t\t", 7, 1, fwp);
					fwrite(pcoord->timestmp, strlen(pcoord->timestmp), 1, fwp);
					fwrite("\n\t\t\t</trkpt>\n", 13, 1, fwp);
					
					havedata = 0;
					free(ptime);
					free(pdat);
				}
				


			}
		}
		fclose(fp);
		fclose(fwp);
		free(pcoord);

	}

}

void mkgpxfooter(const char* outfile)
{
	FILE* fwp = NULL;
	if (int err = fopen_s(&fwp, outfile, "a") == NULL )
	{
		fwrite("\t\t</trkseg>\n", 12, 1, fwp);
		fwrite("\t</trk>\n", 8, 1, fwp);
		fwrite("</gpx>", 6, 1, fwp);
	}
	else
	{
		printf("Cant write footer. Error = %d\n ", err);

	}

	fclose(fwp);
}

void timecorr(const char* filename, int tc)
{
	
	char pref[] = "corrected_";
	int n = ( (strlen(filename)+10));
	char* nfilename = (char*)malloc((strlen(filename) + 10));
	strcpy_s(nfilename,11, pref);
	strcat_s(nfilename, 100, filename);
	//char* pref = "corrected_";


	int c = 10;
	//char* nfilename = filename;//(char*)malloc(sizeof(char)*(strlen(filename) + 4 ))

	//strcat_s(pref, (strlen(filename)+ 11), filename);
	



}