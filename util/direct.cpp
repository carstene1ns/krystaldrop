// Direct v2.2 for C

// TACCRes: lecteur de fichier ACC
// TACCEditMem: TACCRes+ ajout, suppression, extraction en mémoire vive
// TACCIndex: lecture directement sur disque

/* should be rewritten completely to
   allow long file names
   get rid of the bugs !!!!
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "direct.h"

#ifndef min
#define min(a,b) ( (a<b) ? a : b )
#endif

#define TAEMBUFSIZE 1024

char PEmpty= 0;


// types TEntry, PEntry, TEntryTable, PEntryTable définis dans direct.h
// classes TACCRes et TACCEditMem définies dans direct.h


/*  *****************************************************************  */

signed char ChangeMemSize (void* &which, long old_size, long new_size)
{ void* p;

  if ( (p= malloc(new_size))== NULL) return ACC_NOTENOUGHMEMORY;
  if (which) { memcpy (p, which, min (old_size, new_size));
               free (which);
             }
  which= p;
  return ACC_OK;
}


char* fnsplit (char* f)
{ char* p;

#ifdef _LINUX
#define FNSPLITCHAR '/'
#else
#define FNSPLITCHAR '\\'
#endif

  p= strrchr (f, FNSPLITCHAR);
  if (p!= NULL)
   { if (*p== 0) return NULL;
          else if (*(p+1)== 0) return NULL;
                    else return strdup (p+1);
   }
  else
    return strdup (f);
}


/*  *****************************************************************  */
// TACCRes
signed char TACCRes::LoadACC (char* f)
{ unsigned char ch;
  FILE* F;
  long rd, wr;
  unsigned long ind, lg;
  char* p;
  static char Header[4]= "ACC";
  unsigned char Buf[1024];

  if (!pTable) Done();
  pTable= NULL;
  NbEntry= 0;
  CurrentFile= &PEmpty;
  if ((F= fopen (f, "rb"))== NULL) return ACC_FILENOTFOUND;


// Lecture de l'entête
  fread (Buf, 1, 3, F); Buf[3]= 0;
  if (strcmp ((const char*) Buf, Header)) { fclose (F);
                              return ACC_FILECORRUPT; }

  fread (Buf, 1, 3, F);
  NbEntry= Buf[0]+ (unsigned long) Buf[1]* 256;
  if (!NbEntry) { fclose (F);
                  return ACC_FILECORRUPT; }

  fread (&ch, 1, 1, F);
  pTable= (TEntry *) malloc (NbEntry* sizeof(TEntry));
  if (!pTable)  { fclose (F);
                  return ACC_NOTENOUGHMEMORY; }

  ind= 0;
  while (!feof(F))
  { if (ch!= 0 && ch> 2) { NbEntry= 0;
                           free (pTable);
                           pTable= NULL;
                           fclose (F);
                           return ACC_FILECORRUPT; }
    if (ch== 2) break;
    fread (&ch, 1, 1, F);
    fread (Buf, 1, ch, F);

    // prevent an overflowing copy
    if (ch> TEntrySize- 1) ch= TEntrySize- 1;
    Buf[ch]= 0;
	strcpy (pTable[ind].Name, (const char*) Buf);
    fread (Buf, 1, 3, F);
    
    pTable[ind].Length= 1UL* (unsigned char) Buf[0]+ 256UL* (unsigned char) Buf[1]+ 65536UL* (unsigned char) Buf[2];
    fread (&ch, 1, 1, F);
    ind++;
  }

// Lecture des données
  for (ind= 0; ind<= NbEntry- 1 && (!feof(F)); ind++)
   { p= (char*) malloc (pTable[ind].Length);
     if (!p) { fclose (F);
               Done();
               return ACC_NOTENOUGHMEMORY; }
     pTable[ind].pData= p;

    for (lg= pTable[ind].Length, wr= 0; lg> 0; )
     if (lg>= 1024) { rd= fread (Buf, 1, 1024, F);
     				// hint for future version, check if read was successful
                      lg-= rd;
                      memcpy (pTable[ind].pData+ wr, Buf, rd);
                      wr+= rd; }
              else  { rd= fread (Buf, 1, lg, F);
     				// hint for future version, check if read was successful
                      lg= 0;
                      memcpy (pTable[ind].pData+ wr, Buf, rd);
                      wr+= rd; }
  }
  fclose (F);
  if (ind< NbEntry) { Done();
                      return ACC_FILECORRUPT; }

// Le fichier est correct.
  CurrentFile= strdup(f);
  return ACC_OK;
};


