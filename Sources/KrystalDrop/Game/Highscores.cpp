#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <string.h>

#include "Highscores.h"
//#include "../util/direct.h"


KD_ScoreItem::KD_ScoreItem ()
{ max_name_length= 0;
  name= NULL;
  score= -1; 
  info= 0;
}


/* kludge for Visual C */
void KD_ScoreItem::ChangeMaxNameSize (short Max_Name_Length)
{ assert (name== NULL);
  max_name_length= Max_Name_Length;
  name= (char*) malloc (max_name_length+ 1);
  assert (name);
}


KD_ScoreItem::KD_ScoreItem (short Max_Name_Length)
{ max_name_length= Max_Name_Length;
  name= (char*) malloc (max_name_length+ 1);
  assert (name);
  name[0]= 0;
  score= -1;
  info = 0;
}


char* KD_ScoreItem::GetName() { return name; }
int KD_ScoreItem::GetScore()  { return score;}
int KD_ScoreItem::GetInfo()   { return info; }


void KD_ScoreItem::SetName (char* Name)
{ assert (name);
  assert (Name);
  if (name!= NULL && Name!= NULL)
  { strncpy (name, Name, max_name_length);
    name[max_name_length]= 0;
  }
}


void KD_ScoreItem::SetScore (int Score) { score= Score; }
void KD_ScoreItem::SetInfo  (int Info)  { info= Info;   }


signed KD_ScoreItem::IsDefined ()
{ return (name!= NULL && name[0]!= 0);
}


KD_ScoreItem::~KD_ScoreItem()
{ if (name!= NULL) 
  { free (name);
    name= NULL;
  }
}


KD_HighScoreTable::KD_HighScoreTable (short Max_Name_Length, short Max_Scores)
{ max_name_length= Max_Name_Length;
  max_scores= Max_Scores;
  nb_scores= 0;
  
  table= new KD_ScoreItem[max_scores];
  for (int i= 0; i< max_scores; i++) table[i].ChangeMaxNameSize (Max_Name_Length);

  assert (table);
}


KD_HighScoreTable::~KD_HighScoreTable()
{ if (table!= NULL)
  { delete[] table;
    table= NULL;
  }
}


short KD_HighScoreTable::GetNbScores()
{ return nb_scores;
}


char* KD_HighScoreTable::GetName (short pos)
{ assert (pos< nb_scores);
  return table[pos].GetName();
}


int KD_HighScoreTable::GetScore (short pos)
{ assert (pos< nb_scores);
  return table[pos].GetScore();
}


int KD_HighScoreTable::GetInfo (short pos)
{ assert (pos< nb_scores);
  return table[pos].GetInfo();
}
  

signed KD_HighScoreTable::IsDefined (short pos)
{ assert (pos< nb_scores);
  return table[pos].IsDefined();
}

bool KD_HighScoreTable::IsBetterLowScore (int score)
{ 
  if (nb_scores< max_scores) return true;
  return (score< GetScore(max_scores- 1));
}


bool KD_HighScoreTable::IsBetterHighScore (int score)
{ 
  if (nb_scores< max_scores) return true;
  return (score> GetScore(max_scores- 1));
}


void KD_HighScoreTable::InsertLowerScore (char* name, int score, int info)
{ short index;
  short pos;
  
  for (index= 0; index< nb_scores; index++)
  { if (GetScore(index)> score) break;
  }
  
  if (index== max_scores) return;
  if (nb_scores< max_scores) nb_scores++;
  
  pos= index;
  for (index= nb_scores- 2; index>= pos; index--)
  { table[index+ 1].SetName  (table[index].GetName());
    table[index+ 1].SetScore (table[index].GetScore());
    table[index+ 1].SetInfo  (table[index].GetInfo());
  }
  
  table[pos].SetName  (name);
  table[pos].SetScore (score);
  table[pos].SetInfo  (info);
}


void KD_HighScoreTable::InsertHigherScore (char* name, int score, int info)
{ short index;
  short pos;
  
  for (index= 0; index< nb_scores; index++)
  { if (GetScore(index)< score) break;
  }
  
  if (index== max_scores) return;
  if (nb_scores< max_scores) nb_scores++;
  
  pos= index;
  for (index= nb_scores- 2; index>= pos; index--)
  { table[index+ 1].SetName  (table[index].GetName());
    table[index+ 1].SetScore (table[index].GetScore());
    table[index+ 1].SetInfo  (table[index].GetInfo());    
  }
  
  table[pos].SetName  (name);
  table[pos].SetScore (score);
  table[pos].SetInfo  (info);  
}


