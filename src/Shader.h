#pragma once

#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{


	private:

		std::string m_Filepath;
		unsigned int m_RendererID;

		

	private:

		int GetUniformLocation(const std::string& name);

		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

		unsigned int CompileShader(unsigned int type, const std::string& source);

		ShaderProgramSource ParseShader(const std::string filePath);


	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

		

		


};