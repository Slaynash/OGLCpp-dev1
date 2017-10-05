#include "RenderScene.h"
#include <iostream>

int main()
{
	RenderScene scene("OpenGL Test Scene", 800, 800);

	if(scene.initWindow() == false)
		return -1;

  if(scene.initGL() == false)
		return -1;

	scene.run();
	std::cout << "end of program" << std::endl;
	return 0;
}
