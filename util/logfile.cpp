#include "logfile.h"

#include <time.h>

FILE *KD_LogFile::fpt = 0;

void KD_LogFile::Init(char *name)
{
	fpt = fopen(name,"w");

	time_t aclock;
	struct tm *newtime;

	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  /* Convert time to struct */
									/* tm form */


	KD_LogFile::printf("Krystal Drop logfile\n");
	KD_LogFile::printf("Generated %s\n",asctime( newtime ));
}

void KD_LogFile::printf(char *str, ...)
{
	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	va_end (argptr);

}

void KD_LogFile::Close()
{
	fclose(fpt);
}
