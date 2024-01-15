//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/bTHqmzjm2UI?feature=shared

#include "Renderer.h"
#include <iostream>


Renderer::Renderer()
{

}

void Renderer::DrawObject(VertexArray& va, const IndexBuffer& ib) const
{

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

void Renderer::EnableAll() const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

