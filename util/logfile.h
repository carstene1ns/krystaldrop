#ifndef LogFile_H
#define LogFile_H

#include <stdio.h>
#include <stdarg.h>

class KD_LogFile
{
	static FILE *fpt;
public:
	static void Init(char *name);
	static void printf(char *str, ...);
	static void Close();
};

#endif