long TACCRes::EntryId (char* FName)
{ unsigned i;

  if (!pTable) return ACC_NOTINITIALIZED;
  for (i= 0; i< NbEntry; i++)
   if (!strcmp (pTable[i].Name, FName)) return i; // found
  return ACC_ENTRYNOTFOUND;
}

char* TACCRes::EntryName (unsigned Id) { return pTable[Id].Name; };
char* TACCRes::EntryPtr (unsigned Id)  { return pTable[Id].pData; };
unsigned long TACCRes::EntryLength (unsigned Id) { return pTable[Id].Length; };

void TACCRes::Done()
{ if (pTable)
   { for ( ; NbEntry> 0; NbEntry--)
      if (pTable[NbEntry- 1].pData) free (pTable[NbEntry- 1].pData);
     free (pTable);
     pTable= NULL;
   }
  
   if (CurrentFile!= NULL)
   { free (CurrentFile);
     CurrentFile= NULL;
   }
};

/*  *****************************************************************  */

// TACCIndex

signed char TACCIndex::LoadACC (char* f)
{ unsigned char ch;
  long rd, wr;
  unsigned long ind, lg;
  char* p;
  static char Header[4]= "ACC";
  unsigned char Buf[1024];

  if (!pTable) Done();
  pTable= NULL;
  NbEntry= 0;
  CurrentFile= &PEmpty;
  if ((file= fopen (f, "rb"))== NULL) return ACC_FILENOTFOUND;

// Lecture de l'entête
  fread (Buf, 1, 3, file); Buf[3]= 0;
  HeaderSize= 3;
  if (strcmp ((const char*) Buf, Header))
	{	fclose (file);
		file= NULL;
		return ACC_FILECORRUPT; }

  fread (Buf, 1, 3, file);
  HeaderSize+= 3;
  NbEntry= Buf[0]+ (unsigned) Buf[1]* 256;
  if (!NbEntry)	 { fclose (file);
					file= NULL;
                  	return ACC_FILECORRUPT; }

  fread (&ch, 1, 1, file);
  HeaderSize+= 1;
  pTable= (TEntry *) malloc (NbEntry* sizeof(TEntry));
  if (!pTable)	{ fclose (file);
					file= NULL;
     				return ACC_NOTENOUGHMEMORY; }

  ind= 0;
  while (!feof(file))
  { if (ch!= 0 && ch> 2) { NbEntry= 0;
                           free (pTable);
                           pTable= NULL;
                           fclose (file);
                           file= NULL;
                           return ACC_FILECORRUPT; }
    if (ch== 2) break;
    fread (&ch, 1, 1, file);
    HeaderSize+= 1;
    fread (Buf, 1, ch, file);
    // prevent an overflowing copy
    if (ch> TEntrySize- 1) ch= TEntrySize- 1;
    Buf[ch]= 0;
    HeaderSize+= ch;
    strcpy (pTable[ind].Name, (const char*) Buf);
    fread (Buf, 1, 3, file);
    HeaderSize+= 3;
    pTable[ind].Length= 1UL* (unsigned char) Buf[0]+ 256UL* (unsigned char) Buf[1]+ 65536UL* (unsigned char) Buf[2];
    fread (&ch, 1, 1, file);
    HeaderSize+= 1;
    ind++;
  }

// Lecture des données
  for (ind= 0; ind<= NbEntry- 1 && (!feof(file)); ind++)
   { p= (char*) malloc (pTable[ind].Length);
     if (!p) { Done();
               return ACC_NOTENOUGHMEMORY; }
     pTable[ind].pData= p;

    for (lg= pTable[ind].Length, wr= 0; lg> 0; )
     if (lg>= 1024) { rd= fread (Buf, 1, 1024, file);
     				// hint for future version, check if read was successful
                      lg-= rd;
                      memcpy (pTable[ind].pData+ wr, Buf, rd);
                      wr+= rd; }
              else  { rd= fread (Buf, 1, lg, file);
     				// hint for future version, check if read was successful
                      lg= 0;
                      memcpy (pTable[ind].pData+ wr, Buf, rd);
                      wr+= rd; }
  }
//  fclose (file);
  if (ind< NbEntry) { Done();
                      return ACC_FILECORRUPT; }

// Le fichier est correct.
  CurrentFile= f;
  return ACC_OK;
};


