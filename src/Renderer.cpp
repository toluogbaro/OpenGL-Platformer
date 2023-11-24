#include "Renderer.h"
#include <iostream>



void Renderer::DrawObject(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    //shader.Bind();
    //shader.SetUniform4f("u_Colour", r, 0.6f, 0.8f, 1.0f);

    va.Bind(); //bind vertex array
    ib.Bind(); //bind index buffer

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::UnbindAll(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Unbind();
    ib.Unbind();
    shader.Unbind();
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

