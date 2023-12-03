#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


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
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f

        };

        unsigned int indices[] =
        {
            0, 1, 2,

            2, 3, 0
        };



        std::unique_ptr<Renderer> renderer(new Renderer);

        VertexArray va;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);

        VertexBuffer vb(sizeof(vertexList),  vertexList);

        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");

        Texture texture("res/textures/ToluChill.png");
        texture.Bind();

        shader.SetUniform1i("u_Texture", 0);

        shader.Bind();

        shader.SetUniform4f("u_Colour", 0.8f, 0.6f, 0.8f, 1.0f);

        shader.Bind();

        //shader.SetUnifromMat4("u_MVP", proj);

        shader.Bind();

        //va.Unbind();
        //vb.Unbind();
        //ib.Unbind();
        //shader.Unbind();

        

        //vertex arrays are a way to bind buffers with a certain specification
        //the process is binding vertex buffer -> using an attrib array to specify the attributes of the vertex -> bind index buffer
        //Vertex Arrays can be used to differentiate different attributes in the same binding -> modularity?

        /* Loop until the user closes the window */

        float r = 0.0f;
        float increment = 0.05f;

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, GL_TRUE);
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();

        bool shouldBlink = false;

        
        float oldTimeSinceStart = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            
            renderer->Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

           
            shader.SetUniform4f("u_Colour", r, 0.6f, 0.8f, 1.0f);
            shader.SetUnifromMat4("model", model);
            shader.SetUnifromMat4("view", view);
            shader.SetUnifromMat4("projection", projection);
            shader.Bind();


            renderer->DrawObject(va, ib, shader);

            float timeSinceStart = static_cast<float>(glfwGetTime());
            float deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;

            static float f = 0.0f;

            if (shouldBlink)

            {
               

                if (r >= 1.0f)
                    increment = -0.05f;
                else if (r <= 0.0f)
                    increment = 0.05f;

                r += increment * f * deltaTime;
            }


            {
                
                static int counter = 0;

                ImVec2 standardButtonSize{ 200.0f, .0f };

                ImGui::Begin("Hello, world!");                         

                ImGui::SliderFloat("float", &f, 0.0f, 20.0f); 

                if (ImGui::Button("Activate Blink", standardButtonSize))
                    shouldBlink = true;

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        shader.Unbind();

    }

   
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    return 0;
}