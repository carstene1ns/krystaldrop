#ifndef DisplayH
#define DisplayH

struct SDL_Surface;
class KD_Font;

/**
	Static class used to instanciate the Video subsystem of SDL, and other video-related stuff.
  */
class Display
{
private:

public:
	static int width;
	static int height;
	static SDL_Surface *screen;

	static KD_Font *Slapstick;

	/**
		Number of ticks elapsed since last frame.
	*/
	static int ticks;
	
	static void initDisplay(int width, int height, int mode, bool windowed, bool openGL);
	static void deInit();

	static void flip();
	/**
		Sets the background color to black
	*/
	static void clearScreen();

	/**
		Sets the application name in the application window
	*/
	static void setApplicationName(char *name);
};

#endif

