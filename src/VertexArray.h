#pragma once

//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/oD1dvfbyf6A?feature=shared

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
	private:
		unsigned int m_RendererID;
		unsigned int m_VertexArrayCount;
		

	public:
		VertexArray();
		~VertexArray();

		
		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;

};