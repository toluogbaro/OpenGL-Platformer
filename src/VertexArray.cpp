#include "VertexArray.h"



VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);

}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{

	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	//automates the process for adding a buffer and adding vertex buffer elements

	Bind();
	vb.Bind(); // bind vertex buffer
	const auto& elements = layout.GetElements(); // returns layout vector of elements
	unsigned int offset = 0;

	    // the vertex attrib pointer is binding this buffer with the vertex array object.
		// its essentially a list of buffers that the pointer is choosing from
		// instead of connecting multiple buffers to a default array pointer,
		// all we do is change the array index in the pointer to the object we've created

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i]; // element object
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);

		//i = the index of the vertex buffer array
		//count is how many parts there are in each element, for a float of vertex positions, thats 2. x and y
		//element type takes in the enum type, like GL_FLOAT for example
		//normalized flips the int representing the bool
		//Get stride, gets the data between each element part using its type

		offset += element.count * VertexBufferElement::GetSizeofType(element.type);
	}
	
}