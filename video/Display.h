#ifndef DisplayH
#define DisplayH

class Display
{
private:

public:
	static int width;
	static int height;
	
	static void initDisplay(int width, int height, int mode, bool windowed, bool openGL);
	static void deInit();
};

#endif

