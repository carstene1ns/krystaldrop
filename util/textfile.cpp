#include "textfile.h"

#include "direct.h"
#include <stdio.h>
#include <memory.h>

KD_TextFile::KD_TextFile() : pos(0)
{

}

KD_TextFile::KD_TextFile(char *fileName) : pos(0)
{
	Load(fileName);
}

KD_TextFile::KD_TextFile(TACCRes *accFile, char *fileName) : pos(0)
{
	Load(accFile, fileName);
}

bool KD_TextFile::Load(char *fileName)
{
	FILE *fpt = fopen(fileName,"rb");
	
	if (!fpt) return false;

	fseek(fpt, 0, SEEK_END);
	size = ftell(fpt);
	fseek(fpt, 0, SEEK_SET);

	text = new char[size+1];

	fread(text, size*sizeof(char), 1, fpt);
	text[size]=0;

	fclose(fpt);
	return true;
}

bool KD_TextFile::Load(TACCRes *accFile, char *fileName)
{
	int idAcc = accFile->EntryId(fileName);
	size = accFile->EntryLength(idAcc);

	char *ptr = (char *)accFile->EntryPtr(idAcc);

	text = new char[size+1];
	memcpy(text,ptr,size);
	text[size]=0;
	return false;
}

KD_TextFile::~KD_TextFile()
{
	delete[] text;
}

char *KD_TextFile::getPosition()
{
	return text+pos;
}

char *KD_TextFile::find(char c)
{
	while(1)
	{
		if (text[pos]==c) break;
		if (isEOF()) break;
		pos++;
	}
	return text+pos;
}

char *KD_TextFile::jumpLine()
{
	find(13);
	pos++;
	if (text[pos]==10) pos++;
	return text+pos;
}

bool KD_TextFile::isEOF()
{
	if (pos>=size) return true;
	else return false;
}
