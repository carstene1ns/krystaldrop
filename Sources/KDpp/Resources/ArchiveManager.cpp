#include "ArchiveManager.h"
#include "../Tools/Logfile.h"

#ifndef _WIN32
#include <ctype.h>
#endif

std::map<std::string,KD_ArchiveReader*> KD_ArchiveManager::opened_archives;
std::map<std::string,T_ArchiveReaderFactory> KD_ArchiveManager::known_suffixes;

KD_ArchiveManager::~KD_ArchiveManager()
{
  /* close every opened archives */
  T_ArchiveIterator p= opened_archives.begin();
  while (p!= opened_archives.end())
  {
#ifdef DEBUG
    KD_LogFile::printf2("Archive %s has not been closed. Closing it.\n", p->first.c_str());
#endif
    delete (*p++).second;
  }

  opened_archives.clear();
}


void KD_ArchiveManager::RegisterArchiveFormat (std::string suffix, T_ArchiveReaderFactory reader_factory)
{
  NormalizeSuffix (suffix);
  assert (reader_factory!= NULL);
  assert (known_suffixes[suffix]== NULL);
  known_suffixes[suffix]= reader_factory;
}


void KD_ArchiveManager::NormalizeSuffix (std::string& suffix)
{
  // stores the suffix lower-case
  transform (suffix.begin(), suffix.end(), suffix.begin(), tolower);

   // add the dot character `.' if it is missing
  if (suffix[0]!= '.') suffix= '.'+ suffix;
}


bool KD_ArchiveManager::OpenArchive (std::string archive_filename)
{
  /* check if already loaded */
  if (opened_archives[archive_filename]!= NULL)
    return true;

  unsigned pos= archive_filename.find (".");
  std::string suffix= archive_filename.substr (pos);
  NormalizeSuffix (suffix);

  T_ArchiveReaderFactory factory= known_suffixes[suffix];
  assert (factory!= NULL);
  if (factory== NULL) return false;

  KD_ArchiveReader* p= (*factory)();
  if (p== NULL) return false;
  if (p->LoadArchive(archive_filename)!= true)
  {
    delete p;
    return false;
  }

  opened_archives[archive_filename]= p;
  return true;
}


bool KD_ArchiveManager::CloseArchive (std::string archive_filename)
{
  KD_ArchiveReader* p= opened_archives[archive_filename];
  if (p== NULL) return false;

  delete p;
  opened_archives.erase (archive_filename);
  return true;
}


bool KD_ArchiveManager::FetchResource (std::string archive_name, std::string resource_name, const char** p_data, unsigned long* p_size)
{
  KD_ArchiveReader* p= opened_archives[archive_name];
  if (p== NULL)
  {
    if (OpenArchive(archive_name)== false) return false;
    p= opened_archives[archive_name];
  }

  return p->LoadResource (resource_name, p_data, p_size);
}


bool KD_ArchiveManager::FreeResource (std::string archive_name, std::string resource_name)
{
  KD_ArchiveReader* p= opened_archives[archive_name];
  if (p== NULL) return false;

  return p->UnloadResource (resource_name);
}
