//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/n4k7ANAFsIQ?feature=shared

#include "Texture.h"
#include "vendor/stb_image/stb_image.h"


Texture::Texture(const std::string& path, bool isCubemap)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_Bits(0)
{


	if (!isCubemap)
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Bits, 4); //4 is because of rgba

		glGenTextures(1, &m_RendererID); 
		glBindTexture(GL_TEXTURE_2D, m_RendererID); 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // gets x 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // gets y

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	else
	{
		std::string cubemapFaces[6] =
		{
			path,
			path,
			path,
			path,
			path,
			path,

		};

		for (int i = 0; i < cubemapFaces->size(); i++)
		{
			m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Bits, 0); //4 is because of rgba
			stbi_set_flip_vertically_on_load(0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

