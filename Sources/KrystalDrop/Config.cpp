/* this file is more and more obsolete as time passes */

#include <assert.h>
#include <fstream>
#include <string>
#include <stdio.h>

#include "Config.h"

#ifdef WIN32
#define snprintf _snprintf
#define strcasecmp _strcmpi
#endif

static char* default_dir=  "art/";
string line;
string var;
string val;
char status= 0;
char* d;
unsigned line_count;

#define KD_NB_VARNAMES 15
#define KD_CONFIG_LAST_BOOL 3 
/* last variable to accept boolean values */
/* after that, values must be strings */
#define KD_CONFIG_LAST_DIR 5
/* last variable to accept strings */
/* after that, values must be "int,int" */
static char* var_names[KD_NB_VARNAMES]= 
    { "opengl", "sound", "fullscreen",
      "art", "music",
      "p1up", "p1down", "p1left", "p1right", "p1xtra",
      "p2up", "p2down", "p2left", "p2right", "p2xtra"
    };

/* ***** */
KD_Config* KD_Config::config= NULL;

KD_Config* KD_Config::GetConfig()
{ if (config== NULL) config= new KD_Config;
  return config;
}

KD_Config::KD_Config()
{
  /* init to default config here */
  art= default_dir;
  music= default_dir;
#ifndef NO_OPENGL
  opengl= true;
#else
  opengl= false;
#endif
  sound= false;
  fullscreen= false;

  /* ## default keys */ 
}


KD_Config::~KD_Config()
{ if (art!= default_dir)
  { free (art);
    art= default_dir;
  }
  if (music!= default_dir)
  { free (music);
    music= default_dir;
  }
  
  config= NULL;
} 


void KD_Config::ReadConfiguration()
{
#ifndef WIN32
  /* for UNIXes system, try /etc/kdrop.ini first */
  TryConfigurationFile ("/etc/kdrop.ini");
#endif

  /* now, look for user-specific configuration file */  
  char user_file[128];
#ifndef WIN32
  /* find the user's home, the UNIX way */
  char* home= getenv("HOME");
#else
  /* find the user's home, the Windoze way */
  /* Document Settings\username\ ? */
  //string hometemp = getenv("USERPROFILE");
  char* home= ".";
  
#endif

  if (home!= NULL)
  {
    snprintf (user_file, 128, "%s/kdrop.ini", home);
    user_file[127]= 0;
    if (TryConfigurationFile (user_file)== false)
    {
      // try .kdroprc instead
      snprintf (user_file, 128, "%s/.kdroprc", home);
      user_file[127]= 0;
      if (TryConfigurationFile (user_file)== false)
      {
        // last chance
        TryConfigurationFile ("kdrop.ini");
      }
    }
  }
  else TryConfigurationFile ("kdrop.ini");
}


bool KD_Config::AssignVar (const char* var, const char* val)
{ unsigned short i;
  bool b;
  
  // does that variable exist ?
  for (i= 0; i< KD_NB_VARNAMES; i++)
    if (strcasecmp(var, var_names[i])== 0) break;
  if (i== KD_NB_VARNAMES) return false;

  // is it a boolean value? if it is, read its value
  if (i< KD_CONFIG_LAST_BOOL)
  { /* read the first char of val to see if it is 'yes' or 'no' */
    if (val[0]== 'y' || val[0]== 'Y') b= true;  else
    if (val[0]== 'n' || val[0]== 'N') b= false; else return false;
    
    switch (i)
    { case 0: opengl= b;     break;
      case 1: sound= b;      break;
      case 2: fullscreen= b; break;
      default: assert (0);
    }
    return true;
  }

  // do we expect a directory?
  if (i< KD_CONFIG_LAST_DIR)
  {
    switch (i)
    { case 3: if (art!= default_dir) free (art);
              art= strdup(val);  break;
      case 4: if (music!= default_dir) free (music);
              music= strdup(val);break;
    }
    return true;
  }
  
  // we expect a key mapping item
  unsigned short type, value;  
  if (sscanf (val, "%hd , %hd",&type,&value)!= 2)
  { char c;
   // maybe the value was specified as a letter
    if (sscanf (val, "%hd , %c",&type,&c)!= 2) return false;
    value= c;
    // convert into lower case if necessary
    if (value>= 'A' && value<= 'Z') value+= ('a'-'A');
  }

  switch (i)
  { case 5 : p1up_t= type;   p1up= value;   break;
    case 6 : p1down_t= type; p1down= value; break;
    case 7 : p1left_t= type; p1left= value; break;
    case 8 : p1right_t= type;p1right= value;break;
    case 9 : p1xtra_t= type; p1xtra= value; break;
    case 10: p2up_t= type;   p2up= value;   break;
    case 11: p2down_t= type; p2down= value; break;
    case 12: p2left_t= type; p2left= value; break;
    case 13: p2right_t= type;p2right= value;break;
    case 14: p2xtra_t= type; p2xtra= value; break;
    default: assert (0);
  }
  return true;
}


bool KD_Config::TryConfigurationFile (char* f)
{ 
  assert (f!= NULL);

  ifstream fs;
  fs.open (f);
  if (!fs) return false; // could not find file f

  line_count= 0;
  unsigned pos_l, pos_m, pos_n, pos_r;
  while (!fs.eof())
  {
    getline (fs, line, '\n');
    line_count++;   

    pos_l= 0;
    pos_m= 0;
    pos_r= (unsigned int) line.size();   
    while ((line[pos_l]== ' ' || line[pos_l]== '\t') && pos_l< pos_r) pos_l++;

    // check if the line is a comment
    if (line[pos_l]== '#' || pos_l== pos_r)  continue;   
      
    // find the equal sign
    pos_m= (unsigned int) line.find ('=', 0);
    pos_n= pos_m+ 1;    
    if (pos_m== string::npos)
    { fprintf (stderr, "Warning, ignoring illegal line #%d\n", line_count);
      continue;
    }
    
    // remove the trailing spaces
    while (pos_m> pos_l && (line[pos_m- 1]== ' ' || line[pos_m- 1]== '\t')) pos_m--;
    while (pos_n< pos_r && (line[pos_n]== ' ' || line[pos_n]== '\t')) pos_n++;
    while (pos_n< pos_r && (line[pos_r-1]== ' ' || line[pos_r-1]== '\t')) pos_r--;
    var= line.substr (pos_l, pos_m- pos_l);
    val= line.substr (pos_n, pos_r- pos_n);
    
    // if the variable exists, change its value
    if (AssignVar (var.c_str(), val.c_str())== false)
      fprintf (stderr, "Warning, ignoring invalid line #%d\n", line_count);
  } 
  
  fs.close();
  return true;
}


/*
int main()
{
  KD_Config* k= KD_Config::GetConfig();
  k->ReadConfiguration();
  printf ("%d %d %d %d\n", k->opengl, k->sound, k->fullscreen, k->p1up);
  return 0;
}*/
