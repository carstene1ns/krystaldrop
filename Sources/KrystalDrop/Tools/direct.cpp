// Direct v3.0 for C

// correct attr when saving
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#ifdef ACC_ZLIB_SUPPORT
#include <zlib.h>
#endif

#include "direct.h"

#ifndef min
#define min(a,b) ( (a<b) ? a : b )
#endif

typedef struct CACCEntry
{ char* Name;
  unsigned long Length; /* length (unzipped) */
  unsigned long DiskLength; /* length on disk (possibly zipped) */
  unsigned long Offset; /* offset in file OR link destination */
  /* offsets are invalid after calling AddEntry (because the compression isn't
     done yet). This is not true if links are inserted.
  */
  unsigned long Attr;
  char* pData;
};

static unsigned long acc_header= ACC_HEADER;

/* ***************** */ 
/* utility functions */
/* ***************** */ 
// check if f already exists
signed Exist (const char* f)
{ FILE* file= fopen (f, "rb");
  if (file== NULL) 
  { // if (errno== ENOENT) ok, but other errors can occur
    return 1;
  } else 
  { fclose (file);
    return 0;
  }  
}

/*
// alternate Exist function, for UNIX systems
#include <unistd.h>
#include <sys/stat.h>
signed Exist2 (char* f)
{ struct stat buf;
  int res= stat (f, &buf);
  return (!(res== 0));
}
*/


signed char ChangeMemSize (void* &p, long new_size)
// wrap for realloc
{ void* p2;
  
  assert (new_size!= 0);
  p2= (p== NULL)?malloc (new_size):realloc (p, new_size);
  if (p2== NULL) return ACC_NOTENOUGHMEMORY;
  p= p2;
  
  return ACC_OK;
}


char* fnsplit (const char* f)
// returns a copy of the file name without the path
{ char* p;  
  assert (ACC_FNSPLITCHAR);
  assert (f);
  assert (*f!= 0);

  p= strrchr (f, ACC_FNSPLITCHAR);
  if (p!= NULL)
  { if (*p== 0)      return NULL; // this line should be useless, according to strrchr manpage
    if (*(p+ 1)== 0) return NULL;
    return strdup (p+ 1);
  } else return strdup (f); // no delimiter found, copy the whole string
}


void rwbuffer (void* dest, void* src, unsigned short len)
// in LSB architectures, simply copy src to dest
// in MSB architectures, swap the byte order when copying src to dest
// this function should be called for len= 2, 4 or 8
{ 
#ifdef ACC_SWAP
  // architecture is MSB first, must swap the data
  for (unsigned short i= len; i> 0; i--) (char*) dest[i- 1]= (char*) src[len- i];
#else
  // architecture is LSB first
  memcpy (dest, src, len);
#endif
}

#ifdef ACC_ZLIB_SUPPORT
/* signed char Read2Mem (char* name, char** p, long* size) // debug only 
{ FILE* f_in= fopen (name, "rb"); if (f_in== NULL) return ACC_FILENOTFOUND;
  if (fseek (f_in, 0, SEEK_END)== -1) { fclose (f_in); return ACC_SEEKFAILED; }
  *size= ftell (f_in); if (*size== -1) { fclose (f_in); return ACC_SEEKFAILED; }
  *p= (char*) malloc (*size); if (*p== NULL) { fclose (f_in); return ACC_NOTENOUGHMEMORY; }
  if (fseek (f_in, 0, SEEK_SET)== -1) { fclose (f_in); return ACC_SEEKFAILED; }
  size_t rd= fread (*p, 1, *size, f_in);
  fclose (f_in);
  if (rd< *size) return ACC_READFAILED;
  return ACC_OK;
} */

signed char CompressMem2File (const char* p, unsigned long size, FILE* f_out, unsigned long* compr_size)
/* compress (size) bytes pointed by p and write the result to f_out. Returns the output size to compr_size */
{ struct z_stream_s zs;
  char io_buf[ACC_IOBUFFER_SIZE];  
  size_t wr;
  int res;
  char end= 0;
  assert (p);
  assert (f_out);

  zs.zalloc= Z_NULL;
  zs.zfree=  Z_NULL;
  zs.opaque= Z_NULL;
  zs.next_in=  (Bytef*) p;
  zs.next_out= (Bytef*) io_buf;
  zs.avail_in=  size;
  zs.avail_out= ACC_IOBUFFER_SIZE;

  res= deflateInit (&zs, 9);
  if (res== Z_MEM_ERROR) return ACC_NOTENOUGHMEMORY;
  if (res!= Z_OK) return ACC_ZLIBERROR; /* should never happen */
    
  while (!end)
  { assert (zs.avail_in> 0 || (zs.avail_in== 0 && zs.total_in== size));
    res= deflate (&zs, Z_FINISH);
    if (res== Z_STREAM_END) end= 1;
      else if (res!= Z_OK)
    { deflateEnd (&zs);
      return ACC_ZLIBERROR;
    }
    
    wr= fwrite (io_buf, 1, ACC_IOBUFFER_SIZE- zs.avail_out, f_out);
    if (wr< ACC_IOBUFFER_SIZE- zs.avail_out)
    { deflateEnd (&zs);
      return ACC_WRITEFAILED;
    }
    
    zs.avail_out= ACC_IOBUFFER_SIZE;
    zs.next_out= (Bytef*) io_buf;
  }
  
  if (compr_size!= NULL) *compr_size= zs.total_out;
  res= deflateEnd (&zs);
  if (res!= Z_OK) return ACC_ZLIBERROR; /* should never happen */
  return ACC_OK;
}