long TACCIndex::EntryId (char* FName)
{ unsigned i;

  if (!pTable) return ACC_NOTINITIALIZED;
  for (i= 0; i< NbEntry; i++)
   if (!strcmp (pTable[i].Name, FName)) return i; // found
  return ACC_ENTRYNOTFOUND;
}


char* TACCIndex::EntryName (unsigned Id) { return pTable[Id].Name; };
unsigned long TACCIndex::EntryOffset (unsigned Id)
{ unsigned long i;
  unsigned long l= HeaderSize;

  for (i= 0; i< Id; i++) l+= pTable[i].Length;
  return l;
};


unsigned long TACCIndex::EntryLength (unsigned Id) { return pTable[Id].Length; };


void TACCIndex::Done()
{ if (pTable)
   { free (pTable);
     pTable= NULL; }
  CurrentFile= NULL;

	if (file!= NULL)
   {	fclose (file);
   	file= NULL; }
}


signed char TACCIndex::SetPosition (unsigned Id)
{
	if (file== NULL) return ACC_NOTINITIALIZED;
   if ( fseek(file, EntryOffset(Id), SEEK_SET) ) return ACC_FSEEKFAILED;
   return ACC_OK;
}

/*  *****************************************************************  */
// TACCEditMem
void TACCEditMem::Done()
{
  TACCRes::Done();
  CurTabSize= 0;
}


signed char TACCEditMem::AddEntry (char* f, unsigned Pos)
{ unsigned char Buf[TAEMBUFSIZE];
  char* FName;
  char *p, *p2;

  unsigned long NR, Rest;
  FILE* FRead;

  FRead= fopen (f, "rb");
  if (FRead== NULL) return ACC_FILENOTFOUND;

  fseek (FRead, 0, SEEK_END);
  NR= Rest= ftell (FRead);
  fseek (FRead, 0, SEEK_SET);

// On ne garde que le nom du fichier
  FName= fnsplit (f);
  if (FName== NULL) return ACC_INVALIDPARAMETER;

// La table des entrées est-elle trop petite ?
  if (NbEntry* sizeof(TEntry)== CurTabSize)
   if (ChangeMemSize ( (void*&) pTable, CurTabSize,
                        CurTabSize+ 16* sizeof(TEntry))< 0) /* bug quelque part >.< */
                      return ACC_NOTENOUGHMEMORY;
  CurTabSize+= 16* sizeof(TEntry);

// Chargement en mémoire
  if ( (p= (char*) malloc (NR))== NULL) { fclose (FRead);
                                          return ACC_NOTENOUGHMEMORY; }

  p2= p;

  while (Rest> TAEMBUFSIZE) { fread (Buf, 1, TAEMBUFSIZE, FRead);
  							// we don't check how much we actually read
  							// hint for future version: if we can't read all we expected,
  							// exit with an error
                               memcpy (p2, Buf, TAEMBUFSIZE);
                               p2+= TAEMBUFSIZE;
                               Rest-= TAEMBUFSIZE; }
  fread (Buf, 1, Rest, FRead);
	// hint for future version, check if read was successful
  memcpy (p2, Buf, Rest);

// Mise à jour de la table
  if (Pos> NbEntry) Pos= NbEntry;
  for (unsigned ind= NbEntry; ind> Pos; ind--)
pTable[ind]= pTable[ind- 1];
  NbEntry++;

  // prevent an overflowing copy
  if (strlen(FName)>= TEntrySize)
    FName[TEntrySize- 1]= 0;

  strcpy (pTable[Pos].Name, FName);
  pTable[Pos].Length= NR;
  pTable[Pos].pData= p;

  if (FName!= NULL) free (FName);

  return ACC_OK;
}


