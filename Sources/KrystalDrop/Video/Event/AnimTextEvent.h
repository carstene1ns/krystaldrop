/* animtextevent.h

   Various animations for texts
*/   

#ifndef ANIMTEXTEVENT_H
#define ANIMTEXTEVENT_H

#include "../../../KDpp/Controller/Event.h"

class KD_Font;

/* bouncing letters for titles */
class KD_BouncingText: public KD_Event
{ protected:
    KD_Font* font;
    char*    text;
    float*   X_L;
    float*   Y_L;
    float*   A_L; /* angles */
    float    nb;  /* final animation variable */  
    int C_X;      /* center X */
    int C_Y;      /* center Y */ 
    unsigned len; /* text length */
    int space;    /* space between letters, corner to corner */
    char IsRemoving; /* should the text disappear */


  public:
           KD_BouncingText (char* Text, KD_Font* Font, int CenterX, int CenterY);
  virtual ~KD_BouncingText ();
  virtual void Display();
  virtual void Update (float timeElapsed);
          void RemoveText();
};


class KD_MessageText: public KD_Event
{ protected:
    KD_Font* font;
    char*    text;
    float*   X_L;
    float*   Y_L;
    float*   A_L;    /* angles */
    int C_X;         /* center X */
    int C_Y;         /* center Y */
    unsigned len;    /* text length */
    int space;       /* space between letters, corner to corner */
    char IsRemoving; /* should the text disappear */

  public:
           KD_MessageText (char* Text, KD_Font* Font, int CenterX, int CenterY);
  virtual ~KD_MessageText ();
  virtual void Display();
  virtual void Update (float timeElapsed);
          void RemoveText(); /* scroll the text out of the screen, and 
                                eventually destroy it */
};

#endif
