#pragma once

#include "../../dependencies/glew/include/GL/glew.h"
#include "Aliases.h"

#include <cassert>
#include <functional>

//#define ASSERT(x) if(!(x)) __debugbreak();

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

void GLCall(const FFunctionCallback& funcRef);

/*#define GLCall(x) \
    (\
        (GLClearError()), \
        (x), \
        (assert(GLLogCall(#x, __FILE__, __LINE__))), \
        (void)0 \
	)
	//ASSERT(GLLogCall(#x, __FILE__, __LINE__))*/
