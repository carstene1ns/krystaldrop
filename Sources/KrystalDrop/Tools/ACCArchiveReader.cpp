#include "ACCArchiveReader.h"

bool KD_ACCArchiveReader::LoadArchive (std::string archive_filename)
{
  if (acc.InitACC (archive_filename.c_str())!= ACC_OK) return false;
  return true;
}

KD_ACCArchiveReader::~KD_ACCArchiveReader()
{
  acc.Done();
}

bool KD_ACCArchiveReader::LoadResource(std::string name, const char** p_data, unsigned long* p_size)
{
  signed long id= acc.EntryId(name.c_str());
  if (id< 0) return false;

  acc.ChangeLoadPolicy (id, ACC_LOAD); // just to be sure
  if (acc.LoadEntry (id)!= ACC_OK) return false;
  if (acc.EntryPtr (id, p_data)) return false; // not freeing the entry here is not a problem
  *p_size= acc.EntryLength(id);
  
  return true;
}

bool KD_ACCArchiveReader::UnloadResource(std::string name)
{
  signed long id= acc.EntryId(name.c_str());
  if (id< 0) return false;

  return acc.ChangeLoadPolicy (id, ACC_REMOVEFROMMEM);
}

std::string KD_ACCArchiveReader::GetArchiveName()
{
  std::string name= acc.GetName();
  return name;
}


KD_ArchiveReader* CreateACCArchiveReader()
{
  return new KD_ACCArchiveReader;
}
