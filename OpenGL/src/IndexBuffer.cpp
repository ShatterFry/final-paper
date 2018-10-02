#include <iostream>
#include <IndexBuffer.h>
#include <Errors.h>

//IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
IndexBuffer::IndexBuffer(const std::shared_ptr<std::vector<unsigned int>>& data)
	: m_Count(data->size())
{
	//m_Count = data->size();

	ASSERT(sizeof(unsigned int) == sizeof(GLuint))

	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size() * sizeof(unsigned int), data->data(), GL_STATIC_DRAW));
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