//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/n4k7ANAFsIQ?feature=shared

#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
	private:

		unsigned int m_RendererID;

		std::string m_FilePath;

		unsigned char* m_LocalBuffer;

		int m_Width, m_Height, m_Bits;

	public:

		Texture(const std::string& path, bool isCubeMap);
		~Texture();
		

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline int GetBits() const { return m_Bits; }
};

