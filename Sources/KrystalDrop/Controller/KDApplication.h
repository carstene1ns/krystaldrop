#ifndef KDApplication_H
#define KDApplication_H

#include "../../KDpp/Controller/Application.h"
#include "../../KDpp/Tools/defines.h"

class DllExport KD_KDApplication: public KD_Application
{ private:
    /** Copy of the art directory configuration item ; this value is often
        used. A slash character '/' is automatically appended at the end
        if necessary.
    */
    static string art_directory;

  public:
    static KD_KDApplication* GetApplication();

    /** Get the art directory from the XML configuration file */
    string& GetArtDirectory() const;

    /** Add the art directory name before the string resource_file.
        Has to be called after InitFromConfigFile().
    */
    string GetArtFile (char* resource_file) const;

    /** Call config->GetConfigurationString() */
    string GetConfigurationString (string config_item);

    virtual bool InitFromConfigObject(KD_XMLConfig *config);
};

#endif
