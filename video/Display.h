#ifndef DisplayH
#define DisplayH

struct SDL_Surface;
class KD_Font;

/**
	Static class used to instanciate the Video subsystem of SDL, and other video-related stuff.
	It is also used as a Timer depending on the framerate.
  */
class Display
{
private:
	/**
		Time of the last frame.
	*/
	static int lastTime;



public:
	static int width;
	static int height;
	static SDL_Surface *screen;

	static KD_Font *Slapstick;

	/**
		Number of ticks elapsed since last frame.
	*/
	static int ticks;

	/**
		Number of seconds between the last frame and the frame before
	*/
	static float timeElapsed;
	
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
    
    static void DisplayFramesPerSecond (int x, int y, int refresh_rate);

	/**
		Returns the number of timeQuantum elapsed since lastFrame.
		getTimeSlice will round the timeslice correctly in order to avoid returning, for example, always 0.
		WARNING: timeQuantum must be specified in number of milliseconds.
	*/
	static int getTimeSlice(int timeQuantum);
};

#endif

