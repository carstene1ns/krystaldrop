#include "Keyboard.h"

KD_Keyboard *KD_Keyboard::singleton=0;

KD_Keyboard::KD_Keyboard()
{
	SDL_EnableUNICODE(true);
}

KD_Keyboard::~KD_Keyboard()
{
	SDL_EnableUNICODE(false);
}

KD_Keyboard *KD_Keyboard::GetKeyboard()
{
	return singleton;
}

KD_Keyboard *KD_Keyboard::InitKeyboard()
{
	if (singleton==0)
		singleton = new KD_Keyboard();
	return singleton;
}

bool KD_Keyboard::CloseKeyboard()
{
	if (!singleton) return false;
	delete singleton;
	singleton = 0;
	return true;
}

unsigned short KD_Keyboard::GetAsciiCharacter()
{
	/*if ( (last_unicode & 0xFF80) == 0 )
	{
		return last_unicode & 0x7F;
	}
	else 
	{
		return 0;
		//printf("An International Character.\n");
	}*/
	return last_unicode;
}

void KD_Keyboard::ReSetLastKey()
{
	last_unicode=0;
}

void KD_Keyboard::SetLastKey(const SDL_Event &event)
{
	last_unicode = event.key.keysym.unicode;
	last_sym = event.key.keysym.sym;
}

SDLKey KD_Keyboard::GetLastSDLKey()
{
	return last_sym;
}

char *KD_Keyboard::GetKeyName(int key)
{
	return SDL_GetKeyName((SDLKey)key);
}
