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

   /* graphic parameter */
   short gem_height_in_pixel;
   short offset_field_in_pixel;

  public:
        KD_Parameters ();
        KD_Parameters (short Line_Down_Speed, short Line_Down_Accel, 
                       short Gem_Height_In_Pixel, short Offset_Field_In_Pixel);
   
   void SetGameParameters (short Line_Down_Speed, short Line_Down_Accel);
   void SetVideoParameters (short Gem_Height_In_Pixel, short Offset_Field_In_Pixel);
   short Get_Line_Speed_down();
   short Get_Line_Accel_down();
   short Get_Gem_Height_In_Pixel();
   short Get_Offset_Field_In_Pixel();
   
   short IsLineDown();
   void SetLineDown();
   void ClearLineDown();
};

#endif
