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
#include "matrix/Camera.h"
#include "vertex_templates/CubeVertexList.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

float deltaTime;
//float cameraSpeed = 10.0f;
//
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//
//float lastMouseX = 0.0f;
//float lastMouseY = 0.0f;
//float yaw = -90.0f;
//float pitch = -89.0f;
//float fov = 45.0f;
//float mouseSensitivity = 0.1f;
//
//bool firstMouse = true;

std::unique_ptr<Camera> freeRoamCamera(new Camera(10.0f, 45.0f));


void mouse_callback(GLFWwindow* currentWindow, double xPos, double yPos) 
{

    freeRoamCamera->ProcessMovement(xPos, yPos);

}

void scroll_callback(GLFWwindow* currentWindow, double xOffset, double yOffset)
{
    freeRoamCamera->ProcessScroll(yOffset);
}

void ProcessInput(GLFWwindow* currentWindow)
{
    freeRoamCamera->ProcessInput(currentWindow, deltaTime);

    glfwSetCursorPosCallback(currentWindow, mouse_callback);
    glfwSetScrollCallback(currentWindow, scroll_callback);

    if (!glfwGetKey(currentWindow, GLFW_KEY_ESCAPE))
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }
    else
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }

}

void ImGuiInitialize(GLFWwindow* window)
{

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, GL_TRUE);
    ImGui::StyleColorsDark();



}

void ImGuiRun(ImGuiIO& io)
{
    ImGui_ImplGlfwGL3_NewFrame();

    {

        static int counter = 0;
        static float blinkSpeed = 20.0f;

        ImVec2 standardButtonSize{ 200.0f, .0f };

        ImGui::Begin("Hello, world!");

        ImGui::SliderFloat("Blink Speed", &blinkSpeed, 20.0f, 100.0f);

        //ImGui::SliderFloat("Camera Speed", freeRoamCamera->GetCameraSpeed(), 5.0f, 50.0f);

        //ImGui::SliderFloat("Mouse Speed", &mouseSensitivity, 0.1f, 1.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }


    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

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
       
        VertexArray va;

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);

        std::unique_ptr<CubeVertexList> cubeVertices(new CubeVertexList);

        VertexBuffer vb(sizeof(cubeVertices->vertices), cubeVertices->vertices);

        va.AddBuffer(vb, layout);

        IndexBuffer ib(cubeVertices->indices, 36);

        //vertex arrays are a way to bind buffers with a certain specification
        //the process is binding vertex buffer -> using an attrib array to specify the attributes of the vertex -> bind index buffer
        //Vertex Arrays can be used to differentiate different attributes in the same binding -> modularity?


        float r = 0.0f;
        float increment = 0.05f;

        float oldTimeSinceStart = 0.0f;

        std::unique_ptr<Shader> shader(new Shader("res/shaders/Basic.shader"));

        std::unique_ptr<Texture> texture(new Texture("res/textures/ToluChill.png"));

        texture->Bind();

        std::unique_ptr<Renderer> renderer(new Renderer);

        renderer->EnableAll();

        ImGuiInitialize(window);

        ImGuiIO& io = ImGui::GetIO();

        while (!glfwWindowShouldClose(window))
        {
            
            float timeSinceStart = static_cast<float>(glfwGetTime());
            deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;
            
            renderer->Clear();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
          
           /* glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);*/

            freeRoamCamera->ProcessViewMatrix();

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(freeRoamCamera->GetFOV()), 1920.0f / 1080.0f, 0.1f, 100.0f);

            ProcessInput(window);

            shader->SetUniform4f("u_Colour", r, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", model);
            shader->SetUnifromMat4("view", freeRoamCamera->ProcessViewMatrix());
            shader->SetUnifromMat4("projection", projection);
            shader->Bind();

            renderer->DrawObject(va, ib);

            //run UI Window
            ImGuiIO& io = ImGui::GetIO();
            ImGuiRun(io);
           
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            

            
        }
        shader->Unbind();

    }

   
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    return 0;
}