#include "parameter.h"

KD_Parameters::KD_Parameters()
{ SetGameParameters (0, 0, 0, 0);
  SetVideoParameters (0, 0, 0);
}

KD_Parameters::KD_Parameters (short Line_Down_Speed, short Line_Down_Accel,
                              short Take_Hand_Speed, short Take_Hand_Accel,
                              short Gem_Height_In_Pixel, 
                              short Height_Field_In_Pixel, short Offset_Field_In_Pixel)
{ SetGameParameters (Line_Down_Speed, Line_Down_Accel,
                     Take_Hand_Speed, Take_Hand_Accel);
  SetVideoParameters (Gem_Height_In_Pixel, 
                      Height_Field_In_Pixel, Offset_Field_In_Pixel);
}


void KD_Parameters::SetGameParameters (short Line_Down_Speed, short Line_Down_Accel,
                                       short Take_Hand_Speed, short Take_Hand_Accel)
{ line_down_speed= Line_Down_Speed;
  line_down_accel= Line_Down_Accel;
  take_hand_speed= Take_Hand_Speed;
  take_hand_accel= Take_Hand_Accel; 
}


void KD_Parameters::SetVideoParameters (short Height_Gem_In_Pixel, 
                                        short Height_Field_In_Pixel, short Offset_Field_In_Pixel)
{ height_gem_in_pixel= Height_Gem_In_Pixel; 
  height_field_in_pixel= Height_Field_In_Pixel;
  offset_field_in_pixel= Offset_Field_In_Pixel;
}
  
  
short KD_Parameters::Get_Line_Down_Speed()
{ return line_down_speed; }

short KD_Parameters::Get_Line_Down_Accel()
{ return line_down_accel; }

short KD_Parameters::Get_Take_Hand_Speed()
{ return take_hand_speed; }

short KD_Parameters::Get_Take_Hand_Accel()
{ return take_hand_accel; }

short KD_Parameters::Get_Height_Gem_In_Pixel()
{ return height_gem_in_pixel; }

short KD_Parameters::Get_Height_Field_In_Pixel()
{ return height_field_in_pixel; }

short KD_Parameters::Get_Offset_Field_In_Pixel()
{ return offset_field_in_pixel; }

short KD_Parameters::IsLineDown()
{ return (state& KD_S_LINEDOWN); }

void KD_Parameters::SetLineDown()
{ state|= KD_S_LINEDOWN; }

void KD_Parameters::ClearLineDown()
{ state&= ~KD_S_LINEDOWN; }

short KD_Parameters::IsTakeHand()
{ return (state& KD_S_TAKEHAND); }

void KD_Parameters::SetTakeHand()
{ state|= KD_S_TAKEHAND; }

void KD_Parameters::ClearTakeHand()
{ state&= ~KD_S_TAKEHAND; }