//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/bTHqmzjm2UI?feature=shared

#include <GL/glew.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{

	public:
		Renderer();

		void Clear();

		void DrawObject(VertexArray& va, const IndexBuffer& ib) const;

		void UnbindAll(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

		void EnableAll() const;


};

