#ifndef LogFile_H
#define LogFile_H

#include <stdio.h>
#include <stdarg.h>

/**
	static class used to create logfiles.
  */
class KD_LogFile
{
	/**
		The pointer to the logfile
	*/
	static FILE *fpt;
public:
	/**
		Opens the logfile named "name" in write only.
	*/
	static void Init(char *name);
	/**
		Prints something to the logfile. The syntax is the same as printf.
	*/
	static void printf(char *str, ...);
	/**
		Closes the logfile.
	*/
	static void Close();
};

#endif