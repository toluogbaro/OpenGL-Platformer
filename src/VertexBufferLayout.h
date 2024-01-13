#pragma once

#include <GL/glew.h>
#include <vector>


struct VertexBufferElement
{
    unsigned int type; //GLenum type
    unsigned int count; // how many parts there are of each element. ex 2 floats in an x, y position vertex
    unsigned char normalized; // boolean

    static unsigned int GetSizeofType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;

        }

        return 0;
    }
};

class VertexBufferLayout
{
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

    public: 
        VertexBufferLayout()
            : m_Stride(0) {}


        //these templates take in a type and push to the elements list based on type

        template<typename T>
        void Push(unsigned int count)
        {
           // static_assert(false);
        }

        template<>
        void Push<float>(unsigned int count)
        {
            m_Elements.push_back({GL_FLOAT, count, GL_FALSE });
            m_Stride += VertexBufferElement::GetSizeofType(GL_FLOAT) * count;
        }

        template<>
        void Push<unsigned int>(unsigned int count)
        {
            m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE });
            m_Stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT) * count;
        }

        template<>
        void Push<unsigned char>(unsigned int count)
        {
            m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
            m_Stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE) * count;
        }

        void Clear()
        {
            m_Elements.clear();
        }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; } // return current element list of this object
        inline unsigned int GetStride() const { return m_Stride; } // return stride of this object


};
