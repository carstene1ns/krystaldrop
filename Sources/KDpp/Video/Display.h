#ifndef DisplayH
#define DisplayH

struct SDL_Surface;
//class KD_Font;

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
//	static KD_Font *Slapstick;

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

	static void flip();
	/**
		Sets the background color to black (or Display a white screen for a flash)
	*/
	static void clearScreen();

	/**
		Sets the application name in the application window
	*/
	static void setApplicationName(char *name);
  
	/**
		Sets the application icon in the application window
		This method must be called AFTER the InitDisplay method
		bmpName is the name of a bmp or ico file. size: 32x32 pixels

		PROBLEM: I don't know WHEN I should free the icon? But shall I do it?
	*/
	static void setApplicationIcon(char *bmpName);


	/**
		Returns the number of timeQuantum elapsed since lastFrame.
		getTimeSlice will round the timeslice correctly in order to avoid returning, for example, always 0.
		WARNING: timeQuantum must be specified in number of milliseconds.
	*/
	static int getTimeSlice(int timeQuantum);

	static void setClipRect(int x1, int y1, int x2, int y2);

	/**
		Returns the width of the screen in pixels
	*/
	static int getWidth();
	
	/**
		Returns the height of the screen in pixels
	*/
	static int getHeight();

	/**
		Returns whether the OpenGL mode is activated or not
	*/
	static bool getIsOpenGL();

	/**
		Returns the time elapsed between 2 frames
	*/
	static float getTimeElapsed();

	/**
		Returns the number of ticks elapsed since the application's beginning
	*/
	static int getTicks();

	/**
		Sets the clear color
	*/
	static void setClearColor(const KD_Color &color);
};

#endif

