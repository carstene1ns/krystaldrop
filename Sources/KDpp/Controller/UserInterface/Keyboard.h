#ifndef Keyboard_H
#define Keyboard_H

#include "SDL/SDL.h"

#include "../../Tools/defines.h"


/**
	A class to retrieve the state of the Keyboard. It should only be used to read letters.
	To read events on the keyboard, use rather Controllers.
  */
class DllExport KD_Keyboard
{
	static KD_Keyboard *singleton;

	unsigned short last_unicode;
	SDLKey last_sym;


	KD_Keyboard();
	virtual ~KD_Keyboard();

public:
	/**
		Static function returning the Keyboard.
	*/
	static KD_Keyboard *GetKeyboard();

	/**
		Instanciate the singleton
	*/
	static KD_Keyboard *InitKeyboard();

	/**
		Delete the singleton
	*/
	static bool CloseKeyboard();

	/**
		Returns the ascii character of the key that has been pressed.
		Returns 0 if no key has been pressed.
	*/
	unsigned short GetAsciiCharacter();

	/**
		Sets the last key pressed to 0.
		This method should be called on each frame, before handling events.
	*/
	void ReSetLastKey();

	/**
		take an SDL_Event and sets the last key to this value.
		assuming SDL_Event is a key down event.
	*/
	void SetLastKey(const SDL_Event &event);

	/**
		The last SDLKey pressed.
		For the key to be valid, the value of GetAsciiCharacter() must be not NULL.
	*/
	SDLKey GetLastSDLKey();

	/**
		Returns the key name from the key code
		This method is static
	*/
	static char *GetKeyName(int key);
};

#endif
