#include "Display.h"

#include "SDL/SDL.h"

#include "font.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

int Display::width=0;
int Display::height=0;
int Display::lastTime=0;
int Display::ticks=0;
float Display::timeElapsed=0.0f;
bool Display::isOpenGL=false;

KD_Font *Display::Slapstick=0;

SDL_Surface *Display::screen=0;

void Display::initOpenGL(int width, int height)
{
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glDisable (GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, width, height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,Display::width, 0, Display::height, 0.01,10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glAlphaFunc(GL_GEQUAL, 0.05f); 
	//glEnable(GL_ALPHA_TEST); 
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

/**
	init the display: opens a window, and set openGL flags.
	the OpenGL flag is not yet implemented.
  */
void Display::initDisplay(int width, int height, int bits, bool windowed, bool openGL)
{	
	isOpenGL = openGL;
	Display::width=width;
	Display::height=height;

	SDL_InitSubSystem(SDL_INIT_VIDEO);

	// The opengl flag prevents usage of the usual 2D SDL.
	// Use SDL_OPENGLBLIT to use 2D SDL functions.
	// Warning! Use of SDL_OPENGL generates bugs on the texture loading (colors especially).
	// Uint32 flags = SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_OPENGLBLIT;
	Uint32 flags = SDL_HWSURFACE | /*SDL_ANYFORMAT |*/ SDL_DOUBLEBUF;
	
	if (!windowed) flags |= SDL_FULLSCREEN;
	if (openGL) flags |= SDL_OPENGLBLIT;

	screen = SDL_SetVideoMode(width, height, bits, flags);

	if (openGL) initOpenGL(width,height);

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
	if (isOpenGL)
		SDL_GL_SwapBuffers();
	else
		SDL_Flip(screen);

	lastTime = ticks;
	ticks = SDL_GetTicks();
	timeElapsed = ((float)ticks-lastTime)/1000.0f;
}

void Display::clearScreen()
{
	if (isOpenGL)
		glClear(GL_COLOR_BUFFER_BIT);
	else
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

void Display::setClipRect(int x1, int y1, int x2, int y2)
{
	if (isOpenGL)
	{
		glViewport(x1, y1, x2-x1, y2-y1);		// Reset The Current Viewport And Perspective Transformation

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x1,x2, y1, y2, 0.01,10000);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else
	{
		SDL_Rect rect;
		rect.x = x1;
		rect.y = y1;
		rect.w = x2-x1;
		rect.h = y2-y1;
		SDL_SetClipRect(screen, &rect);
	}


}
