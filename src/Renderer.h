#include <GL/glew.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
	private:

	public:

		void Clear();

		void DrawObject(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

		void UnbindAll(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;


};

