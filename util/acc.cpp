#include <popt.h>
#include <string.h>
#include "direct.h"
#define VERSION "0.1"

//char* fnsplit (char* f);

int verbose= 0;
int action= 0;
char* accname= NULL;
char* prgmname= NULL;
char** Files;

signed char AddFiles()
{ TACCEditMem Acc;
  FILE* f;
  signed long res;
  unsigned long i;

  f= fopen (accname, "r");
  
  if (f!= NULL) 
   { fseek (f, 0, SEEK_END);
     i= ftell (f);
     fclose (f);
     if (i!= 0) res= Acc.LoadACC (accname);
            else { Acc.CurrentFile= accname; res= ACC_OK; }
     // LoadACC will fail if the file is empty
   }    
     
   else { f= fopen (accname, "w+");
            if (f== NULL) { fprintf (stderr, "%s is missing and cannot be created. Aborting.\n", accname);
                             return -3; }
              fclose (f);
              if (verbose) printf ("%s has been created.\n", accname);
              res= ACC_OK;
              Acc.CurrentFile= accname;
          }
   
      	
  if (res== ACC_FILECORRUPT)
   { fprintf (stderr, "%s seems to be corrupted.\n", accname); return -1; }
  if (res== ACC_NOTENOUGHMEMORY)
   { fprintf (stderr, "Not enough memory.\n"); return -2; }
   

  for (i= 0; Files[i]!= NULL; i++)
   { res= Acc.EntryId(Files[i]);
     if (res!= ACC_NOTINITIALIZED && res!= ACC_ENTRYNOTFOUND)
      { fprintf (stderr," Could not add file %s: file is already in the ACC.\n", Files[i]);
        if (verbose) printf ("\t(Found with Id: %d)\n", (int) res);
        continue; }
   
     res= Acc.AddEntry (Files[i], Acc.NbEntry);
     if (res< 0)
      { fprintf (stderr," Could not add file %s: ", Files[i]);
        switch (res)
        {case ACC_FILENOTFOUND: printf ("File not found\n"); break;
         case ACC_INVALIDPARAMETER: printf ("Invalid parameter\n"); break; 
         case ACC_NOTENOUGHMEMORY: printf ("Not enough memory\n"); break;}
      } else if (verbose) printf ("%s has been added.\n", Files[i]);
    }
 
  res= Acc.SaveACC (NULL);
  if (res< 0)
  { fprintf (stderr, " Could not create new ACC file : ");
    switch (res)
    { case ACC_CANNOTCREATEFILE: printf ("Could not write to file\n");
      case ACC_DISKFULL: printf ("Disk full or permission denied\n");
    }
   } else if (verbose) printf ("ACC file modified.\n");
  return 1;
}


/* ******************************************** */
/* *               RemoveFiles                * */
/* ******************************************** */


signed char RemoveFiles()
{ TACCEditMem Acc;
  FILE* f;
  signed long res;
  unsigned long i;

  f= fopen (accname, "r");
  if (f== NULL) { fprintf (stderr, "%s is missing. Aborting.\n", accname);
                   return -1; }

  res= Acc.LoadACC (accname);      
  if (res== ACC_FILECORRUPT)
   { fprintf (stderr, "%s seems to be corrupted.\n", accname); return -2; }
  if (res== ACC_NOTENOUGHMEMORY)
   { fprintf (stderr, "Not enough memory.\n"); return -3; }
   
   
  for (i= 0; Files[i]!= NULL; i++)
   { res= Acc.EntryId(Files[i]);
     if (res== ACC_NOTINITIALIZED && res== ACC_ENTRYNOTFOUND)
      { fprintf (stderr," Could not find %s in ACC file.\n", Files[i]);
        continue; }
   
      Acc.DelEntry (res);
      if (verbose) printf ("%s has been removed.\n", Files[i]);
    }
 
  if (Acc.NbEntry== 0) 
   { if (verbose) printf ("Warning, last ACC entry has been deleted. %s is erased\n", accname);
     remove (accname);
     return 0; }
     
  res= Acc.SaveACC (NULL);
  if (res< 0)
  { fprintf (stderr, " Could not create new ACC file : ");
    switch (res)
    { case ACC_CANNOTCREATEFILE: printf ("Could not write to file\n");
      case ACC_DISKFULL: printf ("Disk full or permission denied\n");
    }
   } else if (verbose) printf ("ACC file modified.\n");
   
  return 0;
}




