#include "Textfile.h"

//#include "direct.h"
#include "Logfile.h"
#include <stdio.h>
#include <memory.h>
#include <assert.h>

KD_TextFile::KD_TextFile() : pos(0)
{

}

KD_TextFile::KD_TextFile(const KD_FilePath &fileName) : pos(0)
{
	Load(fileName);
}

/*KD_TextFile::KD_TextFile(TACCRes *accFile, char *fileName) : pos(0)
{
	Load(accFile, fileName);
}*/

bool KD_TextFile::Load(const KD_FilePath &fileName)
{
	if (!fileName.IsArchived())
	{
		FILE *fpt = fopen(fileName.GetPath().c_str(),"rb");
		
		if (!fpt) 
		{
			KD_LogFile::printf2("File %s not found!",fileName.GetFullPath().c_str());
			assert(fpt);
			return false;
		}

		fseek(fpt, 0, SEEK_END);
		size = ftell(fpt);
		fseek(fpt, 0, SEEK_SET);

		text = new char[size+1];

		fread(text, size*sizeof(char), 1, fpt);
		text[size]=0;

		fclose(fpt);
		return true;
	}
	return false;
}

/*bool KD_TextFile::Load(TACCRes *accFile, char *fileName)
{
	if (!accFile)
		return Load(fileName);
	
	int idAcc = accFile->EntryId(fileName);

	if (idAcc<0)
	{
		switch (idAcc)
		{
		case ACC_ENTRYNOTFOUND:
			printf("File %s not found in ACC file %s\n", fileName, accFile->CurrentFile);
			KD_LogFile::printf("File %s not found in ACC file %s\n", fileName, accFile->CurrentFile);
			assert(0);
			return false;
		case ACC_NOTINITIALIZED:
			printf("File %s not found: ACC File not properly Initialized.\n", fileName);
			KD_LogFile::printf("File %s not found: ACC File not properly Initialized.\n",fileName);
			assert(0);
			return false;
		default:
			printf("Unknown error in ACC File. Aborting.\n");
			KD_LogFile::printf("Unknown error in ACC File. Aborting.\n");
			assert(0);
			return false;
		}
	}

	size = accFile->EntryLength(idAcc);

	char *ptr = (char *)accFile->EntryPtr(idAcc);

	text = new char[size+1];
	memcpy(text,ptr,size);
	text[size]=0;
	return false;
}*/

KD_TextFile::~KD_TextFile()
{
	delete[] text;
}

char *KD_TextFile::GetPosition()
{
	return text+pos;
}

char *KD_TextFile::ReadNewLine()
{
	while(1)
	{   if (text[pos]== 13 || text[pos]== 10) break;
		if (IsEOF()) break;
		pos++;
	}
    
    while( (!IsEOF()) && (text[pos]== 13 || text[pos]== 10) ) pos++;

	return text+pos;
}

/**
	Goes to the next valid line (a line containing something else than ' '.
  */
char *KD_TextFile::JumpLine()
{
	ReadNewLine();
  
/*	pos++;*/
  /*  if (IsEOF()) return text+ pos;
	if (text[pos]==10) pos++;
    if (IsEOF()) return text+ pos;
*/
	while (1)
	{
		int newpos = pos;

		// If space
		if (text[newpos]==' ')
			newpos++;
		// If new line: go to new line and return;
		else if (text[newpos]== 13 || text[newpos]== 10)
		{
			newpos++;
			if (text[newpos]== 13 || text[newpos]== 10) newpos++;
			pos = newpos;
		}
		// If end of file.
		else if (IsEOF()) break;
		// if anything else
		else break;
	}

	return text+pos;
}

bool KD_TextFile::IsEOF()
{
	if (pos>=size) return true;
	else return false;
}

string KD_TextFile::GetString()
{
	string str="";

	while(1)
	{
		// If ok
		if (text[pos]=='"') break;
		// If new line: go to new line and return;
		if (text[pos]==13)
		{
			pos++;
			if (text[pos]==10) pos++;
			return "";
		}
		// If end of file.
		if (IsEOF()) return "";

		if (text[pos]!=' ' && text[pos]!='\t')
			break;

		pos++;
	}

	if (IsEOF()) return "";

	if (text[pos]=='"')
	{
		pos++;

		while(1)
		{
			// If ok
			if (text[pos]=='"') break;
			// If new line: go to new line and return;
			if (text[pos]==13)
			{
				pos++;
				if (text[pos]==10) pos++;
				return "";
			}
			// If end of file.
			if (IsEOF()) return "";

			str += text[pos];

			pos++;
		}

		pos++;
	}
	else
	{
		while(1)
		{
			if (text[pos]==' ' || text[pos]=='\t') 
				break;
			// If new line: go to new line and return;
			if (text[pos]==13)
			{
				if (text[pos+1]==10) pos++;
				break;
			}
			// If end of file.
			if (IsEOF()) break;

			str += text[pos];

			pos++;
		}

		pos++;
	}

	return str;
}
