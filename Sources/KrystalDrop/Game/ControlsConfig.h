#ifndef ControlsConfig_H
#define ControlsConfig_H

#include <string>
using namespace std;

/**
	Class storing the results from the configuration of the keys/joystick
	retrieved from config-file
*/
class KD_ControlsConfig
{
	static KD_ControlsConfig *singleton;
  
 	KD_ControlsConfig();
	~KD_ControlsConfig();
 
public:
	enum KD_Keys
	{
		p1up = 0,
		p1down,
		p1left,
		p1right,
		p1extra,
		p2up,
		p2down,
		p2left,
		p2right,
		p2extra
	};

#define KD_NbKeysDefined 10

private:
	int keyCode[KD_NbKeysDefined];

	/**
		Kind of the control (keyboard or joystick)
			CONTROLLER_EVENT_KEYDOWN  1
			CONTROLLER_EVENT_JOYSTICK 2
	*/
	int controlKind[KD_NbKeysDefined];

	void getKeyCodeFromConfig(const string &tag, KD_Keys key);

public:
	static KD_ControlsConfig *GetSingleton();

	/**
		Inits file from the config file.
	*/
	void InitFromConfigFile();

	/**
		Close the config, destroys the object
	*/
	void Close();

	/**
		Sets the key code, and saves it!
	*/
	void SetKeyCode(KD_Keys key, int myKeyCode);

	/**
		Returns the control code
	*/
	int GetControlCode(KD_Keys key);

	/**
		Returns the control kind
	*/
	int GetControlKind(KD_Keys key);

	/**
		Returns the "friendly name" of the key
	*/
	string GetControlName(KD_Keys key);
};

#endif
