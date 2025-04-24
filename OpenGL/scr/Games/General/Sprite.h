#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../Libraries/stb_image.h"

struct Sprite {
	GLuint Image = 0;

	float Scale = 1;
	float Angle = 0;
	float* Position[3] = { 0, 0, 0 };

	Sprite(GLuint texture) : Image(texture) {}
};