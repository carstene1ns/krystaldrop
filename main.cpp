#include "game/hand.h"
#include "game/row.h"
#include "game/set.h"
#include "SDL/SDL.h"
#include "interf/Application.h"

int main( int argc, char* argv[] )
{
	KD_Application *app = KD_Application::getApplication();
	if (!app->Init()) return -1;
	if (!app->Loop()) return -1;
	if (!app->Quit()) return -1;
	return 0;
	
/*	KD_Hand h(6);
	KD_Row r(3, &h);

short p[12]= { 2, 2, 3, 42, 12, 12, 42, 0, 0, 0, 0 };

memcpy (r.content, p, 12*2);

r.AddAtTop (NULL);
*/
}
