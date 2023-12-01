#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
	private:
		unsigned int m_RendererID;
		

	public:
		VertexArray();
		~VertexArray();

		//Ampersand is used to modify the actual variable instead of copy
		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;

};