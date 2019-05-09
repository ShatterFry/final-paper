#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

enum class EDrawTypes{EDT_TRIANGLES, EDT_LINES};

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, std::shared_ptr<Shader>& shader, EDrawTypes drawType) const;
};
