#include "Renderer.h"
#include <iostream>
#include "Errors.h"

void Renderer::Clear() const
{
    FFunctionCallback funcToCall = []()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    };
	GLCall(funcToCall);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, std::shared_ptr<Shader>& shader, EDrawTypes drawType) const
{
	shader->Bind();
	va.Bind();
	ib.Bind();

	switch (drawType)
	{
        case EDrawTypes::EDT_TRIANGLES:
        {
            FFunctionCallback funcCB = [&]()
            {
                glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
            };
            GLCall(funcCB);
            break;
        }
        case EDrawTypes::EDT_LINES:
        {
            FFunctionCallback funcCB = [&]()
            {
                glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
            };
            GLCall(funcCB);
            break;
        }
	}

}
