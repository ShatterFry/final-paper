#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

//class VertexBufferLayout;

class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	template<class T>
	void AddBuffer(const VertexBuffer<T>& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();

		const auto& elements = layout.GetElements();

		unsigned int offset = 0;

		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];

            {
                FFunctionCallback funcToCall = [&]()
                {
                    glEnableVertexAttribArray(i);
                };
                GLCall(funcToCall);
            }

            {
                FFunctionCallback functionToCall = [&]()
                {
                    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),
                    (const void*)offset);
                };
                GLCall(functionToCall);
            }

			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}

	void Bind() const;
	void Unbind() const;
};