signed char DecompressFile2Mem (FILE* f_in, unsigned long compr_size, char** p, unsigned long size)
/* read (compr_size) bytes from f_in, allocates (size) bytes for p and expects to decompress (size) bytes */
{ struct z_stream_s zs;
  char io_buf[ACC_IOBUFFER_SIZE];  
  size_t rd;
  int err;
  int res;
  char end= 0;
  char end_input= 0;
  assert (f_in);

  *p= (char*) malloc (size);
  if (*p== NULL) return ACC_NOTENOUGHMEMORY;

  zs.zalloc= Z_NULL;
  zs.zfree=  Z_NULL;
  zs.opaque= Z_NULL;
  zs.next_in=  (Bytef*) io_buf;
  zs.next_out= (Bytef*) *p;
  zs.avail_in= 0;  
  zs.avail_out= size;

  res= inflateInit (&zs);
  if (res== Z_MEM_ERROR) { free (*p); return ACC_NOTENOUGHMEMORY; }
  if (res!= Z_OK) { free (*p); return ACC_ZLIBERROR; } /* should never happen */
   
  while (!end)
  { if (zs.avail_in== 0 && end_input== 0)
    { rd= fread (io_buf, 1, ACC_IOBUFFER_SIZE, f_in);
      if (ferror(f_in)!= 0) 
      { err= ACC_READFAILED;
        goto error;
      }
      if (rd< ACC_IOBUFFER_SIZE) end_input= 1;
      zs.avail_in= rd;
      zs.next_in= (Byte*) io_buf;
    }

    res= inflate (&zs, Z_SYNC_FLUSH);
    if (res== Z_STREAM_END) end= 1;
      else if (res!= Z_OK) 
    { err= ACC_ZLIBERROR; /* should never happen */
      goto error;
    }
  }
  
  assert (size== zs.total_out);
  res= inflateEnd (&zs);
  if (res!= Z_OK) return ACC_ZLIBERROR; /* should never happen */ 
  return ACC_OK;

error:
  inflateEnd (&zs);
  free (*p);
  return err;
}
#endif // ACC_ZLIB_SUPPORT


/* ******* */
/* CACCRes */
/* ******* */
CACCRes::CACCRes()
{ pTable= NULL;
  CurrentFile= NULL;
  file= NULL;
  NbEntry= 0; 
  HeaderSize= 0;
}


CACCRes::~CACCRes()
{ Done();
}


