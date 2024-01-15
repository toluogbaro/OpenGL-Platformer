//Credit to https://www.youtube.com/@TheCherno
//Followed Tutorial: https://youtu.be/2pv0Fbo-7ms?feature=shared

#include "Shader.h"

Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath)
{
    ShaderProgramSource source = ParseShader(filepath);

    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

//returns vertex or fragment string line by parsing file path
ShaderProgramSource Shader::ParseShader(const std::string filePath)
{
    std::ifstream stream(filePath);

    if (!stream) std::cout << "The filepath " << " " << filePath << " " << "does not exist" << std::endl;

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        //if the shader line is not null

        if (line.find("#shader") != std::string::npos)
        {
            //if the vertext or fragment lines are not null
            //then we add everything after them into a string file path to be read individually

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    //enum for empty shader object of type defined by integer in a list
    unsigned int id = glCreateShader(type);
    //C_String is an array of characters which is more efficient
    //Our source string is a file link to our shader
    const char* src = source.c_str();
    //Takes in shader enum, count of elements in the string array
    //Sets source code of empty shader and compiles
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error catch to check if the compilation of the current shader succeeded
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader" << "" << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

//takes in both vertex and fragment shaders compiled with CompileShader
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //Create empty program object to attach shader to
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attaches shader to program object
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    
    glLinkProgram(program);

    //Checks to see if program can execute
    glValidateProgram(program);

    //Freeing up memory
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}


int Shader::GetUniformLocation(const std::string& name)
{
    
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    //if uniform location is already in hashmap, return

    int location = glGetUniformLocation(m_RendererID, name.c_str()); //gets location using uniform name

    if (location == -1)
        std::cout << "Uniform: " << name << " doesn't exist" << std::endl; //error check

    m_UniformLocationCache[name] = location; //adds location to hashmap

    return location;

}

//a uniform is a pocket of data that replaces manually adding them. For instance
//four floats could be r g b a for a colour in a fragment shader

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUnifromMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}



