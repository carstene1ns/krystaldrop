#include "../global.h"

#include "HighScoresController.h"
#include "../Controller/KDApplication.h"
#include "../Video/Background.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Image.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#include "../../KDpp/Tools/Logfile.h"
#include "../../KDpp/Sound/Music.h"

#define ANIM_SIZE 150

KD_HighScoreTable** KD_HighScoresController::hst= NULL;

/* character select controller */
KD_HighScoresController::KD_HighScoresController(): KD_Controller()
{ unsigned i;

  for (i= 0; i< KD_HSC_NB_FONT; i++)
    font[i]= NULL;

  //hst= (KD_HighScoreTable*) new KD_HighScoreTable[KD_NB_HST](PLAYER_NAME_SIZE, MAX_PLAYERS_IN_HIGH_SCORE);
  hst= (KD_HighScoreTable**) malloc (2* sizeof(KD_HighScoreTable*));
  assert (hst);
  
  hst[0]= new KD_HighScoreTable(PLAYER_NAME_SIZE, MAX_PLAYERS_IN_HIGH_SCORE);
  
  /* try to read the high scores */
  FILE* f= NULL;
  
#ifndef WIN32
  /* try BINDIR/survival.sco first */
  char user_file[128];
  snprintf (user_file, 128, "%s/survival.sco", BINDIR);
	user_file[127]= 0;
  
  f= fopen (user_file, "r");
  if (f!= NULL)
  { signed res= hst[0]->LoadTable (f);
    fclose (f);
    if (res!= 0) /* the high scores table is corrupt */
      f= NULL;
    else goto highscore_ok;
  }
#endif

  /* try ./survival.sco */
  f= fopen ("survival.sco", "r");
  if (f!= NULL)
  { signed res= hst[0]->LoadTable (f);
    fclose (f);
    if (res!= 0) /* the high scores table is corrupt */
      f= NULL;
    else goto highscore_ok;
  }

  /* if the file does not exist, or is incorrect, we re-create one
     using art/survival.sco (which should not change) */
  f= fopen (KD_KDApplication::GetArtFile("survival.sco").c_str(), "r");
  if (f!= NULL)
  { signed res= hst[0]->LoadTable (f);
    fclose (f);
    if (res!= 0) /* the high scores table is corrupt */
      f= NULL;
    else
    {
      /* try to write a survival.sco file. failures are silently ignored */
      f= fopen ("survival.sco", "w+");
      if (f!= NULL)
      {
        hst[0]->SaveTable (f);
        fclose (f);
      }

      /* the high score file should have been restored */
      goto highscore_ok;
    }
  }

  if (f== NULL)
	{
		KD_LogFile::printf2("Error: could not find file a valid \"survival.sco\" file");
		assert (0);
	}

highscore_ok:
  nb_anim_letters= 0;
}
  

KD_HighScoresController::~KD_HighScoresController()
{
  if (hst!= NULL)
  { if (hst[0]!= NULL)
    { delete hst[0]; 
      hst[0]= NULL;
    }
    
    free (hst);
    hst= NULL;
  }
}


void KD_HighScoresController::DisplayTexts()
{ font[0]->xycenteredprintf (320, 55, " High Scores\nSurvival mode");
}


void KD_HighScoresController::DisplayChars()
{ float inc= (Display::GetTimeElapsed())* 100;
  unsigned i;
  
  assert (font[1]);
  assert (X_L);
  assert (Y_L);
  assert (R_L);
  assert (A_L);
  
  for (i= 0; i< nb_anim_letters; i++)
  {
    if (R_L[i]> 1) R_L[i]-= inc*1.1; else R_L[i]= 0;
    A_L[i]+= inc* 0.04;
    font[1]->xycenteredprintf (X_L[i]+ cos(A_L[i])* R_L[i], 
                               Y_L[i]+ sin(A_L[i])* R_L[i], 
                               "%c", C_L[i]);
  }
}


void KD_HighScoresController::DisplayFaces()
{ signed i;
  signed long tick= Display::GetTicks();
  long l;
  
  for (i= MAX_PLAYERS_IN_HIGH_SCORE- 1; i>= 0 ; i--)
  {
    l= 2200+ ((signed long)first_tick- tick)/2+ i* 100;
    if (l< 0) l= 0;
    if (i% 2)
      img[hst[0]->GetInfo (i)]->Display (460+ l, i* 36+ 124);
    else
      img[hst[0]->GetInfo (i)]->Display (105- l, i* 36+ 124);
  }
}


bool KD_HighScoresController::Init()
{  
  LoadResourceFile(KD_KDApplication::GetArtFile("characters/characters.txt"));
   
  for (short ind= 0; ind< KD_HSC_NB_IMG; ind++)
	  img[ind] = (KD_Image*)GetResource(CHAR_IMG_NAME[ind+ KD_NB_CHAR]);
     
  font[0] = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font");
  font[1] = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("medium font");

#ifndef NO_MUSIC
  music = new KD_Music();
#endif
  
  BindKeyDown(SDLK_ESCAPE, 1);
  BindKeyDown(SDLK_SPACE, 2); 
  BindKeyDown(SDLK_RETURN, 2);
  
  return true;
}


