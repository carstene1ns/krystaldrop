#ifndef ACCArchiveReader_H
#define ACCArchiveReader_H

#ifndef ACC_ZLIB_SUPPORT
#define ACC_ZLIB_SUPPORT
#endif

#include "direct.h"
#include "../../KDpp/Resources/ArchiveReader.h"

/** \c KD_ACCArchiveReader implements the abstract class \c KD_ArchiveReader, and is
       a wrapper which reads the `.acc' archive file format. */
class KD_ACCArchiveReader: public KD_ArchiveReader
{
  protected:
    CACCRes acc;
  public:
    virtual bool LoadArchive(std::string archive_filename);
    virtual ~KD_ACCArchiveReader();

    virtual bool LoadResource(std::string name, const char** p_data, unsigned long* p_size);
    virtual bool UnloadResource(std::string name);
    virtual std::string GetArchiveName();
};

/** The KD_ACCArchiveReader factory. */
KD_ArchiveReader* CreateACCArchiveReader();

#endif