signed char CACCRes::InitACC (const char* f, unsigned char open_rw)
{ unsigned long ind, lg, attr, offset;
  long rd;  
  char Buf[ACC_IOBUFFER_SIZE];  
  char ch;  

  if (!pTable) Done();
  pTable= NULL;
  NbEntry= 0;
  CurrentFile= NULL;
  
  if ((file= fopen (f, (open_rw== 0)?"rb":"r+b"))== NULL) return ACC_FILENOTFOUND;

// Header parsing
  rd= fread (Buf, 4, 1, file);
  if (rd< 1) goto file_corrupted;
  rwbuffer (&lg, Buf, 4);
  if (lg!= acc_header) goto file_corrupted;

// Header, nb entries (< 16777216)
  fread (Buf, 4, 1, file);
  if (rd< 1) goto file_corrupted;  
  rwbuffer (&NbEntry, Buf, 4);
  if ((NbEntry& 0xFF000000)>> 24!= 0x03) goto version_unsupported;
  NbEntry&= 0x00FFFFFF;
  if (NbEntry== 0) goto file_corrupted;
  HeaderSize= 8;

  pTable= (CACCEntry*) malloc (NbEntry* sizeof(CACCEntry));
  if (!pTable) goto not_enough_mem;
  memset (pTable, 0, NbEntry* sizeof(CACCEntry)); // this helps when freeing memory if the file is corrupted

// Header, read the filenames and the attributes
  ind= 0;
  while (!feof(file) && ind< NbEntry)
  {  
    rd= fread (&ch, 1, 1, file); // filename length
    if (rd< 1) goto file_corrupted;
    pTable[ind].Name= (char*) malloc (ch+ 1);
    if (pTable[ind].Name== NULL) goto not_enough_mem;
    rd= fread (pTable[ind].Name, 1, ch, file); // reading filename
    if (rd< ch) goto file_corrupted;
    HeaderSize+= ch+ 1;
    pTable[ind].Name[ch]= 0;

    rd= fread (Buf, 4, 1, file); // reading attribute
    if (rd< 1) goto file_corrupted;
    rwbuffer (&attr, Buf, 4);
    HeaderSize+= 4;
    pTable[ind].Attr= attr;
    
    rd= fread (Buf, 4, 1, file); // entry length
    if (rd< 1) goto file_corrupted;
    rwbuffer (&pTable[ind].Length, Buf, 4);
    if ((pTable[ind].Length& 0x80000000)!= (ind< NbEntry- 1)* 0x80000000) goto file_corrupted;
    pTable[ind].Length&= 0x7FFFFFFF;
    HeaderSize+= 4;
    
    if (attr & ACC_ZIPPED) 
    { /* read disk length */
      rd= fread (Buf, 4, 1, file);
      if (rd< 1) goto file_corrupted;
      rwbuffer (&pTable[ind].DiskLength, Buf, 4);
      HeaderSize+= 4;
    } else
      pTable[ind].DiskLength= pTable[ind].Length;
      
    ind++;
  }
  
// header loaded, compute the offsets
  offset= 0;
  for (ind= 0; ind< NbEntry; ind++)
   if (!(pTable[ind].Attr & ACC_LINK))
   { pTable[ind].Offset= offset+ HeaderSize;
     offset+= pTable[ind].DiskLength;
   } else
     pTable[ind].Offset= 0;

  CurrentFile= strdup(f);
  return ACC_OK;
  
version_unsupported:
  Done();
  return ACC_NOTSUPPORTED;
   
file_corrupted:
  Done();
  return ACC_FILECORRUPTED;
  
not_enough_mem:
  Done();
  return ACC_NOTENOUGHMEMORY;
}


signed char CACCRes::LoadEntry (unsigned Id)
{ assert (pTable);
  assert (Id< NbEntry);
  char* p;
  unsigned Id2= Id;
  signed long lg, wr, rd;
  char Buf[ACC_IOBUFFER_SIZE];
  
  // if link, load the linked entry instead
  if (pTable[Id].Attr & ACC_LINK)
  { Id= pTable[Id].Offset;
    if (Id>= NbEntry) return ACC_FILECORRUPTED;
    if (pTable[Id].Attr & ACC_LOADED) /* already done */
    { pTable[Id2].Attr|= ACC_LOADED;
      return ACC_OK;
    }
    if (pTable[Id].Attr & ACC_LINK) return ACC_FILECORRUPTED;
  }

  if (pTable[Id].Attr & ACC_LOADED) return ACC_OK;
  if (ACC_GETLOAD(pTable[Id].Attr)== ACC_DONTLOAD) return ACC_WONTLOADENTRY;

  assert (file); 

  if (pTable[Id].Attr & ACC_ZIPPED)
  {
#ifdef ACC_ZLIB_SUPPORT   
    fseek (file, pTable[Id].Offset, SEEK_SET);
    if (ferror(file)) return ACC_FILECORRUPTED;

    signed char res= DecompressFile2Mem (file, pTable[Id].DiskLength, &p, pTable[Id].Length);
    if (res!= ACC_OK) return res;
#else
    return ACC_NOTSUPPORTED;
#endif
  } else
  { p= (char*) malloc (pTable[Id].Length);
    if (!p)
    { Done();
      return ACC_NOTENOUGHMEMORY; 
    }
    
    fseek (file, pTable[Id].Offset, SEEK_SET);
    if (ferror(file)) return ACC_FILECORRUPTED;

    for (lg= pTable[Id].Length, wr= 0; lg> 0; )
     if (lg>= ACC_IOBUFFER_SIZE) 
      { rd= fread (Buf, 1, ACC_IOBUFFER_SIZE, file);
        if (rd< ACC_IOBUFFER_SIZE)
        { free (p);
          return ACC_FILECORRUPTED;
        }
        lg-= rd;
        memcpy (p+ wr, Buf, rd);
        wr+= rd;
      } else  
      { rd= fread (Buf, 1, lg, file);
        if (rd< lg)
        { free (p);
          return ACC_FILECORRUPTED;
        }
        lg= 0;
        memcpy (p+ wr, Buf, rd);
        wr+= rd; 
      }
  }

  pTable[Id].pData= p;
  pTable[Id].Attr|= ACC_LOADED;
  if (Id!= Id2) pTable[Id2].Attr|= ACC_LOADED; /* if ACC_LINK */
  return ACC_OK;
}


