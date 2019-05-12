#include <iostream>
#include "Errors.h"

void GLCall(const FFunctionCallback& funcRef)
{
    GLClearError();
    funcRef();

    std::string testFuncName("Test function name");
    std::string testFileName("Test file name");
    int testLineNum = 42;

    assert(GLLogCall(testFuncName.c_str(), testFileName.c_str(), testLineNum));
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
    {
		std::cout << "[OpenGL error] (" << error << "): " << function
			<< " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}
