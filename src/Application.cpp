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
#include "Shader.h"




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


        Shader shader("res/shaders/Basic.shader");

        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();


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

            shader.Bind();
            shader.SetUniform4f("u_Colour", r, 0.6f, 0.8f, 1.0f);

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
        shader.Unbind();

    }

   

    glfwTerminate();
    return 0;
}