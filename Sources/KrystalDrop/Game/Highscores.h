#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#define KD_E_CANTWRITESCOREFILE    -90
#define KD_E_CANTREADSCOREFILE     -91
#define KD_E_INCOMPATIBLESCOREFILE -92
#define KD_E_INCORRECTSCOREFILE    -93
#define KD_E_CANTREADACCFILE       -94

class TACCRes;

class KD_ScoreItem
{ protected:
   short max_name_length;
   char* name;
   int score;
   int info;
  public:
   KD_ScoreItem();
   KD_ScoreItem (short Max_Name_Length);
  ~KD_ScoreItem();

  void ChangeMaxNameSize (short Max_Name_Length);
  
  char*  GetName();
  int    GetScore();
  int    GetInfo();
  void   SetName (char* Name);
  void   SetScore(int Score);
  void   SetInfo (int Info);
  signed IsDefined();
};


class KD_HighScoreTable
{ protected:
   short max_name_length;
   short max_scores;
   KD_ScoreItem* table;
   short nb_scores;
  
  public:
    KD_HighScoreTable (short Max_Name_Length, short Max_Scores);
   ~KD_HighScoreTable ();
    
  signed LoadTable (FILE* f); 
  //signed LoadTableFromACC (TACCRes* acc, unsigned Index); 
  signed SaveTable (FILE* f);
  /* For LoadTable and SaveTable, f must be opened and positioned */
  /* If loading fails, the content of the memory is not preserved */
  /* If saving fails, the content of the memory is preserved */
  
  short  GetNbScores();
  char*  GetName   (short pos);
  int    GetScore  (short pos);
  int    GetInfo   (short pos);
  signed IsDefined (short pos);
  
  bool IsBetterHighScore (int score);
  bool IsBetterLowScore  (int score);
  
  void InsertLowerScore (char* name, int score, int info);
  void InsertHigherScore(char* name, int score, int info);
};

#endif
