#ifdef _WIN32
#include <windows.h>
#endif
#ifndef NO_OPENGL
#include <GL/gl.h>
#endif

#include "SDL/SDL.h"
#include "Display.h"
#include "font.h"

int Display::width=0;
int Display::height=0;
int Display::lastTime=0;
float Display::flashTime=0.0f;
int Display::ticks=0;
bool Display::isOpenGL=false;
float Display::timeElapsed=0.0f;

#define FLASHTIME 0.25
#define FLASHMUL (1.0f/FLASHTIME)

KD_Font *Display::Slapstick=0;

SDL_Surface *Display::screen=0;

#ifndef NO_OPENGL
void Display::initOpenGL(int width, int height)
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
	
	//glAlphaFunc(GL_GEQUAL, 0.05f); 
	//glEnable(GL_ALPHA_TEST); 
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}
#endif

/**
	init the display: opens a window, and set openGL flags.
	the OpenGL flag is not yet implemented.
  */
void Display::initDisplay(int width, int height, int bits, bool windowed, bool openGL)
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
	if (openGL) initOpenGL(width,height);
#endif	

	ticks = SDL_GetTicks();
	lastTime = ticks;

	setApplicationName("Krystal Drop");

	SDL_ShowCursor(SDL_DISABLE);

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
#ifndef NO_OPENGL
	if (isOpenGL) SDL_GL_SwapBuffers(); else
#endif      
      SDL_Flip(screen);

	lastTime = ticks;
	ticks = SDL_GetTicks();
	timeElapsed = ((float)ticks-lastTime)/1000.0f;
}

void Display::clearScreen()
{ 
#ifndef NO_OPENGL
  if (isOpenGL) 
    { if (flashTime> 0)
      { flashTime-= timeElapsed;
        if (flashTime< 0) flashTime= 0;
        float col= flashTime* FLASHMUL;
        glClearColor (col, col, col, 1);
      }
      glClear(GL_COLOR_BUFFER_BIT);
    }
  else
#endif    
  { if (flashTime> 0)
    { flashTime-= timeElapsed;
      if (flashTime< 0) flashTime= 0;
      unsigned long col= (long) (flashTime* FLASHMUL* 255);
      unsigned long c= (col*256+ col)*256+ col;
      SDL_FillRect (screen, 0, c);
      /* I don't think this is `safe', depending on the kind of SDL surface */
      /* SDL_MapRGBA may be a better idea */
      /* on the other hand, I don't really mind the colour ;p */
    }
    else
    SDL_FillRect(screen, 0, 0);
  }
}

void Display::setApplicationName(char *name)
{
	SDL_WM_SetCaption(name, 0);
}


#ifdef DISPLAY_FPS
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
#endif

int Display::getTimeSlice(int timeQuantum)
{
	return (ticks-ticks%timeQuantum-(lastTime-lastTime%timeQuantum))/timeQuantum;
}

void Display::setClipRect(int x1, int y1, int x2, int y2)
{
#ifndef NO_OPENGL  
  if (isOpenGL)
  { glViewport(x1, y1, x2-x1, y2-y1); // Reset The Current Viewport And Perspective Transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(x1,x2, y1, y2, 0.01,10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  } else
#endif        
	{	SDL_Rect rect;
      
		rect.x = x1;
		rect.y = y1;
		rect.w = x2-x1;
		rect.h = y2-y1;
		SDL_SetClipRect(screen, &rect);
	}
}

void Display::Flash()
{ flashTime= FLASHTIME; 
}
