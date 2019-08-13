#ifndef gpxconv_h
#define gpxconv_h

#include <stdio.h>


void gpxconverter(const char *filename, FILE*);

void timecorr(FILE*,const char* , int );


#endif

#ifdef CONV

#define gpxconv gpxconverter

#define time timecorr

#endif
