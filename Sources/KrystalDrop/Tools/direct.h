// direct.h v3.0 
// October 2002
// Christopher Gautier (krys@via.ecp.fr)

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.

// history
// v 1.0 TACCRes
// v 2.0 TACCEditMem
// v 2.1 header ACC modified
// v 2.2 TACCIndex
// v 3.0 rewrite

// TACCRes: ACC file reader (read from memory)
// TACCEditMem: can add, delete, extract from memory
// TACCIndex: ACC file reader (read from disk)

// Max entries: 16777215 entries
// Max entry name length: 255 characters
// Max entry length: 2147483647 bytes

#ifndef DIRECT_H
#define DIRECT_H
#define DIRECT_VER "v3.0"

#include <stdio.h>

// Entries attributes
#define ACC_LINK         1 /* symbolic link into another entry, links to link are forbidden to avoid infinite loops */
#define ACC_LOADED       2 /* loaded in memory */
#define ACC_ZIPPED       4 /* data is compressed (on disk) */
#define ACC_LOAD          (0+  0)
#define ACC_DONTLOAD      (0+  8) /* never load */
#define ACC_LOADONDEMAND  (16+ 0) /* don't load in LoadACC, only when calling EntryPtr */
//#define ACC_FETCHONNETWORK (16+ 8) // future version functionality
#define ACC_REMOVEFROMMEM 32 /* don't change load policy but remove from memory when calling ChangeLoadPolicy */

#define ACC_POLICYMASK (8+ 16+ 32) /* relevant bits for ChangePolicy */

#define ACC_GETLOAD(attr) ((attr)& (8+16))
#define ACC_SETLOAD(attr,load) { attr= ((attr) & (~(8+16))) | (load); } /* assume load== load& (8+ 16) */

#define ACC_HEADER 0x33434341

#ifndef ACC_DEFINES_OFFSET
#define ACC_DEFINES_OFFSET 0
// Define this value to avoid possible error codes clashes, with other 
// libraries for instance.
// This value is added to any ACC_XXXX defines and is likely to be negative.
#endif

#ifndef ACC_IOBUFFER_SIZE
#define ACC_IOBUFFER_SIZE 1024
// fread/fwrite buffer size (must be > 4), buffer uses stack
#endif

#ifndef ACC_FNSPLITCHAR // directory delimiter character (OS dependant)
#ifdef _LINUX
#define ACC_FNSPLITCHAR '/'
#elif _WIN32
#define ACC_FNSPLITCHAR '\\'
#elif _MAC
#define ACC_FNSPLITCHAR ':'
#define ACC_SWAP // not sure, see rwbuffer()
#else
#define ACC_FNSPLITCHAR '/'  // default
#endif
#endif // #ifndef ACC_FNSPLITCHAR


/* Error codes */
#define ACC_OK 0
#define ACC_NOTENOUGHMEMORY  ( -101+ ACC_DEFINES_OFFSET )
#define ACC_INVALIDPARAMETER ( -102+ ACC_DEFINES_OFFSET )
#define ACC_NOTSUPPORTED     ( -103+ ACC_DEFINES_OFFSET )
#define ACC_ENTRYWOULDBELOST ( -104+ ACC_DEFINES_OFFSET )
#define ACC_FILENOTFOUND     ( -111+ ACC_DEFINES_OFFSET )
#define ACC_FILECORRUPTED    ( -112+ ACC_DEFINES_OFFSET )
#define ACC_ENTRYNOTFOUND    ( -113+ ACC_DEFINES_OFFSET )
#define ACC_CANNOTCREATEFILE ( -114+ ACC_DEFINES_OFFSET )
#define ACC_DISKFULL         ( -115+ ACC_DEFINES_OFFSET )
#define ACC_SEEKFAILED       ( -116+ ACC_DEFINES_OFFSET )
#define ACC_READFAILED       ( -117+ ACC_DEFINES_OFFSET )
#define ACC_WRITEFAILED      ( -118+ ACC_DEFINES_OFFSET )
#define ACC_WONTLOADENTRY    ( -119+ ACC_DEFINES_OFFSET )
#define ACC_ZLIBERROR        ( -120+ ACC_DEFINES_OFFSET ) 
#define ACC_NOTINITIALIZED   ( -126+ ACC_DEFINES_OFFSET )
#define ACC_NOTIMPLEMENTED   ( -127+ ACC_DEFINES_OFFSET )

/*typedef*/ struct CACCEntry;

class CACCRes 
{ protected:
   CACCEntry*    pTable;     // points to the entry table
   unsigned long NbEntry;
   unsigned long HeaderSize; 
   FILE*         file;
   char*         CurrentFile;// current ACC file
  
     signed char  WriteHeader (FILE* F);
  public:  
                  CACCRes();
                 ~CACCRes();

            void  Done ();
     signed char  ChangeAllLoadPolicy (unsigned attr);  
     signed char  ChangeLoadPolicy    (unsigned Id, unsigned attr);
     signed long  EntryAttr  (unsigned Id); // Entry's attributes
     signed long  EntryDiskLength (unsigned Id); // Entry's length in ACC file
     signed long  EntryId    (const char* f);    // First ID matching name f
     signed long  EntryLinkId(unsigned Id); // destination of a link, (or Id if not a link)
     signed long  EntryLength(unsigned Id); // Entry's length in memory
            char* EntryName  (unsigned Id); // Entry's name     
     signed long  EntryOffset(unsigned Id);
     signed char  EntryPtr   (unsigned Id, const char** p); // Entry's data pointer in memory
      const char* GetErrorMessage (signed char error_code);
            FILE* GetFile();
            char* GetName();
   unsigned long  GetNbEntry();
     signed char  InitACC    (const char* f, unsigned char open_rw= 0); // Read an ACC header
     signed char  LoadACC    ();              // Load data from an ACC file
     signed char  LoadEntry  (unsigned Id);
     signed char  MoveFileAt (unsigned Id);

#ifdef ACC_DEBUG
            void  Dump();
#endif
};

#ifndef ACC_NOACCEDITMEM
class CACCEditMem: public CACCRes
// this version always loads the entries in memory, no matter what their attributes are
// This is because adding, renaming  or removing entries in memory, while not updating 
// the file stored on disk makes very difficult copying the original data into the new 
// archive
// That is why LoadACCInMemory should be used instead of LoadACC
{ public:
               CACCEditMem();
   signed char AddEntry (const char* f, unsigned long Pos, unsigned long Attr, unsigned long* data);
   signed char DelEntry (unsigned Id);
   signed char InitACC  (const char* f);
   signed char LoadACCInMemory ();
   signed char SaveACC  (const char* f);  // if f== NULL then f= CurrentFile
   signed char ExtractEntry (unsigned Id, const char* f); // if f== NULL then f= pTable[Id].Name
          void Done();
          void SetName (const char* name);

  protected:
   unsigned long CurTabSize;
};
#endif

// utility function: returns 1 if f exists, 0 if f doesn't exist (or another error occured !)
signed Exist (const char* f);


#endif
