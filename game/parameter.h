/* parameter.h
   
   Contains the 'physical' parameters of the gem, and the state of the field
*/

#ifndef PARAMETER_H
#define PARAMETER_H

#define KD_S_LINEDOWN 1
#define KD_S_TAKEHAND 2

class KD_Parameters
{ protected:
   short line_down_speed;
   short line_down_accel;
   short take_hand_speed;
   short take_hand_accel;
   short drop_hand_speed;
   short drop_hand_accel;   
   unsigned short state;

   /* graphic parameter */
   short height_gem_in_pixel;
   short height_field_in_pixel;
   short offset_field_in_pixel;

  public:
        KD_Parameters ();
        KD_Parameters (short Line_Down_Speed, short Line_Down_Accel, 
                       short Take_Hand_Speed, short Take_Down_Accel,
                       short Drop_Hand_Speed, short Drop_Down_Accel,                       
                       short Height_Gem_In_Pixel, 
                       short Height_Field_In_Pixel, short Offset_Field_In_Pixel);
   
   void SetGameParameters (short Line_Down_Speed, short Line_Down_Accel,
                           short Take_Hand_Speed, short Take_Hand_Accel,
                           short Drop_Hand_Speed, short Drop_Hand_Accel);
   void SetVideoParameters (short Height_Gem_In_Pixel, 
                            short Height_Field_In_Pixel, short Offset_Field_In_Pixel);
   short Get_Line_Down_Speed(); /* used in Set but not in row */
   short Get_Line_Down_Accel(); /* ditto */
   short Get_Take_Hand_Speed();
   short Get_Take_Hand_Accel(); 
   short Get_Drop_Hand_Speed();
   short Get_Drop_Hand_Accel(); 
   short Get_Height_Gem_In_Pixel();
   short Get_Height_Field_In_Pixel();   
   short Get_Offset_Field_In_Pixel();
   
   short IsLineDown();
   void SetLineDown();
   void ClearLineDown();
   
   short IsTakeHand();
   void SetTakeHand();
   void ClearTakeHand();
};

#endif
