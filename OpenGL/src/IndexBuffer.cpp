#include <iostream>
#include "IndexBuffer.h"
#include "Errors.h"

//IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
IndexBuffer::IndexBuffer(const std::shared_ptr<UIntVector>& data)
	: m_Count(data->size())
{
	//m_Count = data->size();

	assert(sizeof(unsigned int) == sizeof(GLuint));

    {
        FFunctionCallback functionToCall = [&]()
        {
            glGenBuffers(1, &m_RendererID);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback functionToCall = [&]()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        };
        GLCall(functionToCall);
    }

    {
        FFunctionCallback funcToCall = [&]()
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size() * sizeof(unsigned int), data->data(), GL_STATIC_DRAW);
        };
        GLCall(funcToCall);
    }
}

IndexBuffer::~IndexBuffer() {
    FFunctionCallback functionToCall = [&]()
    {
        glDeleteBuffers(1, &m_RendererID);
    };
	GLCall(functionToCall);

	//std::cout << "IndexBuffer::~IndexBuffer()\n";
	//std::cout << "&m_RendererID = " << &m_RendererID << std::endl;
}

void IndexBuffer::Bind() const {
    FFunctionCallback functionToCall = [&]()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    };
	GLCall(functionToCall);

	//std::cout << "IndexBuffer::Bind()\n";
	//std::cout << "m_RendererID = " << m_RendererID << std::endl;
}

void IndexBuffer::Unbind() const
{
    FFunctionCallback functionToCall = []()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };
	GLCall(functionToCall);

	//std::cout << "IndexBuffer::Unbind()\n";
}
