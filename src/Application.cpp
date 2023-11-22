#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"



struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

//returns vertex or fragment string line by parsing file path
static ShaderProgramSource ParseShader(const std::string filePath)
{
    std::ifstream stream(filePath);

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

//takes in vertex or fragment with file source
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    //enum for empty shader object of type defined by integer in a list
    unsigned int id = glCreateShader(type);
    //C_String is an array of characters which is more efficient
    //Our source string is a file link to our shader
    const char* src = source.c_str();
    //Takes in shader enum, count of elements in the string array and string array
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
        std::cout << "Failed to compile shader" << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader" ) << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

//takes in both vertex and fragment shaders compiled with CompileShader
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //Create empty program object to attach shader to
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attaches shader to program object
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //links the program object after attaching shaders to it
    glLinkProgram(program);

    //Checks to see if program can execute
    glValidateProgram(program);

    //Freeing up memory
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "MultiMax", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "not glew ok" << std::endl;

    std::cout << "Current OpenGL Version is: " << GLFW_VERSION_MAJOR << "\n";
    {
        float vertexList[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,

        };

        unsigned int indices[] =
        {
            0, 1, 2,

            2, 3, 0
        };

        VertexArray va;

        VertexBuffer vb(vertexList, (2 * 4) * sizeof(float));

        VertexBufferLayout layout;

        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        //unsigned int ibo; //index buffer object
        //glGenBuffers(1, &ibo); //generate index buffer
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //Give buffer data

        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

        int location = glGetUniformLocation(shader, "u_Colour");

        _ASSERT(location != -1);

        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //vertex arrays are a way to bind buffers with a certain specification
        //the process is binding vertex buffer -> using an attrib array to specify the attributes of the vertex -> bind index buffer
        //Vertex Arrays can be used to differentiate different attributes in the same binding -> modularity?

        /* Loop until the user closes the window */

        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader);
            glUniform4f(location, r, 0.6f, 0.8f, 1.0f);

            va.Bind(); //bind vertex array
            ib.Bind(); //bind index buffer

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            if (r >= 1.0f)
                increment = -0.05f;
            else if (r <= 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        glDeleteProgram(shader);

    }

   

    glfwTerminate();
    return 0;
}