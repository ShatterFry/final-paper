#pragma once

#include <vector>
#include <memory>
#include <Errors.h>

template <class T>
class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const std::shared_ptr<std::vector<T>>& data)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data->size(), data->data(), GL_STATIC_DRAW));
	}

	~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};