signed char CACCRes::LoadACC()
{ unsigned long ind;
  signed char res;

  if (!pTable) return ACC_NOTINITIALIZED;
  
  for (ind= 0; ind<= NbEntry- 1; ind++)    
   if (ACC_GETLOAD(pTable[ind].Attr)== ACC_LOAD)
//    if (!(pTable[ind].Attr & ACC_LINK))
    { res= LoadEntry (ind);
      if (res!= ACC_OK)
       return res;
    }
    
  return ACC_OK;
}


signed char CACCRes::ChangeLoadPolicy (unsigned Id, unsigned attr)
{ if (!pTable)      return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;

  attr&= ACC_POLICYMASK;
  if (attr& ACC_REMOVEFROMMEM)
  { if (pTable[Id].Attr& ACC_LOADED)
    { if (file== NULL) return ACC_ENTRYWOULDBELOST; /* entry is only stored in memory, so it would be lost if removed from there */
      if (pTable[Id].pData!= NULL)
      { free (pTable[Id].pData);
        pTable[Id].pData= NULL;
      }
      pTable[Id].Attr&= ~ACC_LOADED;
    }
    return ACC_OK;
  }
  
  ACC_SETLOAD (pTable[Id].Attr, attr);
  // Note that ACC_LOADED | ACC_DONTLOAD is possible
  return ACC_OK;
}


signed char CACCRes::ChangeAllLoadPolicy (unsigned attr)
{ signed res= 0;
  if (!pTable) return ACC_NOTINITIALIZED;
  
  if (NbEntry> 0)
   for (unsigned i= 0; i< NbEntry; i++)
    res= min(res, ChangeLoadPolicy (i, attr)); // report an error if one of them failed, but still try them all
   
  return res;
}


signed long CACCRes::EntryAttr (unsigned Id)
{ if (!pTable) return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;  
  return pTable[Id].Attr;
}


signed long CACCRes::EntryId (const char* FName)
{ signed long i;

  if (!pTable) return ACC_NOTINITIALIZED;
  for (i= 0; i< (signed long) NbEntry; i++)
   if (!strcmp (pTable[i].Name, FName)) return i; // found
  return ACC_ENTRYNOTFOUND;
}


signed long CACCRes::EntryLinkId (unsigned Id)
{ if (!pTable) return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;
    
  return (pTable[Id].Attr & ACC_LINK)?pTable[Id].Offset:Id;
}



char* CACCRes::EntryName (unsigned Id)
{ if (!pTable || Id>= NbEntry) return NULL;
  return pTable[Id].Name;
}


signed char CACCRes::EntryPtr (unsigned Id, const char** p)
{ signed res= 0;
  
  if (!pTable)      return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;

  if (pTable[Id].Attr & ACC_LINK)
  { Id= pTable[Id].Offset;
    if (Id>= NbEntry) return ACC_FILECORRUPTED;
    if (pTable[Id].Attr & ACC_LINK) return ACC_FILECORRUPTED;
  }
  
  if (!(pTable[Id].Attr & ACC_LOADED))
  { switch (ACC_GETLOAD(pTable[Id].Attr))
    { case ACC_DONTLOAD: 
        return ACC_WONTLOADENTRY;
      case ACC_LOADONDEMAND:
      case ACC_LOAD:
        res= LoadEntry (Id);
        if (res== ACC_OK) *p= pTable[Id].pData;
        return res;
      default: 
        assert (0);
        return ACC_FILECORRUPTED;
    }
  }
    
  *p= pTable[Id].pData;
  return ACC_OK;
}


signed long CACCRes::EntryLength (unsigned Id)
{ if (!pTable)      return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;
    
  if (pTable[Id].Attr & ACC_LINK)
  { Id= pTable[Id].Offset;
    if (Id>= NbEntry) return ACC_FILECORRUPTED;
    if (pTable[Id].Attr & ACC_LINK) return ACC_FILECORRUPTED;
  }
  
  return pTable[Id].Length;
}


