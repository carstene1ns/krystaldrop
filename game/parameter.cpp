#include "parameter.h"

KD_Parameters::KD_Parameters (short Line_Down_Speed, short Line_Down_Accel)
{ SetParameters (Line_Down_Speed, Line_Down_Accel);
}


void KD_Parameters::SetParameters (short Line_Down_Speed, short Line_Down_Accel)
{ line_down_speed= Line_Down_Speed;
  line_down_accel= Line_Down_Accel;
}
  
  
short KD_Parameters::Get_Line_Speed_down()
{ return line_down_speed;
}


short KD_Parameters::Get_Line_Accel_down()
{ return line_down_accel;
}


short KD_Parameters::IsLineDown()
{ return (state& KD_S_LINEDOWN); 
}


void KD_Parameters::SetLineDown()
{ state|= KD_S_LINEDOWN;
}


void KD_Parameters::ClearLineDown()
{ state&= ~KD_S_LINEDOWN;
}