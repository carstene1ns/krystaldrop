#include "config.h"
#include "game/hand.h"
#include "game/parameter.h"
#include "interf/Application.h"

int main (int argc, char* argv[])
{ KD_Application* app= KD_Application::getApplication();
  KD_Config* config= KD_Config::GetConfig();  
  
  if (!app->Init()) return -1;
  if (!app->Loop()) return -1;
  if (!app->Quit()) return -1;
    
  delete config;  
  delete app;
 
  return 0;
}