signed long CACCRes::EntryOffset (unsigned Id)
{ if (!pTable)      return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;
    
  if (pTable[Id].Attr & ACC_LINK)
  { Id= pTable[Id].Offset;
    if (Id>= NbEntry) return ACC_FILECORRUPTED;
    if (pTable[Id].Attr & ACC_LINK) return ACC_FILECORRUPTED;
  }
  
  return pTable[Id].Offset;
}


signed long CACCRes::EntryDiskLength (unsigned Id)
{ if (!pTable)      return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_INVALIDPARAMETER;

  if (pTable[Id].Attr & ACC_LINK)
  { Id= pTable[Id].Offset;
    if (Id>= NbEntry) return ACC_FILECORRUPTED;
    if (pTable[Id].Attr & ACC_LINK) return ACC_FILECORRUPTED;
  }
  
  return pTable[Id].DiskLength;
}

const char* CACCRes::GetErrorMessage (signed char error_code)
{
  static const char* msg[]= { "Not enough memory",  // ACC_NOTENOUGHMEMORY
                              "Invalid parameter",
                              "Operation not supported",
                              "Entry would be lost",
                              "File not found",
                              "File corrupted",
                              "Entry not found",
                              "Cannot create file",
                              "Disk full",
                              "seek() failed",
                              "read() failed",
                              "write() failed",
                              "Will not load entry in memory",
                              "zlib returned an error",
                              "Not initialized",
                              "Not implemented",
                              NULL};

  switch (error_code) 
  { case ACC_NOTENOUGHMEMORY: return msg[0];
    case ACC_INVALIDPARAMETER: return msg[1];
    case ACC_NOTSUPPORTED: return msg[2];
    case ACC_ENTRYWOULDBELOST: return msg[3];
    case ACC_FILENOTFOUND: return msg[4];
    case ACC_FILECORRUPTED: return msg[5];
    case ACC_ENTRYNOTFOUND: return msg[6];
    case ACC_CANNOTCREATEFILE: return msg[7];
    case ACC_DISKFULL: return msg[8];
    case ACC_SEEKFAILED: return msg[9];
    case ACC_READFAILED: return msg[10];
    case ACC_WRITEFAILED: return msg[11];
    case ACC_WONTLOADENTRY: return msg[12];
    case ACC_ZLIBERROR: return msg[13];
    case ACC_NOTINITIALIZED: return msg[14];
    case ACC_NOTIMPLEMENTED: return msg[15];
    default: return NULL;
  }

  return NULL;
}


FILE* CACCRes::GetFile()
{ return file;
}


char* CACCRes::GetName()
{ return CurrentFile;
}

unsigned long CACCRes::GetNbEntry()
{ return NbEntry;
}

void CACCRes::Done()
{ if (pTable!= NULL)
  { for ( ; NbEntry> 0; NbEntry--)
    { if (pTable[NbEntry- 1].pData!= NULL) free (pTable[NbEntry- 1].pData);
      if (pTable[NbEntry- 1].Name!= NULL)  free (pTable[NbEntry- 1].Name);
    }
    free (pTable);
    pTable= NULL;
  }
  
  NbEntry= 0;
  HeaderSize= 0;
  
  if (CurrentFile!= NULL)
  { free (CurrentFile);
    CurrentFile= NULL;
  }
  
  if (file!= NULL)
  { fclose (file);
    file= NULL;
  }
}


#ifdef ACC_DEBUG
void CACCRes::Dump()
{ for (unsigned long ind= 0; ind< NbEntry; ind++)
    printf ("#%ld (%s): LEN= %ld DSK= %ld ATTR= %ld OFF= %ld DATA= %p\n", ind, 
                      EntryName(ind),
                      EntryLength(ind),
                      EntryDiskLength(ind),
                      EntryAttr(ind),
                      EntryOffset(ind),
                      pTable[ind].pData);
}
#endif


signed char CACCRes::MoveFileAt (unsigned Id)
{
  if (file== NULL) return ACC_NOTINITIALIZED;
  if (fseek(file, pTable[Id].Offset, SEEK_SET) ) return ACC_SEEKFAILED;
  return ACC_OK;
}


/*  *****************************************************************  */
// CACCEditMem
/*  *****************************************************************  */
#ifndef ACC_NOACCEditMem
CACCEditMem::CACCEditMem()
{ CurTabSize= 0;
}

void CACCEditMem::Done()
{
  CACCRes::Done();
  CurTabSize= 0;
}


void CACCEditMem::SetName (const char* name)
{
  if (CurrentFile!= NULL) free (CurrentFile);
  CurrentFile= strdup (name);
}


