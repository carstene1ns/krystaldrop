#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif
#ifndef NO_OPENGL
#include <GL/gl.h>
#endif

#include "SDL/SDL.h"
#include "Display.h"

int Display::width=0;
int Display::height=0;
int Display::lastTime=0;
int Display::ticks=0;
bool Display::isOpenGL=false;
float Display::timeElapsed=0.0f;
KD_Color Display::clearColor=KD_Color();

SDL_Surface *Display::screen=0;

#ifndef NO_OPENGL
void Display::InitOpenGL(int width, int height)
{
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, width, height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,Display::width, 0, Display::height, 0, 10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}
#endif

/**
	Init the Display: opens a window, and set openGL flags.
	the OpenGL flag is not yet implemented.
  */
void Display::InitDisplay(int width, int height, int bits, bool windowed, bool openGL)
{	
	isOpenGL= openGL;
#ifdef NO_OPENGL
    if (isOpenGL== true)
    { fprintf (stderr, "Impossible to setup OpenGL: Krystal Drop has been compiled without OpenGL support.\n");
      exit (1);
    }
#endif
	Display::width= width;
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

#ifndef NO_OPENGL
	if (openGL) InitOpenGL(width,height);
#endif	

	ticks = SDL_GetTicks();
	lastTime = ticks;

//	SDL_ShowCursor(SDL_DISABLE);
}

void Display::deInit()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

/**
	Flips the back-buffer and the front-buffer
  */
void Display::Flip()
{
#ifndef NO_OPENGL
	if (isOpenGL) SDL_GL_SwapBuffers();
	else
#endif      
    SDL_Flip(screen);

	lastTime = ticks;
	ticks = SDL_GetTicks();
	timeElapsed = ((float)ticks-lastTime)/1000.0f;
}

void Display::ClearScreen()
{ 
#ifndef NO_OPENGL
	if (isOpenGL) 
    { 
		glClear(GL_COLOR_BUFFER_BIT);
    }
	else
#endif    
	{
		unsigned long c= (unsigned long)((clearColor.r*256.0f*256+ clearColor.g*256.0f)*256+ clearColor.b*256.0f)*256;
		SDL_FillRect (screen, 0, c);
		/* I don't think this is `safe', depending on the kind of SDL surface */
		/* SDL_MapRGBA may be a better idea */
		/* on the other hand, I don't really mind the colour ;p */
		//SDL_FillRect(screen, 0, 0);
	}
}

void Display::SetApplicationName(char *name)
{
	SDL_WM_SetCaption(name, 0);
}

void Display::SetApplicationIcon(char *bmpName)
{
	SDL_Surface *surf = SDL_LoadBMP(bmpName);
	SDL_WM_SetIcon(surf, NULL);
}

/*void Display::DisplayFramesPerSecond (int x, int y, int refresh_rate)
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
}*/

int Display::GetTimeSlice(int timeQuantum)
{
	return (ticks-ticks%timeQuantum-(lastTime-lastTime%timeQuantum))/timeQuantum;
}

void Display::SetClipRect(int x1, int y1, int x2, int y2)
{
#ifndef NO_OPENGL  
	if (isOpenGL)
	{ 
		glViewport(x1, y1, x2-x1, y2-y1); // Reset The Current Viewport And Perspective Transformation
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(x1,x2, y1, y2, 0.01,10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	} 
	else
#endif        
	{	
		SDL_Rect rect;
      	rect.x = x1;
		rect.y = y1;
		rect.w = x2-x1;
		rect.h = y2-y1;
		SDL_SetClipRect(screen, &rect);
	}
}

int Display::GetWidth()
{
	return width;
}
	
int Display::GetHeight()
{
	return height;
}

bool Display::GetIsOpenGL()
{
	return isOpenGL;
}

float Display::GetTimeElapsed()
{
	return timeElapsed;
}

void Display::SetClearColor(const KD_Color &color)
{
	clearColor=color;
#ifndef NO_OPENGL
	if (isOpenGL) 
		glClearColor (clearColor.r, clearColor.g, clearColor.b, 1);
#endif
}

int Display::GetTicks()
{
	return ticks;
}
