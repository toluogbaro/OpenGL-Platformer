#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




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
    window = glfwCreateWindow(1920, 1080, "Doodle Jump Remake", NULL, NULL);
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
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,

        };

        unsigned int indices[] =
        {
            0, 1, 2,

            2, 3, 0
        };

        Renderer renderer;

        VertexArray va;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        VertexBuffer vb(vertexList, (4 * 4) * sizeof(float));

        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");

        Texture texture("res/textures/ToluChill.png");
        texture.Bind();

        shader.SetUniform1i("u_Texture", 0);

        shader.Bind();

        shader.SetUniform4f("u_Colour", 0.8f, 0.6f, 0.8f, 1.0f);

        shader.Bind();

        shader.SetUnifromMat4("u_MVP", proj);

        shader.Bind();

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        

        //vertex arrays are a way to bind buffers with a certain specification
        //the process is binding vertex buffer -> using an attrib array to specify the attributes of the vertex -> bind index buffer
        //Vertex Arrays can be used to differentiate different attributes in the same binding -> modularity?

        /* Loop until the user closes the window */

        float r = 0.0f;
        float increment = 0.05f;

        while (!glfwWindowShouldClose(window))
        {
            
            renderer.Clear();

            //need to take in material here, not shader
            shader.SetUniform4f("u_Colour", r, 0.6f, 0.8f, 1.0f);
            shader.Bind();
            
            
            renderer.DrawObject(va, ib, shader);

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
        shader.Unbind();

    }

   

    glfwTerminate();
    return 0;
}