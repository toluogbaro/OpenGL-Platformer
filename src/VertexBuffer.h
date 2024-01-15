//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/bTHqmzjm2UI?feature=shared
#pragma once

#include "GL/glew.h"

class VertexBuffer
{
	private:
		unsigned int m_RendererID;

	public:
	
		VertexBuffer(unsigned int size, const void* data);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

};