signed KD_HighScoreTable::SaveTable (FILE* f)
{ assert (f);
  short index;
  signed car;
  signed res;
  unsigned long check= 1;
  char m= 0;
  
  res= fprintf (f, "%hx %hx %hx\n", max_scores, max_name_length, nb_scores);
  if (res<= 0) return KD_E_CANTWRITESCOREFILE;

  for (index= 0; index< nb_scores; index++)
  { for (car= 0; car< max_name_length; car++)
    { m= m ^ GetName(index)[car];
      check+= m;
      fwrite (GetName(index)+ car, 1, 1, f);
    }
    
    res= fprintf (f, " %x %x\n", GetScore(index), GetInfo(index));
    if (res<= 0) return KD_E_CANTWRITESCOREFILE;
    check+= GetScore(index)* GetInfo(index);
  }
  
  res= fprintf (f, "%lx\n", check); 
  if (res<= 0) return KD_E_CANTWRITESCOREFILE;
  return 0;
}


signed KD_HighScoreTable::LoadTable (FILE* f)
{ assert (f);
  unsigned long check_c= 1;
  unsigned long check_r= 0;
  short max_scores_r;
  short max_name_length_r, nb_scores_r;
  short index;
  int score_r, info_r;
  signed res;

  res= fscanf (f, "%hx %hx %hx\n", &max_scores_r, &max_name_length_r, &nb_scores_r);
  if (res< 3) return KD_E_CANTREADSCOREFILE;

  if (max_scores_r!= max_scores || max_name_length_r!= max_name_length)
    return KD_E_INCOMPATIBLESCOREFILE;
  
  if (nb_scores_r> max_scores_r)
    return KD_E_INCORRECTSCOREFILE;
  
  nb_scores= nb_scores_r;
  
  char m= 0;
  for (index= 0; index< nb_scores; index++)
  { char c;
    
    for (signed car= 0; car< max_name_length; car++)
    { res= fread (&c, 1, 1, f);
      if (res<= 0) return KD_E_CANTREADSCOREFILE;
      GetName(index)[car]= c;
      m= m ^ c;
      check_c+= m;
    }
    
    res= fscanf (f, " %x %x\n", &score_r, &info_r);
    if (res< 1) return KD_E_CANTREADSCOREFILE;
    table[index].SetScore (score_r);
    table[index].SetInfo (info_r);    
    check_c+= GetScore(index)* GetInfo(index);      
  }
  
  res= fscanf (f, "%lx", &check_r);
  if (res< 1) return KD_E_CANTREADSCOREFILE;
  if (check_c!= check_r) return KD_E_INCORRECTSCOREFILE;
  
  return 0;
}


/*signed KD_HighScoreTable::LoadTableFromACC (TACCRes* acc, unsigned Index)
{ assert (acc);
  unsigned long check_c= 1;
  unsigned long check_r= 0;
  short max_scores_r;
  short max_name_length_r, nb_scores_r;
  short index;
  int score_r, info_r;
  signed res;
  char* buf;
  int count;
  
  buf= acc->EntryPtr (Index);
  if (buf== NULL) return KD_E_CANTREADACCFILE;

  res= sscanf (buf, "%hx %hx %hx\n%n", &max_scores_r, &max_name_length_r, &nb_scores_r, &count);
  buf+= count;
  if (res< 3) return KD_E_CANTREADSCOREFILE;

  if (max_scores_r!= max_scores || max_name_length_r!= max_name_length)
    return KD_E_INCOMPATIBLESCOREFILE;
  
  if (nb_scores_r> max_scores_r)
    return KD_E_INCORRECTSCOREFILE;
  
  nb_scores= nb_scores_r;
  
  char m= 0;
  for (index= 0; index< nb_scores; index++)
  { char c;
    
    for (signed car= 0; car< max_name_length; car++)
    { c= *buf; 
      buf++;
      GetName(index)[car]= c;
      m= m ^ c;
      check_c+= m;
    }
    
    res= sscanf (buf, " %x %x\n%n", &score_r, &info_r, &count);
    buf+= count;
    if (res< 1) return KD_E_CANTREADSCOREFILE;
    table[index].SetScore (score_r);
    table[index].SetInfo  (info_r);
    check_c+= GetScore(index)* GetInfo(index);
  }

  res= sscanf (buf, "%lx", &check_r);
  if (res< 1) return KD_E_CANTREADSCOREFILE;
  if (check_c!= check_r) return KD_E_INCORRECTSCOREFILE;

  return 0;
}*/


// uses this to create a default table
/*
int main()
{ KD_HighScoreTable t(3,9);
  t.InsertHigherScore ("Ark", 10000,1);
  t.InsertHigherScore ("Krs", 9000,2);
  t.InsertHigherScore ("Imp", 8000,3);
  t.InsertHigherScore ("Ssb", 7000,4);
  t.InsertHigherScore ("Keo", 6000,5);
  t.InsertHigherScore ("Tux", 5000,6);
  t.InsertHigherScore ("Gpl", 2000,7);
  t.InsertHigherScore ("Ssh", 500,8);
  t.InsertHigherScore ("XP!", 50,9);


  FILE* f;
  f= fopen ("survival.sco","w+");
  TACCRes acc;
  acc.LoadACC ("test.acc");
  t.LoadTableFromACC (&acc, 0);
  fclose (f);
  f= fopen ("temp2.bin","w+");
  t.SaveTable (f);
  fclose (f);
}
*/
