#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

/*
 start point (depicted by "*")
  *_____________________
  |                    |
  |                    |
  |                    |
  |                    |
  |____________________* end point (depicted by "*")


  pt.1                 pt.2
  ______________________
  |                    |
  |                    |
  |                    |
  |                    |
  |____________________|

  pt.3                 pt.4
*/

namespace GLGame
{
	struct Quad
	{
		float x1;
		float y1;
		float x2;
		float y2;
	};
}
