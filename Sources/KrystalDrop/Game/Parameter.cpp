#include <assert.h>

#include "Parameter.h"

KD_Parameters::KD_Parameters()
{ SetGameParameters (0, 0, 0, 0, 0, 0, 0, 0);
  SetVideoParameters (0, 0, 0, 0, 0);
  state= 0;
  gems_count= 0;
}

KD_Parameters::KD_Parameters (short Line_Down_Speed, short Line_Down_Accel,
                              short Gem_Up_Speed,    short Gem_Up_Accel,
                              short Take_Hand_Speed, short Take_Hand_Accel,
                              short Drop_Hand_Speed, short Drop_Hand_Accel,                              
                              short Gem_Height_In_Pixel, short Gem_Width_In_Pixel,
                              short Height_Field_In_Pixel, 
                              short Offset_Field_X_In_Pixel, short Offset_Field_Y_In_Pixel)
{ SetGameParameters (Line_Down_Speed, Line_Down_Accel,
                     Gem_Up_Speed,    Gem_Up_Accel,
                     Take_Hand_Speed, Take_Hand_Accel,
                     Drop_Hand_Speed, Drop_Hand_Accel);                     
  SetVideoParameters (Gem_Height_In_Pixel, Gem_Width_In_Pixel,
                      Height_Field_In_Pixel, 
                      Offset_Field_X_In_Pixel, Offset_Field_Y_In_Pixel);
  state= 0;
}


void KD_Parameters::SetGameParameters (short Line_Down_Speed, short Line_Down_Accel,
                                       short Gem_Up_Speed,    short Gem_Up_Accel,
                                       short Take_Hand_Speed, short Take_Hand_Accel,
                                       short Drop_Hand_Speed, short Drop_Hand_Accel)
{ line_down_speed= Line_Down_Speed;
  line_down_accel= Line_Down_Accel;
  gem_up_speed= Gem_Up_Speed;
  gem_up_accel= Gem_Up_Accel;
  take_hand_speed= Take_Hand_Speed;
  take_hand_accel= Take_Hand_Accel; 
  drop_hand_speed= Drop_Hand_Speed;
  drop_hand_accel= Drop_Hand_Accel;   
}


void KD_Parameters::SetVideoParameters (short Height_Gem_In_Pixel, short Width_Gem_In_Pixel, 
                                        short Height_Field_In_Pixel, 
                                        short Offset_Field_X_In_Pixel, short Offset_Field_Y_In_Pixel)
{ height_gem_in_pixel= Height_Gem_In_Pixel; 
  width_gem_in_pixel= Width_Gem_In_Pixel;
  height_field_in_pixel= Height_Field_In_Pixel;
  offset_field_x_in_pixel= Offset_Field_X_In_Pixel;
  offset_field_y_in_pixel= Offset_Field_Y_In_Pixel;
}
  

short KD_Parameters::Get_Line_Down_Speed()
{ return line_down_speed; }

short KD_Parameters::Get_Line_Down_Accel()
{ return line_down_accel; }

short KD_Parameters::Get_Gem_Up_Speed()
{ return gem_up_speed; }

short KD_Parameters::Get_Gem_Up_Accel()
{ return gem_up_accel; }

short KD_Parameters::Get_Take_Hand_Speed()
{ return take_hand_speed; }

short KD_Parameters::Get_Take_Hand_Accel()
{ return take_hand_accel; }

short KD_Parameters::Get_Drop_Hand_Speed()
{ return drop_hand_speed; }

short KD_Parameters::Get_Drop_Hand_Accel()
{ return drop_hand_accel; }

short KD_Parameters::Get_Height_Gem_In_Pixel()
{ return height_gem_in_pixel; }

short KD_Parameters::Get_Width_Gem_In_Pixel()
{ return width_gem_in_pixel; }

short KD_Parameters::Get_Height_Field_In_Pixel()
{ return height_field_in_pixel; }

short KD_Parameters::Get_Offset_Field_X_In_Pixel()
{ return offset_field_x_in_pixel; }

short KD_Parameters::Get_Offset_Field_Y_In_Pixel()
{ return offset_field_y_in_pixel; }

short KD_Parameters::Get_Gems_Count()
{ return gems_count; }

void KD_Parameters::Increment_Gems_Count()
{ gems_count++; 
}

void KD_Parameters::Decrement_Gems_Count()
{ assert (gems_count> 0);
  gems_count--; 
}

short KD_Parameters::NeedCheckOverflow()
{ return (state& KD_S_CHECKOVERFLOW); }

void KD_Parameters::SetCheckOverflow()
{ state|= KD_S_CHECKOVERFLOW; }

void KD_Parameters::ClearCheckOverflow()
{ state&= ~KD_S_CHECKOVERFLOW; }

short KD_Parameters::IsTakeHand()
{ return (state& KD_S_TAKEHAND); }

void KD_Parameters::SetTakeHand()
{ state|= KD_S_TAKEHAND; }

void KD_Parameters::ClearTakeHand()
{ state&= ~KD_S_TAKEHAND; }
/*
short KD_Parameters::IsNeedClashTest()
{ return (state& KD_S_NEEDCLASHTEST); }

void KD_Parameters::SetNeedClashTest()
{ state|= KD_S_NEEDCLASHTEST; }

void KD_Parameters::ClearNeedClashTest()
{ state&= ~KD_S_NEEDCLASHTEST; }
*/
short KD_Parameters::IsRemoving()
{ return (state& KD_S_REMOVING); }

void KD_Parameters::SetRemoving()
{ state|= KD_S_REMOVING; }

void KD_Parameters::ClearRemoving()
{ state&= ~KD_S_REMOVING; }
