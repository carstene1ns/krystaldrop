#ifndef ArchiveManager_H
#define ArchiveManager_H

#include <map>
#include <string>

#include "ArchiveReader.h"

typedef KD_ArchiveReader*(*T_ArchiveReaderFactory)();
typedef std::map<std::string,KD_ArchiveReader*>::iterator T_ArchiveIterator;

/** \c KD_ArchiveManager stores a list of archive readers, one for each opened archive
*/
class KD_ArchiveManager
{
  protected:
  /** Opened archives and their corresponding \c KD_ArchiveReader object */
    static std::map<std::string,KD_ArchiveReader*> opened_archives;
  
  /** \c NormalizeSuffix() puts the suffix string into lower-case and adds a dot if necessary
  */
    static void NormalizeSuffix (std::string& suffix);

  public:
  
  /**
    The following list stores the known suffixes for archive files.
  By default, it contains nothing. \c RegisterArchiveFormat("bar",XXX)
  must be called to recognize `foo.bar' as an archive of type `bar'. \c XXX is
  a \c T_ArchiveReaderFactory function which creates a class derived from
  \c KD_ArchiveReader (which implements the actual reading of the archive).
  The suffix is case insensitive, and the prefixing dot `.' is optional. */
    static std::map<std::string,T_ArchiveReaderFactory> known_suffixes;

   
  /**
    Add \c suffix in the known archive suffixes list (duplicates are not removed), and
    associate it with a \c ArchiveReaderFactory, which is a function which allocates
    a specific instance of a derived class of \c KD_ArchiveReader.
  */
    static void RegisterArchiveFormat (std::string suffix, T_ArchiveReaderFactory reader_factory);

  public:
  /**
    Closes every archive left open. Print a warning message in that case,
    in \c DEBUG mode.
  */
   ~KD_ArchiveManager();

  /**
    \c OpenArchive() will not open the archive if it has already open.
       Returns \c true if successful.
  */
    static bool OpenArchive (std::string archive_filename);

  /**
    Returns \c true if successful.
  */
    static bool CloseArchive (std::string archive_filename);

  /** 
    \c resource_name is the name of the resource as stored inside the archive.
    The archive is opened if it has not been opened yet.
    
    The advantage of calling \c OpenArchive() explicitly is that you can control
    when the archive header will be read (this can possibly take a lot of time
    for some archives). Returns \c true if successful.
  */
    static bool FetchResource (std::string archive_name, std::string resource_name, const char** p_data, unsigned long* p_size);

  /** Free the resource allocated by \c FetchResource().
  */
    static bool FreeResource (std::string archive_name, std::string resource_name);    
};

#endif
