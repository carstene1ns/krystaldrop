#ifndef LogFile_H
#define LogFile_H

#include <stdio.h>
#include <stdarg.h>

#include "../Tools/defines.h"

/**
	static class used to create logfiles.
  */
class DllExport KD_LogFile
{
	/**
		Singleton instanciated before the call to main()!
	*/
	static KD_LogFile singleton;

	KD_LogFile(char *name);

	~KD_LogFile();

	/**
		The pointer to the logfile
	*/
#ifndef NDEBUG
	FILE *fpt;
#endif
public:
	
	/**
		Prints something to the logfile. The syntax is the same as printf.
	*/
	static void printf(char *str, ...);

	/**
		Prints something to the logfile and to the stdio. The syntax is the same as printf.
	*/
	static void printf2(char *str, ...);
	
};

#endif
