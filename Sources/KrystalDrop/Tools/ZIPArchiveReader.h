#ifndef ZIPArchiveReader_H
#define ZIPArchiveReader_H

#include <map>
#include <string>

#include "../../KDpp/Resources/ArchiveReader.h"

class KD_ZIPArchiveReader: public KD_ArchiveReader
{ protected:

  typedef struct
  { unsigned long disk_size;
    unsigned long disk_offset; // absolute offset
    unsigned long mem_size;
    const char* ptr;
    unsigned char method;
  } T_ZipEntry;
  std::map<std::string,T_ZipEntry> zip_index;
  
  std::string archive_name;

  public:
    bool LoadArchive(std::string archive_filename);
    virtual ~KD_ZIPArchiveReader();
  
    bool LoadResource(std::string name, const char** p_data, unsigned long* p_size);
    bool UnloadResource(std::string name);
    std::string GetArchiveName();
};

/** The KD_ZIPArchiveReader factory. */
KD_ArchiveReader* CreateZIPArchiveReader();


#endif
