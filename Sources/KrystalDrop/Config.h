#ifndef Config_H
#define Config_H

#include <string>

using namespace std;

class KD_Config
{ protected:
//   string config_filename;
   static KD_Config* config;
    bool AssignVar (const char* var, const char* val);
    bool TryConfigurationFile (char* f);

  public:
   unsigned short p1up_t;
   unsigned short p1up;
   unsigned short p1down_t;
   unsigned short p1down;
   unsigned short p1left_t;
   unsigned short p1left;
   unsigned short p1right_t;
   unsigned short p1right;
   unsigned short p1xtra_t;
   unsigned short p1xtra;
   unsigned short p2up_t;
   unsigned short p2up;
   unsigned short p2down_t;
   unsigned short p2down;
   unsigned short p2left_t;
   unsigned short p2left;
   unsigned short p2right_t;
   unsigned short p2right;
   unsigned short p2xtra_t;
   unsigned short p2xtra;
   char* art;
   char* music;
   bool opengl;
   bool sound;
   bool fullscreen;
/* ## volume */   

       KD_Config();
      ~KD_Config();
  void ReadConfiguration();
static KD_Config* GetConfig();
};

#endif
