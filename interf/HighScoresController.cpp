#include <assert.h>
#include <math.h>

#include "Application.h"
#include "HighScoresController.h"
#include "../sound/music.h"
#include "../sound/soundsystem.h"
#include "../util/direct.h"
#include "../video/background.h"
#include "../video/Display.h"
#include "../video/font.h"
#include "../video/image.h"
#include "../video/imagemanager.h"
#include "../video/sprite.h"

#define ANIM_SIZE 150

KD_HighScoreTable** KD_HighScoresController::hst= NULL;

/* character select controller */
KD_HighScoresController::KD_HighScoresController(): KD_Controller()
{ unsigned i;

  for (i= 0; i< KD_HSC_NB_SPRI; i++)
  { spri[i]= NULL;
  }

  for (i= 0; i< KD_HSC_NB_FONT; i++)
  { font[i]= NULL;
  }
  
  music= new KD_Music();
  assert (music);
  
  back= KD_Background::GetBackground();
  assert (back);
  
  //hst= (KD_HighScoreTable*) new KD_HighScoreTable[KD_NB_HST](PLAYER_NAME_SIZE, MAX_PLAYERS_IN_HIGH_SCORE);
  hst= (KD_HighScoreTable**) malloc (KD_NB_HST* sizeof(KD_HighScoreTable*));
  assert (hst);
  
  hst[0]= new KD_HighScoreTable(PLAYER_NAME_SIZE, MAX_PLAYERS_IN_HIGH_SCORE);
  /* try to read the high scores */
  /* if the file doesn't exist, create from the default one */
  FILE* f= fopen (HST_NAME[1], "r");
  if (f!= NULL)
  { signed res= hst[0]->LoadTable (f);
    fclose (f);
    if (res!= 0) /* tried to modify high scores table ? reload the default one */
      f= NULL;
  }
  
  if (f== NULL)
  { /* read the default table from the acc file */ 
    TACCRes acc;
    signed res;
    
    res= acc.LoadACC (HST_NAME[0]);
    assert (res== 0);
    res= hst[0]->LoadTableFromACC (&acc, acc.EntryId (HST_NAME[1]));
    assert (res== 0);
    
    FILE* f= fopen (HST_NAME[1], "w+");
    assert (f);
    hst[0]->SaveTable (f);
    fclose (f);
  }

  nb_anim_letters= 0;
}
  

KD_HighScoresController::~KD_HighScoresController()
{ if (music!= NULL)
  { delete music;
    music= NULL;
  }
  
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
{ float inc= (Display::timeElapsed)* 100;
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


bool KD_HighScoresController::init()
{ signed res;
  bool b;
  
  /* load the graphics */
  TACCRes* acc= new TACCRes();
  assert (acc);
  if (acc== NULL) return false;
    
  res= acc->LoadACC ("art/charsel.acc");
  assert (res== 0);
  if (res< 0) return false;
   
  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  for (short i= 0; i< KD_HSC_NB_IMG; i++)
  { b= image_manager->Load (acc, CHAR_IMG_NAME[i+ KD_NB_CHAR]);
    assert (b);
    if (b== false) return false;
    img[i]= image_manager->getImage(CHAR_IMG_NAME[i+ KD_NB_CHAR]);
  }
  
/*  b= spr[0].Load(acc,"ar_l.txt"); assert (b); if (b== false) return false;
  spri[0]= new KD_SpriteInstance (&spr[0]); assert (spri[0]);
  spri[0]->x= (int) (KD_CSC_CENTER_X1- KD_CSC_CENTER_R1* 0.8);
  spri[0]->y= KD_CSC_CENTER_Y1+ 12;
  spri[0]->setAnim(0);*/
  
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
     
  font[0]= Display::Slapstick;
  font[1]= Display::Slapstick->resize(0.5);

  music->Load(MUSIC_NAME[KD_MUS_HIGHSCORES]);
  music->PlayMusic();
  
  first_tick= 0; 
  first_tick= SDL_GetTicks();

  bindKeyDown(SDLK_ESCAPE, 1);
  bindKeyDown(SDLK_SPACE, 2); 
  bindKeyDown(SDLK_RETURN, 2);
  return true;
}


bool KD_HighScoresController::processEvent(int value)
{ switch(value)
  { case 1:  
		KD_Application::getApplication()->sendStopEvent(); 
		return true;
    case 2:
		KD_Application::getApplication()->gotoController ("title");
		return true;
  }

  return false;
}


bool KD_HighScoresController::display()
{
  Display::clearScreen();

  assert (back);
  back->Display();
  DisplayChars();
  DisplayFaces();
  DisplayTexts();
  /* 80000 */
/*  spri[0]->Display();*/
  
  return true;
}


bool KD_HighScoresController::quit()
{ music->StopMusic();
  music->CloseMusic();
 
  if (font[1])
  { delete (font[1]);
    font[1]= NULL;
  }
  
  if (X_L!= NULL) { free (X_L); X_L= NULL; }
  if (Y_L!= NULL) { free (Y_L); Y_L= NULL; }
  if (R_L!= NULL) { free (R_L); R_L= NULL; }
  if (A_L!= NULL) { free (A_L); A_L= NULL; }
  if (C_L!= NULL) { free (C_L); C_L= NULL; }  
  
  /* save the high scores */
  FILE* f;
  f= fopen (HST_NAME[1], "w+");
  assert (f);
  assert (hst[0]);
  signed res= hst[0]->SaveTable (f);
  assert (res== 0);
  fclose (f);
  
  KD_ImageManager* image_manager= KD_ImageManager::getImageManager();
  assert (image_manager);
  if (image_manager== NULL) return false;
    
  for (short i= 0; i< KD_HSC_NB_IMG; i++)
    image_manager->releaseImage (img[i]);

  delete spri[0]; spri[0]= NULL;
  
  return true;
}
