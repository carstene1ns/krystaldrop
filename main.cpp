#include "game/hand.h"
#include "game/parameter.h"
#include "game/row.h"
#include "game/set.h"
#include "SDL/SDL.h"
#include "interf/Application.h"

class KD_Gem;
/* temporary */

int main( int argc, char* argv[] )
{
  // debug
/*    KD_Parameters param(3, 1);
	KD_Hand h(6);
	KD_Row r(5, &h, &param);
	//KD_Set set(-1, 5, 6);
  // Set will create KD_Row and KD_Hand.

short p[15]= { 1, 2, 3, 4, 5, 3, 2, 3, 4, 5, 5, 4, 6, 7, 0 };
memcpy (r.content, p, 15*2);

r.PrintRow();
//printf ("Split %d\n", r.SplitLastBlockAt (r.content+ 5, 2));
printf ("AddAtTop %d\n", r.AddAtTop ((KD_Gem*) 0x1234abcd));
r.PrintRow();

return 0;
*/
    
	KD_Application *app = KD_Application::getApplication();
	if (!app->Init()) return -1;
	if (!app->Loop()) return -1;
	if (!app->Quit()) return -1;
	return 0;
	
}
