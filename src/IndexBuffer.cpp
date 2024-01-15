//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/MXNMC1YAxVQ?feature=shared

#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    _ASSERT(sizeof(unsigned int) == sizeof(GLuint)); //make sure size of unsigned int isnt -1

    glGenBuffers(1, &m_RendererID); //generate index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}