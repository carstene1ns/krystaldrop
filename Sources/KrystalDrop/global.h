#ifndef Global_H
#define Global_H

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define DELETE(p)      { if (p!= NULL) { delete   p; p= NULL; } }
#define DELETE_ARR(p)  { if (p!= NULL) { delete[] p; p= NULL; } }
#define FREE(p)        { if (p!= NULL) { free   (p); p= NULL; } }
  
#ifndef DEBUG
#define CHECK_ALLOC(p) { if (p== NULL) { fprintf (stderr, "Error allocating memory in %s:%d.\n", __FILE__, __LINE__); exit (1); } }
#else
#define CHECK_ALLOC(p) { assert (p); }
#endif

#define NEW(p,object)  { p= new object; CHECK_ALLOC(p); }

#define GETBACK(p)     { p= KD_Background::GetBackground(); assert (p); }
#ifndef NO_MUSIC
#define PLAYMUSIC(m)   { music->Load(m); music->PlayMusic(); }
#define CLOSEMUSIC()   { music->StopMusic(); music->CloseMusic(); }
#else
#define PLAYMUSIC(m)
#define CLOSEMUSIC()
#endif

class KD_ResourceSet;
void LoadArtResourceFile (KD_ResourceSet* rs, char* archive, char* resource);

#define SCR_W 640
#define SCR_H 480
#define SCR_HW (SCR_W/2)
#define SCR_HH (SCR_H/2)

#endif
