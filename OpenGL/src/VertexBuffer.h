#pragma once

#include <vector>
#include <memory>

#include "Errors.h"

template <class T>
class VertexBuffer {
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const std::shared_ptr<std::vector<T>>& data)
	{
	    {
	        FFunctionCallback funcToCall = [&]()
            {
                glGenBuffers(1, &m_RendererID);
            };
            GLCall(funcToCall);
	    }

	    {
	        FFunctionCallback funcToCall = [&]()
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
            };
            GLCall(funcToCall);
	    }

        {
            FFunctionCallback funcToCall = [&]()
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data->size(), data->data(), GL_STATIC_DRAW);
            };
            GLCall(funcToCall);
        }
	}

	~VertexBuffer()
	{
	    FFunctionCallback funcToCall = [&]()
	    {
	        glDeleteBuffers(1, &m_RendererID);
	    };
		GLCall(funcToCall);
	}

	void Bind() const
	{
	    FFunctionCallback functionToCall = [&]()
	    {
	        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	    };
		GLCall(functionToCall);
	}

	void Unbind() const
	{
	    FFunctionCallback funcToCall = []()
	    {
	        glBindBuffer(GL_ARRAY_BUFFER, 0);
	    };
		GLCall(funcToCall);
	}
};
