#ifndef KDApplication_H
#define KDApplication_H

#include "../../KDpp/Controller/Application.h"
#include "../../KDpp/Tools/defines.h"

class DllExport KD_KDApplication: public KD_Application
{ private:
    static string art_directory; // a '/' is automatically appended at the end if necessary
  
  
  public:
    static KD_KDApplication *GetApplication();

 	/** 
    Add the art directory name before the string resource_file
    Has to be called after InitFromConfigFile()
  */
    static string GetArtFile (char* resource_file);
    static string GetArtDirectory();
  
    virtual bool InitFromConfigObject(KD_XMLConfig *config);
};

#endif