signed char TACCEditMem::DelEntry (unsigned Id)
{ if (!NbEntry) return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_ENTRYNOTFOUND;

// Mise à jour du tableau
  if (pTable[Id].pData) free (pTable[Id].pData);
  for (unsigned i= Id; i< NbEntry- 1; i++) pTable[i]= pTable[i+ 1];
  memset (pTable+ NbEntry- 1, 0, sizeof(TEntry));

// Peut-on réduire la table ?
  if (NbEntry* sizeof(TEntry)< CurTabSize/ 2)
   if (ChangeMemSize ( (void*&) pTable, CurTabSize, CurTabSize/ 2)== ACC_OK)
    CurTabSize/= 2;

  NbEntry--;
  return ACC_OK;
}


signed char TACCEditMem::ExtractEntry (unsigned Id, char* f)
{ unsigned long Rest;
  char *p;

  FILE* FWrite;
  signed Written;

  if (!NbEntry) return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_ENTRYNOTFOUND;
  if (!f) f= pTable[Id].Name;
  FWrite= fopen (f, "wb");
  if (FWrite== NULL) return ACC_CANNOTCREATEFILE;

  p= EntryPtr (Id);
  Rest= EntryLength (Id);

// Extraction vers le fichier
  while (Rest> TAEMBUFSIZE) { Written= fwrite (p, 1, TAEMBUFSIZE, FWrite);
                              if (Written== 0 || Written== -1)
                               { fclose (FWrite);
                                 return ACC_DISKFULL; // or permission denied
                               }
                              p+= TAEMBUFSIZE;
                              Rest-= TAEMBUFSIZE; }
  if (Rest> 0)
   { Written= fwrite (p, 1, Rest, FWrite);
     if (Written== 0 || Written== -1)
      { fclose (FWrite);
        return ACC_DISKFULL; //or permission denied
      }
   }
  fclose (FWrite);
  return ACC_OK;
}


signed char TACCEditMem::LoadACC (char* f)
{ signed char i;

  i= TACCRes::LoadACC (f);
  if (i== ACC_OK) CurTabSize= NbEntry* sizeof(TEntry);
  return i;
}


signed char TACCEditMem::SaveACC (char* f)
{ char Header[]= "ACC";
  char* p;
  long Rest;
  signed Written;
  unsigned ind;
  FILE* F;
  char c;

  if (!NbEntry) return ACC_NOTINITIALIZED;
  if (!f)
   if (!CurrentFile) return ACC_NOTINITIALIZED;
                else f= CurrentFile;
  if ( (F= fopen (f, "wb"))== NULL) return ACC_CANNOTCREATEFILE;

// Ecriture de l'entête
  fwrite (Header, 1, 3, F);
  c= (char) (NbEntry% 256); fwrite (&c, 1, 1, F);
  c= (char) (NbEntry/ 256); fwrite (&c, 1, 1, F);
  c= 0; fwrite (&c, 1, 1, F);
  c= 1; fwrite (&c, 1, 1, F);

  for (ind= 0; ind< NbEntry; ind++)
   { c= strlen (EntryName (ind)); fwrite (&c, 1, 1, F);
     fwrite (EntryName(ind), 1, c, F);
     Rest= EntryLength (ind);

     c= (char) (Rest% 256); fwrite (&c, 1, 1, F);
     c= (char) (Rest/ 256); fwrite (&c, 1, 1, F);
     c= (char) (Rest/ 65536); fwrite (&c, 1, 1, F);
     c= (char) ((ind== NbEntry- 1)+ 1); fwrite (&c, 1, 1, F);
   }

// Ecriture des données
  for (ind= 0; ind< NbEntry; ind++)
   { p= EntryPtr (ind);
     Rest= EntryLength (ind);

     while (Rest> TAEMBUFSIZE) { Written= fwrite (p, 1, TAEMBUFSIZE, F);
                                 if (Written== 0 || Written== -1)
                                 { fclose (F);
                                   return ACC_DISKFULL; //or permission denied
                                 }
                                 p+= TAEMBUFSIZE;
                                 Rest-= TAEMBUFSIZE; }
                                 
    if (Rest> 0) { Written= fwrite (p, 1, Rest, F);
                   if (Written== 0 || Written== -1)
                   { fclose (F);
                     return ACC_DISKFULL; //or permission denied
                   }
                 }

   }

  fclose (F);
  return ACC_OK;
}

// v 1.0 TACCRes
// v 2.0 TACCEditMem
// v 2.1 Format ACC modifié‚: ajout de l'entête "ACC", et du nombre d'entrées
//        (2 octets) puis #0
// v 2.2 Ajout TACCIndex
