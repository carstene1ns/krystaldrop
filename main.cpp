#include "game/hand.h"
#include "game/parameter.h"
#include "game/row.h"
#include "game/set.h"
#include "SDL/SDL.h"
#include "interf/Application.h"

#include "video/gem.h"
/* temporary */

int main( int argc, char* argv[] )
{
    
// debug
KD_Parameters* param= new KD_Parameters();
 param->SetVideoParameters (30,200, 0);
 param->SetGameParameters (1, 0, 1, 1, 2, -1);
KD_Hand* hand= new KD_Hand(6);
KD_Row* row= new KD_Row(8, hand, param);
// KD_Gem* g= new KD_Gem(NULL, 1);
  
short p[15]= { 1, 2, 3, 4, 5, 3, 2, 3, 4, 5, 5, 4, 6, 7, 0 };
memcpy (row->content, p, 15*2);

row->PrintRow();

printf ("Split %d\n", row->JoinBlocks (row->content));
row->PrintRow();

return 0;

    
	KD_Application *app = KD_Application::getApplication();
	if (!app->Init()) return -1;
	if (!app->Loop()) return -1;
	if (!app->Quit()) return -1;
	return 0;
	
}
