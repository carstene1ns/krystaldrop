#ifndef ArchiveReader_H
#define ArchiveReader_H

#include <map>
#include <string>

/** \c KD_ArchiveReader is a generic abstract class which reads a specific kind of archive
    (`.zip' for instance)
    One \c KD_ArchiveReader instance will be associated to one file.
*/
class KD_ArchiveReader
{
  public:
    /** Loads the header of an archive. */
    virtual bool LoadArchive(std::string archive_filename)= 0;
    /** Loads a resource into memory. Returns a pointer to the memory allocated and its size.*/
    virtual bool LoadResource(std::string name, const char** p_data, unsigned long* p_size)= 0;
    /** Frees the memory allocated with \c LoadResource(). */
    virtual bool UnloadResource(std::string name)= 0;
    /** Returns the name of the archive after \c LoadArchive() has been called. */
    virtual std::string GetArchiveName()= 0;
};

#endif
