#include "parameter.h"

KD_Parameters::KD_Parameters()
{ SetGameParameters (0, 0);
  SetVideoParameters (0, 0);
}

KD_Parameters::KD_Parameters (short Line_Down_Speed, short Line_Down_Accel,
                              short Gem_Height_In_Pixel, short Offset_Field_In_Pixel)
{ SetGameParameters (Line_Down_Speed, Line_Down_Accel);
  SetVideoParameters (Gem_Height_In_Pixel, Offset_Field_In_Pixel);
}


void KD_Parameters::SetGameParameters (short Line_Down_Speed, short Line_Down_Accel)
{ line_down_speed= Line_Down_Speed;
  line_down_accel= Line_Down_Accel;
}


void KD_Parameters::SetVideoParameters (short Gem_Height_In_Pixel, short Offset_Field_In_Pixel)
{ gem_height_in_pixel= Gem_Height_In_Pixel; 
  offset_field_in_pixel= Offset_Field_In_Pixel;
}
  
  
short KD_Parameters::Get_Line_Speed_down()
{ return line_down_speed; }

short KD_Parameters::Get_Line_Accel_down()
{ return line_down_accel; }

short KD_Parameters::Get_Gem_Height_In_Pixel()
{ return gem_height_in_pixel; }

short KD_Parameters::Get_Offset_Field_In_Pixel()
{ return offset_field_in_pixel; }


short KD_Parameters::IsLineDown()
{ return (state& KD_S_LINEDOWN); 
}


void KD_Parameters::SetLineDown()
{ state|= KD_S_LINEDOWN;
}


void KD_Parameters::ClearLineDown()
{ state&= ~KD_S_LINEDOWN;
}