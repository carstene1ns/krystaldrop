#ifndef BACKGROUND_H
#define BACKGROUND_H

#define KD_BACKGROUND_SPR 20

class KD_Image;

class KD_Background
{ protected:
    void  InitBackground(); 
   float X_S[KD_BACKGROUND_SPR];
   float Y_S[KD_BACKGROUND_SPR];
  static KD_Background* p_background;
  
   KD_Image* back;
  
  public:
   KD_Background();
  ~KD_Background();
  
   void  Display();  

  static KD_Background* GetBackground();
};

#endif