signed char CACCEditMem::AddEntry (const char* f, unsigned long Pos, unsigned long Attr, unsigned long* data)
{ unsigned long rd, size, Len, L, ind;
  char* FName;
  char* p= NULL;
  signed char res;
  FILE* f_in= NULL;
  
  if (Attr& ACC_REMOVEFROMMEM) return ACC_INVALIDPARAMETER;
  if (!(Attr& ACC_LINK))
  { if ((f_in= fopen (f, "rb"))== NULL) return ACC_FILENOTFOUND;
    fseek (f_in, 0, SEEK_END);
    size= Len= ftell (f_in);
    fseek (f_in, 0, SEEK_SET);
  } else
  { /* check if data is correct */
    if (data== NULL) return ACC_INVALIDPARAMETER;
    unsigned long* id= (unsigned long*) data;
    if (*id>= NbEntry) return ACC_INVALIDPARAMETER;
    signed long target_attr= EntryAttr (*id);
    if (target_attr< 0 || target_attr & ACC_LINK) return ACC_INVALIDPARAMETER;
    if (Pos<= *id) (*id)++;
     
    size= Len= 0; // not used for links
  }

// Remove the path from f
  FName= fnsplit (f);
  if (FName== NULL) return ACC_INVALIDPARAMETER;

// is pTable too small ?
  if (NbEntry* sizeof(CACCEntry)== CurTabSize)
  { if (ChangeMemSize ((void*&) pTable, CurTabSize+ 16* sizeof(CACCEntry))< 0)
     return ACC_NOTENOUGHMEMORY;
    CurTabSize+= 16* sizeof(CACCEntry);
  }

  if (!(Attr& ACC_LINK))
  { // load in memory
    char* p2;
    if ( (p= (char*) malloc (size))== NULL) 
    { res= ACC_NOTENOUGHMEMORY;
      goto error;
    }
    p2= p;
    while (Len> 0)
    { L= min(Len, ACC_IOBUFFER_SIZE);
      rd= fread (p2, 1, L, f_in);
      if (rd< L) 
      { free (p);
        res= ACC_READFAILED;
        goto error;
      }
      p2+= L;
      Len-= L;
    }
  }
  
// Table update
  if (Pos> NbEntry) Pos= NbEntry;
  for (ind= NbEntry; ind> Pos; ind--) 
  { pTable[ind]= pTable[ind- 1];
    if (pTable[ind].Attr & ACC_LINK) 
     if (pTable[ind].Offset>= Pos)
      pTable[ind].Offset++; // update the link target if necessary
  }
  NbEntry++;

  pTable[Pos].Name= FName;
  pTable[Pos].Length= (Attr& ACC_LINK)?0:size;
  pTable[Pos].DiskLength= (Attr& ACC_ZIPPED)?0:pTable[Pos].Length; /* DiskLength is unknown if ACC_ZIPPED */
  pTable[Pos].Offset= (Attr& ACC_LINK)?*((long*)data):0; /* we are sure that data!= NULL */
  pTable[Pos].Attr= Attr | ACC_LOADED;
  pTable[Pos].pData= (Attr& ACC_LINK)?NULL:p;
  res= ACC_OK;
  goto end;
  
error:
  if (FName) free (FName);
end:  
  if (f_in!= NULL) fclose (f_in);
  return res;
}


signed char CACCEditMem::DelEntry (unsigned Id)
{ unsigned i, j, ind;
  
  if (!NbEntry) return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_ENTRYNOTFOUND;

// update links pointing to Id' != Id
  for (i= 0; i< NbEntry; i++)
   if (i!= Id && (pTable[i].Attr & ACC_LINK) && pTable[i].Offset!= Id)
   { for (j= ind= 0; j< pTable[i].Offset; j++)
      if (j== Id || ((pTable[j].Attr & ACC_LINK)&& pTable[j].Offset== Id)) 
       ind++;
     pTable[i].Offset-= ind;
   }
  
// remove entry Id and all links pointing to this entry
// i is the old index, ind the new index
  for (i= ind= 0; i< NbEntry; i++)
   if (i== Id || ((pTable[i].Attr & ACC_LINK)&& pTable[i].Offset== Id))
   { if (pTable[i].Name!= NULL) free (pTable[i].Name);
     if (pTable[i].pData!= NULL) free (pTable[i].pData);
   } else
   { pTable[ind++]= pTable[i];
   }
  NbEntry-= i- ind;

// if pTable is small enough, we resize it
  if (NbEntry* sizeof(CACCEntry)< CurTabSize/ 2)
   if (ChangeMemSize ( (void*&) pTable, CurTabSize/ 2)== ACC_OK)
    CurTabSize/= 2;

  return ACC_OK;
}


