#include "KDApplication.h"
#include "../../KDpp/Tools/XMLConfig.h"

string KD_KDApplication::art_directory= "";


KD_KDApplication* KD_KDApplication::GetApplication()
{
	if (getInternalApplication()== NULL) 
      setInternalApplication(new KD_KDApplication()); // implicit cast from KD_KDApplication to KD_Application
  return (KD_KDApplication*) getInternalApplication();
}


string KD_KDApplication::GetArtDirectory()
{ return KD_KDApplication::art_directory;
}


string KD_KDApplication::GetArtFile (char* resource_file)
{
  string art_file(resource_file);
  art_file= art_directory+ art_file;

  return art_file;
}


bool KD_KDApplication::InitFromConfigObject(KD_XMLConfig *config)
{
  bool res= KD_Application::InitFromConfigObject (config);
  if (res== true) art_directory= config->GetArtDirectory();
  return res;
}
