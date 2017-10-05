#include "RenderScene.h"


int main()
{
	RenderScene scene("OpenGL Test Scene", 800, 800);

	if(scene.initWindow() == false)
		return -1;

  if(scene.initGL() == false)
		return -1;

	scene.run();

	return 0;
}