signed char CACCEditMem::ExtractEntry (unsigned Id, const char* f)
{ unsigned long Len, L;
  signed char res;
  FILE* f_out;
  size_t wr;
  const char* p;  

  if (!NbEntry) return ACC_NOTINITIALIZED;
  if (Id>= NbEntry) return ACC_ENTRYNOTFOUND;
  if (!f) f= pTable[Id].Name;
    
  if (pTable[Id].Attr & ACC_LINK)
  { Id= pTable[Id].Offset;
    if (Id>= NbEntry) return ACC_FILECORRUPTED;
    if (pTable[Id].Attr & ACC_LINK) return ACC_FILECORRUPTED;
  }
  res= EntryPtr (Id, &p);
  if (res!= ACC_OK) return res;
  Len= EntryLength (Id);

  if ((f_out= fopen (f, "wb"))== NULL) return ACC_CANNOTCREATEFILE;

  while (Len> 0)
  { L= min(Len, ACC_IOBUFFER_SIZE);
    wr= fwrite (p, 1, L, f_out);
    if (wr< L)
    { fclose (f_out);
      return ACC_WRITEFAILED;
    }
    p+= L;
    Len-= L;
  }

  fclose (f_out);
  return ACC_OK;
}


signed char CACCEditMem::InitACC (const char* f)
{ signed char res;

  res= CACCRes::InitACC (f, 1); /* 1= open read/write instead of read-only */
  CurTabSize= (res== ACC_OK)?NbEntry* sizeof(CACCEntry):0;
  return res;
}


signed char CACCRes::WriteHeader (FILE* F)
{ unsigned long Attr, Len, L;
  unsigned ind;
  unsigned char c;
  
  rwbuffer (&L, &acc_header, sizeof(L));
  if (fwrite (&L, sizeof(L), 1, F)< 1) return ACC_DISKFULL;
    
  L= NbEntry & 0x00FFFFFF;
  L= NbEntry | 0x03000000; // version
  rwbuffer (&Len, &L, sizeof(L));
  if (fwrite (&Len, sizeof(L), 1, F)< 1) return ACC_DISKFULL;
  HeaderSize= 8;

  for (ind= 0; ind< NbEntry; ind++)
  {
    c= strlen (pTable[ind].Name); 
    if (fwrite (&c, 1, 1, F)< 1) return ACC_DISKFULL;
    if (fwrite (pTable[ind].Name, 1, c, F)< c) return ACC_DISKFULL;
    HeaderSize+= 1+ c;
    
    Attr= EntryAttr (ind) & ~ACC_LOADED;
    rwbuffer (&L, &Attr, sizeof(Attr));
    if (fwrite (&L, sizeof(Attr), 1, F)< 1) return ACC_DISKFULL;
    HeaderSize+= sizeof(Attr);
    
    Len= (EntryLength (ind)& 0x7FFFFFFFUL) | (0x80000000UL* (ind!= NbEntry- 1));
    rwbuffer (&L, &Len, sizeof(Len));
    if (fwrite (&L, sizeof(Len), 1, F)< 1) return ACC_DISKFULL; // unzipped length
    HeaderSize+= sizeof(Len);
    
    if (Attr & ACC_ZIPPED)
    { 
#ifdef ACC_ZLIB_SUPPORT      
      rwbuffer (&L, &pTable[ind].DiskLength, 4);
      if (fwrite (&L, 4, 1, F)< 1) return ACC_DISKFULL;
      HeaderSize+= 4;
#else
      return ACC_NOTSUPPORTED;
#endif
    }
  }
  
  return ACC_OK;
}


static int CompareNames (const char* n1, const char* n2)
// n1 and n2 must be non-NULL
{ char* name1= fnsplit (n1);
  char* name2= fnsplit (n2);
  int res= strcmp (name1, name2);

  if (name1!= NULL) free (name1);
  if (name2!= NULL) free (name2);
  return res;
}

/*
static signed char CopyFile (FILE* src, FILE* dest, unsigned long len)
{ unsigned l;
  size_t r;
  unsigned char Buf[ACC_IOBUFFER_SIZE];

  while (len> 0)
  { l= min (len, ACC_IOBUFFER_SIZE);
    r= fread (Buf, 1, l, src);
    if (r< l) return ACC_READFAILED;
    r= fwrite (Buf, 1, l, dest);
    if (r< l) return ACC_WRITEFAILED;
    len-= l;
  }

  return ACC_OK;
}*/


signed char CACCEditMem::LoadACCInMemory()
{
  unsigned long ind;
  signed char res;

  if (!pTable) return ACC_NOTINITIALIZED;

  for (ind= 0; ind<= NbEntry- 1; ind++)  
  {
    signed long save_load= ACC_GETLOAD(pTable[ind].Attr);
    ACC_SETLOAD (pTable[ind].Attr, ACC_LOAD);
    res= LoadEntry (ind);
    if (res!= ACC_OK) return res;
    ACC_SETLOAD (pTable[ind].Attr, save_load);
  }

  return ACC_OK;
}


