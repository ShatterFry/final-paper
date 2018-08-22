#include <iostream>

#include <IndexBuffer.h>
#include <Renderer.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint))

	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
		data, GL_STATIC_DRAW));

	/*std::cout << "IndexBuffer::IndexBuffer()\n";
	std::cout << "&m_RendererID = " << &m_RendererID << std::endl;
	std::cout << "m_RendererID = " << m_RendererID << std::endl;
	std::cout << "sizeof(insugned int) = " << sizeof(unsigned int)
		<< std::endl;
	std::cout << "data = " << data << std::endl;*/
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));

	//std::cout << "IndexBuffer::~IndexBuffer()\n";
	//std::cout << "&m_RendererID = " << &m_RendererID << std::endl;
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

	//std::cout << "IndexBuffer::Bind()\n";
	//std::cout << "m_RendererID = " << m_RendererID << std::endl;
}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	//std::cout << "IndexBuffer::Unbind()\n";
}