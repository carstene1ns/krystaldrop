#include "../global.h"

#include "../../KDpp/Controller/Application.h"
#include "HighScoresController.h"
//#include "../util/direct.h"
#include "../Video/Background.h"
#include "../../KDpp/Video/Display.h"
#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Video/Image.h"
//#include "../../KDpp/Video/imagemanager.h"
#include "../../KDpp/Video/Sprite.h"
#include "../../KDpp/Resources/GlobalResourceSet.h"
#include "../../KDpp/Tools/Logfile.h"
#include "../../KDpp/Sound/Music.h"

#define ANIM_SIZE 150

KD_HighScoreTable** KD_HighScoresController::hst= NULL;

/* character select controller */
KD_HighScoresController::KD_HighScoresController(): KD_Controller()
{ unsigned i;

 /* for (i= 0; i< KD_HSC_NB_SPRI; i++)
  { spri[i]= NULL;
  }*/

  for (i= 0; i< KD_HSC_NB_FONT; i++)
  { font[i]= NULL;
  }
  
  // CHANGER BACKGROUND: on doit en réinstancier un.
//  GETBACK (back);
  //LoadResourceFile("art/title/titleRes.txt");
  //back = new KD_Background((KD_DisplayableResource *)GetResource("title1"));
  
  //hst= (KD_HighScoreTable*) new KD_HighScoreTable[KD_NB_HST](PLAYER_NAME_SIZE, MAX_PLAYERS_IN_HIGH_SCORE);
  hst= (KD_HighScoreTable**) malloc (2* sizeof(KD_HighScoreTable*));
  assert (hst);
  
  hst[0]= new KD_HighScoreTable(PLAYER_NAME_SIZE, MAX_PLAYERS_IN_HIGH_SCORE);
  /* try to read the high scores */
  /* if the file doesn't exist, create from the default one */
  FILE* f= fopen ("survival.sco", "r");
  if (f!= NULL)
  { signed res= hst[0]->LoadTable (f);
    fclose (f);
    if (res!= 0) /* tried to modify high scores table ? reload the default one */
      f= NULL;
  }
  
  if (f== NULL)
  { /* read the default table from the acc file */ 
    //TACCRes acc;
    signed res=0;
    
    //res= acc.LoadACC (HST_NAME[0]);
    //assert (res== 0);
    //res= hst[0]->LoadTableFromACC (&acc, acc.EntryId (HST_NAME[1]));
    //assert (res== 0);
	FILE* f= fopen ("art/survival.sco", "r");
    if (f!= NULL)
	{
		res= hst[0]->LoadTable (f);
		fclose (f);
		if (res!= 0) /* tried to modify high scores table ? reload the default one */
			f= NULL;
	}	
    if (f==NULL || res!=0)
	{
		KD_LogFile::printf2("Error: could not find file \"art/survival.sco\"");
		assert (res==0);
	}

    f= fopen ("survival.sco", "w+");
    assert (f);
    hst[0]->SaveTable (f);
    fclose (f);
  }

  nb_anim_letters= 0;
}
  

KD_HighScoresController::~KD_HighScoresController()
{  
	//ReleaseResource("title1");

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
  signed long tick= SDL_GetTicks();
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
{ //signed res;
  //bool b;
  
  LoadResourceFile("art/characters/characters.txt");
   
  for (short ind= 0; ind< KD_HSC_NB_IMG; ind++)
  { /*b= image_manager->Load (acc, CHAR_IMG_NAME[ind+ KD_NB_CHAR]);
    assert (b);
    if (b== false) return false;
    img[ind]= image_manager->getImage(CHAR_IMG_NAME[ind+ KD_NB_CHAR]);*/
	  img[ind] = (KD_Image*)GetResource(CHAR_IMG_NAME[ind+ KD_NB_CHAR]);
  }
  
  
     
  font[0] = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("big font");
  font[1] = (KD_Font *)KD_GlobalResourceSet::GetGlobalResource()->GetResource("medium font");

  //  PLAYMUSIC (MUSIC_NAME[KD_MUS_HIGHSCORES]);
    music = new KD_Music();
  
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
{ //Display::clearScreen();

  //back->Display();
  DisplayChars();
  DisplayFaces();
  DisplayTexts();
  /* 80000 */
  if (SDL_GetTicks()- first_tick> 80000) 
  {
	  KD_Application::GetApplication()->DisableController(this);
	  KD_Application::GetApplication()->EnableController ("TitleController");   
  }
  
  return true;
}


bool KD_HighScoresController::Quit()
{
  //CLOSEMUSIC();

  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("big font");
  KD_GlobalResourceSet::GetGlobalResource()->ReleaseResource("medium font");
  //DELETE (font[1]);

  delete music;
  
  //delete back;
  
  /* save the high scores */
  FILE* f;
  f= fopen ("survival.sco", "w+");
  assert (f);
  assert (hst[0]);
#ifndef NDEBUG  
  signed res= 
#endif  
  hst[0]->SaveTable (f);
  assert (res== 0);
  fclose (f);
  
  //KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  //assert (image_manager);
  //if (image_manager== NULL) return false;
    
  //for (short i= 0; i< KD_HSC_NB_IMG; i++)
  //  image_manager->releaseImage (img[i]);

  for (short i= 0; i< KD_HSC_NB_IMG; i++)
	  ReleaseResource(img[i]);

//  delete spri[0]; spri[0]= NULL;
  
  return true;  
}

bool KD_HighScoresController::OnEnable()
{
	music->Load(MUSIC_NAME[KD_MUS_HIGHSCORES]);
	music->PlayMusic();

	first_tick= SDL_GetTicks();

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
	music->StopMusic();
	music->CloseMusic();

	if (X_L!= NULL) { free (X_L); X_L= NULL; }
  if (Y_L!= NULL) { free (Y_L); Y_L= NULL; }
  if (R_L!= NULL) { free (R_L); R_L= NULL; }
  if (A_L!= NULL) { free (A_L); A_L= NULL; }
  if (C_L!= NULL) { free (C_L); C_L= NULL; }  

	return true;
}
