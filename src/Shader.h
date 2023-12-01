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

		//Gets uniform location from shader using its name
		int GetUniformLocation(const std::string& name);
		
		//creates shader using parsed file reference
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

		//compiles shader to add to program
		unsigned int CompileShader(unsigned int type, const std::string& source);

		//takes file path of shader to parse
		ShaderProgramSource ParseShader(const std::string filePath);


	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		//set a uniform for 1 float
		void SetUniform1f(const std::string& name, float value);

		//set a uniform for 1 int
		void SetUniform1i(const std::string& name, int value);

		//set a uniform for 4 floats
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

		//set a uniform for a matrix of 4
		void SetUnifromMat4(const std::string& name, const glm::mat4& matrix);

		

		


};