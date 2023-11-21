#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
    {

    }
}

static void GLCheckError()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]" << error << std::endl;
    }
}

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "MultiMax", NULL, NULL);
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

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (4 * 2) * sizeof(float), vertexList, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    unsigned int ibo; //index buffer object
    (glGenBuffers(1, &ibo)); //generate index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //Give buffer data

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Colour");

    _ASSERT(location != -1);

    glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(location, 0.2f, 0.6f, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}