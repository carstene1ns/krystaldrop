#include "Logfile.h"

#include <time.h>

KD_LogFile KD_LogFile::singleton("log_file.txt");

KD_LogFile::KD_LogFile(char *name)
{
#ifndef NDEBUG
	fpt = fopen(name,"w");

	time_t aclock;
	struct tm *newtime;

	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  /* Convert time to struct */
									 /* tm form */

	KD_LogFile::printf("KD++ logfile\n");
	KD_LogFile::printf("Generated %s\n",asctime( newtime ));
#endif
}

KD_LogFile::~KD_LogFile()
{
#ifndef NDEBUG
	fclose(fpt);
#endif
}

void KD_LogFile::printf(char *str, ...)
{
#ifndef NDEBUG
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	va_end (argptr);
#endif
}

void KD_LogFile::printf(const char *str, ...)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	va_end (argptr);
}

/*void KD_LogFile::printf(const string &str, va_list argptr)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	vfprintf (fpt, str.c_str(), argptr);
}*/

void KD_LogFile::printf(const string &str, ...)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str.c_str(), argptr);
	va_end (argptr);
}


void KD_LogFile::printf2(char *str, ...)
{
#ifndef NDEBUG
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	vprintf (str, argptr);
	va_end (argptr);
#endif
}

void KD_LogFile::printf2(const char *str, ...)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str, argptr);
	vprintf (str, argptr);
	va_end (argptr);
}

void KD_LogFile::printf2(const string &str, ...)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	va_list argptr;

	va_start (argptr, str);
	vfprintf (fpt, str.c_str(), argptr);
	vprintf (str.c_str(), argptr);
	va_end (argptr);
}

/*void KD_LogFile::printf2(const string &str, va_list argptr)
{
	FILE *fpt = KD_LogFile::singleton.fpt;

	vfprintf (fpt, str.c_str(), argptr);
	vprintf (str.c_str(), argptr);
}*/
