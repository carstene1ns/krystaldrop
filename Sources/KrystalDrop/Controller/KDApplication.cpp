#include "../global.h"

#include "KDApplication.h"
#include "../../KDpp/Tools/XMLConfig.h"

string KD_KDApplication::art_directory= "";

KD_KDApplication* KD_KDApplication::GetApplication()
{
    if (GetInternalApplication()== NULL)
        SetInternalApplication(new KD_KDApplication());
  return (KD_KDApplication*) GetInternalApplication();
}

string& KD_KDApplication::GetArtDirectory() const
{
    return art_directory;
}

string KD_KDApplication::GetArtFile (char* resource_file) const
{
    string art_file(resource_file);
    art_file= art_directory+ art_file;

    return art_file;
}

string KD_KDApplication::GetConfigurationString(string config_item)
{
    assert (GetConfigFile());
    return GetConfigFile()->GetConfigurationString(config_item);
}

bool KD_KDApplication::InitFromConfigObject(KD_XMLConfig *config)
{
    bool res= KD_Application::InitFromConfigObject (config);
    if (res== true)
    {
        art_directory = GetConfigurationString ((string)"art/base_directory");
        if (art_directory[art_directory.size()]!= '/')
            art_directory = art_directory + '/';
    }

    return res;
}
