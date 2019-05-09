#pragma once

#include "../../dependencies/glew/include/GL/glew.h"
#include <cassert>

//#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
	x;\
	assert(GLLogCall(#x, __FILE__, __LINE__));
	//ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
