#ifndef DisplayH
#define DisplayH

struct SDL_Surface;

#include "../Tools/defines.h"
#include "Color.h"

/**
	Static class used to instanciate the Video subsystem of SDL, and other video-related stuff.
	It is also used as a Timer depending on the framerate.
  */
class DllExport Display
{ private:
	/**
		Time of the last frame.
	*/
	static int lastTime;

  public:
	static int width;
	static int height;
	static SDL_Surface *screen;

	static KD_Color clearColor;

	/**
		Is OpenGL activated or not?
	*/
	static bool isOpenGL;

	/**
		Number of ticks elapsed since starting of application.
	*/
	static int ticks;

	/**
		Number of seconds between the last frame and the frame before
	*/
	static float timeElapsed;
	
	/**
		Init the 2D OpenGL stuff
	*/
#ifndef NO_OPENGL	
	static void InitOpenGL(int width, int height);
#endif	

	static void InitDisplay(int width, int height, int mode, bool windowed, bool openGL);
	static void deInit();

	static void Flip();
	/**
		Sets the background color to black (or Display a white screen for a flash)
	*/
	static void ClearScreen();

	/**
		Sets the application name in the application window
	*/
	static void SetApplicationName(char *name);
  
	/**
		Sets the application icon in the application window
		This method must be called AFTER the InitDisplay method
		bmpName is the name of a bmp or ico file. size: 32x32 pixels

		PROBLEM: I don't know WHEN I should free the icon? But shall I do it?
	*/
	static void SetApplicationIcon(char *bmpName);


	/**
		Returns the number of timeQuantum elapsed since lastFrame.
		getTimeSlice will round the timeslice correctly in order to avoid returning, for example, always 0.
		WARNING: timeQuantum must be specified in number of milliseconds.
	*/
	static int GetTimeSlice(int timeQuantum);

	static void SetClipRect(int x1, int y1, int x2, int y2);

	/**
		Returns the width of the screen in pixels
	*/
	static int GetWidth();
	
	/**
		Returns the height of the screen in pixels
	*/
	static int GetHeight();

	/**
		Returns whether the OpenGL mode is activated or not
	*/
	static bool GetIsOpenGL();

	/**
		Returns the time elapsed between 2 frames
	*/
	static float GetTimeElapsed();

	/**
		Returns the number of ticks elapsed since the application's beginning
	*/
	static int GetTicks();

	/**
		Sets the clear color
	*/
	static void SetClearColor(const KD_Color &color);
};

#endif

