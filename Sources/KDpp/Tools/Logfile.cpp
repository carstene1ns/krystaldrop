#include "Logfile.h"

#include <time.h>

KD_LogFile KD_LogFile::singleton("log_file.txt");

KD_LogFile::KD_LogFile(char *name)
{
	fpt = fopen(name,"w");

	time_t aclock;
	struct tm *newtime;

	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  /* Convert time to struct */
									 /* tm form */

	KD_LogFile::printf("KD++ logfile\n");
	KD_LogFile::printf("Generated %s\n",asctime( newtime ));
}

KD_LogFile::~KD_LogFile()
{
	fclose(fpt);
}

void KD_LogFile::printf(char *str, ...)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	va_end (argptr);

}

void KD_LogFile::printf2(char *str, ...)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	vprintf (str, argptr);
	va_end (argptr);
}
