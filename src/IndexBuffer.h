#pragma once

//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/MXNMC1YAxVQ?feature=shared

class IndexBuffer
{
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;

	public:
	
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; };

};