signed char CACCEditMem::SaveACC (const char* f)
// Known bug: will fail if f is a symlink to CurrentFile !
// in this version, all entries must be loaded in memory
{ const char* p;
  unsigned long Len, L;
  size_t wr;
  unsigned ind;
  signed char res;
  FILE* F_final;
  FILE* F_old;

  if (!pTable || NbEntry== 0) return ACC_NOTINITIALIZED;

  if (f== NULL && CurrentFile== NULL)
  {
    // we need a filename and there is no current one
    return ACC_NOTINITIALIZED;
  }
  
  F_old= file;
  if (CurrentFile!= NULL)
   if (f!= NULL)
    if (CompareNames (f, CurrentFile)== 0)
    { F_final= file;
    }
    else
    {
      F_final= fopen (f, "w+b");
      if (F_final== NULL) return ACC_CANNOTCREATEFILE;
    }
   else
   {
     F_final= file;
   }
  else
  {
    F_final= fopen (f, "w+b");
    if (F_final== NULL) return ACC_CANNOTCREATEFILE;   
  }
  
// Write the header
  fseek (F_final, 0, SEEK_SET);  
  res= WriteHeader (F_final); // disk lengths may be incorrect at this point
  if (res!= ACC_OK) goto end;
  
  for (ind= 0; ind< NbEntry; ind++)
  { if (EntryAttr(ind) & ACC_LINK) continue;
    Len= EntryLength (ind);    
    if (EntryAttr(ind) & ACC_LOADED) // that should always be the case
    { // resource is in memory
      res= EntryPtr (ind, &p);
      assert (res== ACC_OK);
      if (res!= ACC_OK || (signed long) Len== ACC_NOTINITIALIZED || (signed long) Len== ACC_INVALIDPARAMETER)
      { res= ACC_FILECORRUPTED;
        goto end;
      }

      if (EntryAttr(ind) & ACC_ZIPPED)
      { // compress and write to disk
#ifdef ACC_ZLIB_SUPPORT
        res= CompressMem2File (p, Len, F_final, &(pTable[ind].DiskLength));
        if (res!= ACC_OK) goto end;
#else
        res= ACC_NOTSUPPORTED;
        goto end;
#endif
      } else
      { while (Len> 0)
        { L= min(Len, ACC_IOBUFFER_SIZE);
          wr= fwrite (p, 1, L, F_final);
          if (wr< L) goto disk_full; // or permission denied
          p+= L;
          Len-= L;
        }
      }
    }
    else
    { 
      // entry was not loaded, so cannot save
      // warning, file created is corrupted
      // that means the original is corrupted if f= NULL !
      return ACC_FILECORRUPTED;
    }
  }
  
// update disk lengths
  unsigned long offset;
  for (ind= 0, offset= 8; ind< NbEntry; ind++)
  { offset+= strlen(pTable[ind].Name)+ 1+ 4+ 4; // name+ name len+ attr+ len
    if (pTable[ind].Attr & ACC_ZIPPED)
    { 
      fseek (F_final, offset, SEEK_SET);
      if (ferror(F_final)) 
      { res= ACC_SEEKFAILED;
        goto end;
      }

      rwbuffer (&L, &pTable[ind].DiskLength, 4);
      if (fwrite (&L, 4, 1, F_final)< 1) goto disk_full;
      offset+= 4;
    }
  }

// recompute offsets
  for (ind= 0, offset= 0; ind< NbEntry; ind++)
  { if (!(pTable[ind].Attr & ACC_LINK))
    { pTable[ind].Offset= offset+ HeaderSize;
      offset+= pTable[ind].DiskLength;
    } else
    pTable[ind].Offset= 0;
  }
  
// update file
  if (file!= NULL)
   if (file!= F_final)
   { fclose (file);
     file= F_final;
   }
   else
   { 
#ifndef WIN32
       ftruncate (fileno(file), offset+ HeaderSize);
#else
       // NOT YET TESTED!!!!!
       _chsize(file->_file , offset+ HeaderSize);
#endif

   }
  else
  {
    file= F_final;
  }
  
// update CurrentFile
  if (f!= NULL)
  {
    if (CurrentFile!= NULL) free (CurrentFile);
    CurrentFile= strdup (f);
  }

  res= ACC_OK;
  goto end;

disk_full:
  res= ACC_DISKFULL;
end:

  if (F_final!= file) fclose (F_final);
  return res;
}
#endif

