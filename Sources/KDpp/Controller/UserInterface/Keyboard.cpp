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

KD_Keyboard *KD_Keyboard::getKeyboard()
{
	return singleton;
}

KD_Keyboard *KD_Keyboard::InitKeyboard()
{
	if (singleton==0)
		singleton = new KD_Keyboard();
	return singleton;
}

bool KD_Keyboard::closeKeyboard()
{
	if (!singleton) return false;
	delete singleton;
	singleton = 0;
	return true;
}

unsigned short KD_Keyboard::getAsciiCharacter()
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

void KD_Keyboard::resetLastKey()
{
	last_unicode=0;
}

void KD_Keyboard::setLastKey(const SDL_Event &event)
{
	last_unicode = event.key.keysym.unicode;
	last_sym = event.key.keysym.sym;
}

SDLKey KD_Keyboard::getLastSDLKey()
{
	return last_sym;
}

char *KD_Keyboard::getKeyName(int key)
{
	return SDL_GetKeyName((SDLKey)key);
}

