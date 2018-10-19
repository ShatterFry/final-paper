#include <Renderer.h>
#include <iostream>
#include <Errors.h>

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, std::shared_ptr<Shader>& shader, EDrawTypes drawType) const
{
	shader->Bind();
	va.Bind();
	ib.Bind();

	switch (drawType)
	{
	case EDrawTypes::EDT_TRIANGLES:
		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
		break;
	case EDrawTypes::EDT_LINES:
		GLCall(glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
		break;
	}
	
}