/* ******************************************** */
/* *                   ListFile               * */
/* ******************************************** */


signed char ListFile()
{ TACCEditMem Acc;
  FILE* f;
  signed long res;
  unsigned long i;

  f= fopen (accname, "r");
  if (f== NULL) { fprintf (stderr, "%s is missing. Aborting.\n", accname); return -1; }

  res= Acc.LoadACC (accname);      
  if (res== ACC_FILECORRUPT) { fprintf (stderr, "%s seems to be corrupted.\n", accname); return -2; }
  if (res== ACC_NOTENOUGHMEMORY) { fprintf (stderr, "Not enough memory.\n"); return -3; }
   
  for (i= 0; i< Acc.NbEntry; i++)
   if (verbose) printf ("#%li: %s (size= %li) \n", i, Acc.EntryName(i), Acc.EntryLength(i) );
           else printf ("%li: %s\n", i, Acc.EntryName(i) );
           
  return 0;
}





/* ******* *
 *  Usage  *
 * ******* * */ 
 
static void Version();
static void Usage()
{ Version();
  fprintf (stderr, "\nUsage: %s -hv -f accfile [-a|-l|-r] filenames\n\n"
      "You must specify an action:\n"
      "    -a, --add              : add files to target file\n"
      "    -r, --remove           : remove files (enter filenames without any path)\n"
      "    -l, --list             : list contents of target file\n\n"
      "Valid options are:\n"
      "    -f, --file             : Specify the target ACC file (mandatory)\n "
      "    -h, --help             : Display this usage information and exit\n"
      "    -V, --version          : Display the version and exit\n"
      "    -v, --verbose          : Display verbose information\n"
    , prgmname);
}


static void Version() { fprintf(stderr, "acc v"VERSION" - 2001 Krys\n"); }

/* ******* *
 *   Main  *
 * ******* * */ 



int main(int argc, char *argv[])
{ int file_specified= 0;
//  char *buffer, **Files;
  int c= 0;
  poptContext optCon;
  struct poptOption options[]= {
    { "help", 'h', 0, NULL, 'h' },
    { "version", 'V', 0, NULL, 'V' }, 
    { "verbose", 'v', 0, NULL, 'v' },
    { "add", 'a', 0, NULL, 'a' },
    { "list", 'l', 0, NULL, 'l' },
    { "remove", 'r', 0, NULL, 'r' },
    { "file", 'f', POPT_ARG_STRING, &accname, 'f' },
    { NULL, 0, 0, NULL, 0 }
  };

  prgmname= argv[0];
  
  optCon= poptGetContext("acc", argc, (const char **)argv, options, 0);
  poptReadDefaultConfig(optCon, 0);

  while ((c= poptGetNextOpt(optCon))>= 0) switch (c) 
  { case 'a': action= 1; break;
    case 'f': file_specified= 1; break; 
    case 'h': Usage(); return 0;
    case 'l': action= 3; break;
    case 'r': action= 2; break;
    case 'V': Version(); return 0;
    case 'v': verbose= 1; break;
  }

  Files= (char **)poptGetArgs(optCon);  
  
  if (file_specified== 0) { Usage(); 
                             fprintf (stderr, "Target ACC file must be specified (with -f).\n"); 
                             return 1; }
                             
  if (c< -1) { fprintf (stderr, "%s: %s\n", 
                         poptBadOption(optCon, POPT_BADOPTION_NOALIAS),
                         poptStrerror(c));
                return 2; }

  if ( action== 0 || ( (!Files || !Files[0]) && action!= 3) ) 
   { fprintf (stderr, "Nothing to be done.\n"); 
     return 3; }

  switch (action) 
  { case 1: return AddFiles();
    case 2: return RemoveFiles();
    case 3: return ListFile();
  } 
  
  return 0;
}
 