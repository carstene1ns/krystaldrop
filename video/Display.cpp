#include "Display.h"

#include "SDL/SDL.h"

#include "font.h"

int Display::width=0;
int Display::height=0;
int Display::lastTime=0;
int Display::ticks=0;
float Display::timeElapsed=0.0f;


KD_Font *Display::Slapstick=0;

SDL_Surface *Display::screen=0;

/**
	init the display: opens a window, and set openGL flags.
	the OpenGL flag is not yet implemented.
  */
void Display::initDisplay(int width, int height, int bits, bool windowed, bool openGL)
{	
	Display::width=width;
	Display::height=height;

	SDL_InitSubSystem(SDL_INIT_VIDEO);

	// The opengl flag prevents usage of the usual 2D SDL.
	// Use SDL_OPENGLBLIT to use 2D SDL functions.
	// Warning! Use of SDL_OPENGL generates bugs on the texture loading (colors especially).
	// Uint32 flags = SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_OPENGLBLIT;
	Uint32 flags = SDL_HWSURFACE | /*SDL_ANYFORMAT |*/ SDL_DOUBLEBUF;
	
	if (!windowed) flags |= SDL_FULLSCREEN;

	screen = SDL_SetVideoMode(width, height, bits, flags);

	ticks = SDL_GetTicks();

	lastTime = ticks;

	setApplicationName("Krystal Drop");

	Slapstick = new KD_Font("art/Slapstick.txt");
	Slapstick->convertToColorKey(0,128);
}

void Display::deInit()
{
	delete Slapstick;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

/**
	Flips the back-buffer and the front-buffer
  */
void Display::flip()
{
	SDL_Flip(screen);

	lastTime = ticks;
	ticks = SDL_GetTicks();
	timeElapsed = ((float)ticks-lastTime)/1000.0f;
}

void Display::clearScreen()
{
	SDL_FillRect(screen, 0, 0);
}

void Display::setApplicationName(char *name)
{
	SDL_WM_SetCaption(name, 0);
}


void Display::DisplayFramesPerSecond (int x, int y, int refresh_rate)
{ static int count= 0;
  static float elapsed= 0.0;
  static int fps= 0;
  
  count++;
  elapsed+= Display::timeElapsed;
  if (count>= refresh_rate)
  { fps= (int) (count/elapsed);
    count= 0;
    elapsed= 0; 
  }
      
  if (fps> 0) Display::Slapstick->xyprintf (x, y, "FPS:%d", fps);
}

int Display::getTimeSlice(int timeQuantum)
{
	return (ticks-ticks%timeQuantum-(lastTime-lastTime%timeQuantum))/timeQuantum;
}