bool KD_HighScoresController::ProcessEvent(int value)
{ switch (value)
  { case 1: KD_Application::GetApplication()->SendStopEvent(); return true;
    case 2:
		KD_Application::GetApplication()->DisableController(this);
		KD_Application::GetApplication()->EnableController ("TitleController");   
		
		return true;
  }

  return false;
}

bool KD_HighScoresController::Process()
{
	return false;
}

bool KD_HighScoresController::Display()
{
  DisplayChars();
  DisplayFaces();
  DisplayTexts();

  //if (SDL_GetTicks()- first_tick> 80000) 
  if (Display::GetTicks()- first_tick> 80000) 
  {
	  KD_Application::GetApplication()->DisableController(this);
	  KD_Application::GetApplication()->EnableController ("TitleController");   
  }
  
  return true;
}


bool KD_HighScoresController::Quit()
{
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("medium font");

#ifndef NO_MUSIC
  delete music;
#endif

  /* save the high scores */
  FILE* f= NULL;
  
#ifndef WIN32
  /* try BINDIR/survival.sco first */
  char user_file[128];
  snprintf (user_file, 128, "%s/survival.sco", BINDIR);
	user_file[127]= 0;
  f= fopen (user_file,"w+");
#endif

  if (f== NULL) f= fopen ("survival.sco", "w+");
  if (f!= NULL)
  {
    assert (hst[0]);
#ifndef NDEBUG  
    signed res= 
#endif  
    hst[0]->SaveTable (f);
    assert (res== 0);
    fclose (f);
  }
  else
  {
    // # print warning here
  }
  
  for (short i= 0; i< KD_HSC_NB_IMG; i++)
	  ReleaseResource(img[i]);

  return true;  
}

bool KD_HighScoresController::OnEnable()
{
#ifndef NO_MUSIC
	music->Load(KD_KDApplication::GetArtFile(MUSIC_NAME[KD_MUS_HIGHSCORES]).c_str());
	music->PlayMusic();
#endif 

	first_tick= Display::GetTicks();

	/* how many letters will we have to animate ? */
  unsigned i, j;
  nb_anim_letters= 0;
  for (i= 0; i< MAX_PLAYERS_IN_HIGH_SCORE; i++)
  { for (j= 0; j< PLAYER_NAME_SIZE; j++)
      if (hst[0]->GetName(i)[j]!= 32) nb_anim_letters++;
    
    nb_anim_letters+= (unsigned) (log10(hst[0]->GetScore(i)+ 1)+ 1);
  }
  
  X_L= (float*) malloc(nb_anim_letters* sizeof(float));
  Y_L= (float*) malloc(nb_anim_letters* sizeof(float));
  R_L= (float*) malloc(nb_anim_letters* sizeof(float));
  A_L= (float*) malloc(nb_anim_letters* sizeof(float));
  C_L= (char*)  malloc(nb_anim_letters* sizeof(char)) ;
  assert (X_L); assert (Y_L); assert (R_L); assert (A_L); assert (C_L);
  
  nb_anim_letters= 0;
  for (i= 0; i< MAX_PLAYERS_IN_HIGH_SCORE; i++)
  { for (j= 0; j< PLAYER_NAME_SIZE; j++)
      if (hst[0]->GetName(i)[j]!= 32)
      { C_L[nb_anim_letters]= hst[0]->GetName(i)[j];
        Y_L[nb_anim_letters]= i* 36+ 170;
        X_L[nb_anim_letters]= j* 30+ 200;
        R_L[nb_anim_letters]= 680+ nb_anim_letters* 10;
        A_L[nb_anim_letters]= nb_anim_letters* 0.8;
        nb_anim_letters++;
      }

    int score= hst[0]->GetScore(i);
    int pos= 0;
    while (1)
    { Y_L[nb_anim_letters]= i* 36+ 170;
      X_L[nb_anim_letters]= 430- pos* 16;
      R_L[nb_anim_letters]= 730+ nb_anim_letters* 10;
      A_L[nb_anim_letters]= nb_anim_letters* 0.8;
      C_L[nb_anim_letters]= 48+ (score% 10);
      nb_anim_letters++;
      pos++;
      score/= 10;
      if (score== 0) break;
    }
  }
  return true;
}

bool KD_HighScoresController::OnDisable()
{
#ifndef NO_MUSIC
	music->StopMusic();
	music->CloseMusic();
#endif

  if (X_L!= NULL) { free (X_L); X_L= NULL; }
  if (Y_L!= NULL) { free (Y_L); Y_L= NULL; }
  if (R_L!= NULL) { free (R_L); R_L= NULL; }
  if (A_L!= NULL) { free (A_L); A_L= NULL; }
  if (C_L!= NULL) { free (C_L); C_L= NULL; }  

	return true;
}
