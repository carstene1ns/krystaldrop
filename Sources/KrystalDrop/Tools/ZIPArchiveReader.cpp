#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include <map>
#include <string>

#include "ZIPArchiveReader.h"

/* PKZIP header definitions */
#define ZIPMAG 0x4b50           /* two-byte zip lead-in */
#define LOCREM 0x0403           /* remaining two bytes in zip signature */
#define LOCLIS 0x0201           /* remaining two bytes in zip listing */
#define LOCSIG 0x04034b50L      /* full signature */
#define LOCFLG 4                /* offset of bit flag */
#define  CRPFLG 1               /*  bit for encrypted entry */
#define  EXTFLG 8               /*  bit for extended local header */
#define LOCHOW 6                /* offset of compression method */
#define LOCTIM 8                /* file mod time (for decryption) */
#define LOCCRC 12               /* offset of crc */
#define LOCSIZ 16               /* offset of compressed size */
#define LOCLEN 20               /* offset of uncompressed length */
#define LOCFIL 24               /* offset of file name field length */
#define LOCEXT 26               /* offset of extra field length */
#define LOCHDR 28               /* size of local header, including LOCREM */
#define EXTHDR 16               /* size of extended local header, inc sig */

#define STORED            0 
#define DEFLATED          8
#define ENHDEFLATED       9

/* Macros for getting two-byte and four-byte header values */
#define SWAP2(p) ((unsigned long)((p)[0]) | ((unsigned long)((p)[1]) << 8))
#define SWAP4(p) ((unsigned long)(SWAP2(p)) | ((unsigned long)(SWAP2((p)+2)) << 16))


/* DecompressFile2Mem is a modified version of the one from direct.cpp */
static signed char DecompressFile2Mem (FILE* f_in, unsigned long compr_size, char** p, unsigned long size)
/* read (compr_size) bytes from f_in, allocates (size) bytes for p and expects to decompress (size) bytes */
{ struct z_stream_s zs;
  #define Z_IOBUFFER_SIZE 1024
  char io_buf[Z_IOBUFFER_SIZE];  
  size_t rd;
  int err;
  int res;
  char end= 0;
  char end_input= 0;
  assert (f_in);

  *p= (char*) malloc (size);
  if (*p== NULL) return Z_MEM_ERROR;

  zs.zalloc= Z_NULL;
  zs.zfree=  Z_NULL;
  zs.opaque= Z_NULL;
  zs.next_in=  (Bytef*) io_buf;
  zs.next_out= (Bytef*) *p;
  zs.avail_in= 0;  
  zs.avail_out= size;

  res= inflateInit2(&zs, -15); // 15 is the window bit width, < 0 is for no header
  if (res== Z_MEM_ERROR) { free (*p); return Z_MEM_ERROR; }
  if (res!= Z_OK) { free (*p); return Z_BUF_ERROR; } /* should never happen */  

  while (!end)
  { if (zs.avail_in== 0 && end_input== 0)
    { rd= fread (io_buf, 1, Z_IOBUFFER_SIZE, f_in);
      if (ferror(f_in)!= 0) 
      { err= Z_STREAM_ERROR;
        goto decompress_error;
      }
      if (rd< Z_IOBUFFER_SIZE) end_input= 1;
      zs.avail_in= rd;
      zs.next_in= (Byte*) io_buf;
    }

    res= inflate (&zs, Z_SYNC_FLUSH);
    if (res== Z_STREAM_END) end= 1;
      else if (res!= Z_OK) 
    { err= Z_BUF_ERROR; /* should never happen */
      goto decompress_error;
    }
  }

  assert (size== zs.total_out);
  res= inflateEnd (&zs);
  if (res!= Z_OK) return Z_BUF_ERROR; /* should never happen */ 
  return Z_OK;

decompress_error:
  inflateEnd (&zs);
  free (*p);
  return err;
}


