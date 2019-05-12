#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    FFunctionCallback funcToCall = [&]()
    {
        glGenVertexArrays(1, &m_RendererID);
    };
	GLCall(funcToCall);
}

VertexArray::~VertexArray()
{
    FFunctionCallback funcToCall = [&]()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    };
    GLCall(funcToCall);
}

void VertexArray::Bind() const
{
    FFunctionCallback funcToCall = [&]()
    {
        glBindVertexArray(m_RendererID);
    };
	GLCall(funcToCall);
}

void VertexArray::Unbind() const
{
    FFunctionCallback funcCB = []()
    {
        glBindVertexArray(0);
    };
	GLCall(funcCB);
}
