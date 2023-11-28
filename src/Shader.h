#pragma once

#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "glm/glm.hpp"

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
		std::unordered_map<std::string, int> m_UniformLocationCache;
		

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

		void SetUniform1f(const std::string& name, float value);
		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUnifromMat4(const std::string& name, const glm::mat4& matrix);

		

		


};