bool KD_ZIPArchiveReader::LoadArchive(std::string archive_filename)
{
  FILE* in;
  int n;
  int err= 0;
  int method;
  unsigned char h[LOCHDR];
  char* filename= NULL;
  std::string s_filename;

  /* open file */
  if ((in= fopen(archive_filename.c_str(), "rb")) == (FILE*)NULL)
    return Z_STREAM_ERROR;

  while (!feof(in))
  {
    /* read local header */
    n = getc(in); n|= getc(in) << 8;
    if (n== ZIPMAG)
    { if (fread((char*)h, 1, LOCHDR, in) != LOCHDR || (SWAP2(h) != LOCREM && SWAP2(h) != LOCLIS))
      { err= Z_VERSION_ERROR; goto error; }
      
      if (SWAP2(h)== LOCLIS) break; /* no more entries */
      
      method = SWAP2(h + LOCHOW);
      if (method!= STORED && method!= DEFLATED)
      { err= Z_VERSION_ERROR; goto error; }
    }
    else /* no gzip support or unknown file format */
    { err= Z_VERSION_ERROR; goto error; }    
  
    /* read the filename */
    filename= (char*) malloc(SWAP2(h+ LOCFIL)+ 1);
    if (filename== NULL)
    { err= Z_MEM_ERROR; goto error; }
    if (fread (filename, 1, SWAP2(h+LOCFIL), in)!= SWAP2(h+LOCFIL))
    { err= Z_STREAM_ERROR; goto error; }
    filename[SWAP2(h+LOCFIL)]= 0;
    s_filename= filename;
    free (filename);
    filename= NULL;
    
    /* ignore extra fields */
    for (n= SWAP2(h + LOCEXT); n--; getc(in));

    /* fill the index */
    zip_index[s_filename].disk_size= SWAP4(h+LOCSIZ);
    zip_index[s_filename].mem_size= SWAP4(h+LOCLEN);
    zip_index[s_filename].disk_offset= ftell(in);
    zip_index[s_filename].method= method;    
    zip_index[s_filename].ptr= NULL;

    /* skip actual content */
    fseek (in, zip_index[s_filename].disk_size, SEEK_CUR);

    /* ignore extended header */
    if ((h[LOCFLG] & EXTFLG))
      for (n= EXTHDR; n--; getc(in));
  }

  archive_name= archive_filename;
  
  error:
  if (filename!= NULL) free (filename);
  if (in!= NULL) fclose (in);

  return (err== 0);
}


KD_ZIPArchiveReader::~KD_ZIPArchiveReader()
{
}


bool KD_ZIPArchiveReader::LoadResource(std::string name, const char** p_data, unsigned long* p_size)
{
  if (zip_index[name].disk_offset== 0) return false;
    
  *p_size= zip_index[name].mem_size;    
  if (zip_index[name].ptr!= NULL)
  {
    *p_data= zip_index[name].ptr;
    return true;
  }

  FILE* f= fopen (archive_name.c_str(), "rb");
  if (f== NULL) return false;
  fseek (f, zip_index[name].disk_offset, SEEK_SET);

  bool res= false;
  if (zip_index[name].method== DEFLATED)
    res= DecompressFile2Mem (f, zip_index[name].disk_size, (char**) p_data, *p_size)== Z_OK;
  else
  if (zip_index[name].method== STORED)
  {
    *p_data= (char*) malloc (*p_size);
    if (*p_data== NULL) return false;
    res= fread ((char**)*p_data, 1, *p_size, f)> 0;
  }
  
  zip_index[name].ptr= *p_data;
  fclose (f);
  return res;
}


bool KD_ZIPArchiveReader::UnloadResource(std::string name)
{
  if (zip_index[name].ptr== NULL) return false;
  free ((void*)zip_index[name].ptr);
  zip_index[name].ptr= NULL;
  return true;
}


std::string KD_ZIPArchiveReader::GetArchiveName()
{ return archive_name;
}


KD_ArchiveReader* CreateZIPArchiveReader()
{
  return new KD_ZIPArchiveReader;
}
