#pragma once

//#include <memory>
#include <vector>
#include "Aliases.h"

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	//IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const std::shared_ptr<UIntVector>& data);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
