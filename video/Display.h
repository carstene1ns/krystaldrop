#ifndef DisplayH
#define DisplayH

struct SDL_Surface;

class Display
{
private:

public:
	static int width;
	static int height;
	static SDL_Surface *screen;

	/**
		Number of ticks elapsed since last frame.
	*/
	static int ticks;
	
	static void initDisplay(int width, int height, int mode, bool windowed, bool openGL);
	static void deInit();

	static void flip();
};

#endif

