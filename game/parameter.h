/* parameter.h
   
   Contains the 'physical' parameters of the gem, and the state of the field
*/

#ifndef PARAMETER_H
#define PARAMETER_H

#define KD_S_LINEDOWN 1

class KD_Parameters
{ protected:
   short line_down_speed;
   short line_down_accel;
   unsigned short state;

  public:
        KD_Parameters (short Line_Down_Speed, short Line_Down_Accel);
   
   void SetParameters (short Line_Down_Speed, short Line_Down_Accel);
   short Get_Line_Speed_down();
   short Get_Line_Accel_down();
   
   short IsLineDown();
   void SetLineDown();
   void ClearLineDown();
};

#endif
