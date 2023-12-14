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
#include "MeshTemplate.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

float deltaTime;
float cameraOffsetX;
float cameraOffsetY;
float cameraOffsetZ;

int windowHeight = 1920;
int windowWidth = 1080;

std::unique_ptr<Camera> cameraObj(new Camera(10.0f, 45.0f));


void mouse_callback(GLFWwindow* currentWindow, double xPos, double yPos) 
{

   if(cameraObj->GetCameraMode() == Camera_Mode::FREE_ROAM) cameraObj->ProcessMouseInput(currentWindow, xPos, yPos, windowHeight, windowWidth);
   //mouse callback calls this function when mouse input is detected
   //Movement and rotation control

}

void scroll_callback(GLFWwindow* currentWindow, double xOffset, double yOffset)
{
    cameraObj->ProcessScroll(yOffset);
    //scroll callback calls this function when the user scrolls their mousewheel
    //FOV control
}

void key_callback(GLFWwindow* currentWindow, int key, int scancode, int action, int mods)
{
    std::cout << "Key: " << glfwGetKeyName(key, 0) << " is pressed." << std::endl;
}


void ProcessInput(GLFWwindow* currentWindow)
{
    if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE))
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }

    if (cameraObj->GetCameraMode() == Camera_Mode::FREE_ROAM)
    {
        cameraObj->ProcessKeyInput(currentWindow, deltaTime);

        glfwSetCursorPosCallback(currentWindow, mouse_callback);
        glfwSetScrollCallback(currentWindow, scroll_callback);
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

        ImVec2 standardButtonSize{ 200.0f, .0f };

        ImGui::Begin("Hello, world!");

        ImGui::SliderFloat("Offset X", &cameraOffsetX, -5.0f, 5.0f);
        ImGui::SliderFloat("Offset Y", &cameraOffsetY, -5.0f, 5.0f);
        ImGui::SliderFloat("Offset Z", &cameraOffsetZ, -5.0f, 5.0f);

        if (ImGui::Button("Free Roam", standardButtonSize))
        {
            cameraObj->ChangeCameraMode(Camera_Mode::FREE_ROAM);
        }

        if (ImGui::Button("Player Mode", standardButtonSize))
        {
            cameraObj->ChangeCameraMode(Camera_Mode::PLAYER);
        }

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
    window = glfwCreateWindow(windowHeight, windowWidth, "Doodle Jump Remake", NULL, NULL);

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

        std::unique_ptr<VertexArray> va(new VertexArray);


        std::unique_ptr<VertexBufferLayout> layout(new VertexBufferLayout);
        std::unique_ptr<VertexBufferLayout> skyboxLayout(new VertexBufferLayout);
        std::unique_ptr<MeshTemplate> meshTemplate(new MeshTemplate);

        layout->Push<float>(3);
        layout->Push<float>(2);

        std::unique_ptr<VertexBuffer> vb(new VertexBuffer(sizeof(meshTemplate->m_Cube_Vertices), meshTemplate->m_Cube_Vertices));
        std::unique_ptr<VertexBuffer> skyboxVB(new VertexBuffer(sizeof(meshTemplate->m_Cube_Vertices), meshTemplate->m_Cube_Vertices));

        skyboxLayout->Push<float>(3);
        skyboxLayout->Push<float>(2);

        va->AddBuffer(*vb, *layout);
        va->AddBuffer(*skyboxVB, *skyboxLayout);


        std::unique_ptr<IndexBuffer> ib(new IndexBuffer(meshTemplate->m_Cube_Indices, 36));
        std::unique_ptr<IndexBuffer> skyboxIB(new IndexBuffer(meshTemplate->m_Cube_Indices, 36));

        std::unique_ptr<Shader> shader(new Shader("res/shaders/Basic.shader"));
        std::unique_ptr<Shader> skyboxShader(new Shader("res/shaders/Skybox.shader"));

        std::unique_ptr<Texture> skyboxTexture(new Texture("res/textures/Sunset.png", true));

        skyboxTexture->Bind();

        std::unique_ptr<Renderer> renderer(new Renderer);

        renderer->EnableAll();

        ImGuiInitialize(window);

        ImGuiIO& io = ImGui::GetIO();

        float oldTimeSinceStart = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            
            float timeSinceStart = static_cast<float>(glfwGetTime());
            deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;
            
            renderer->Clear();

            ProcessInput(window);
            //glfwSetKeyCallback(window, key_callback);

            glm::vec3 playerTransform = glm::vec3(0.0f, 0.0f, 0.0f);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, playerTransform);

            if (cameraObj->GetCameraMode() == Camera_Mode::PLAYER)
            {
                cameraObj->PlayerCamera(playerTransform);
                cameraObj->SetCameraOffset(cameraOffsetX, cameraOffsetY, cameraOffsetZ);
            }

            cameraObj->ProcessViewMatrix();

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(cameraObj->GetFOV()), 1920.0f / 1080.0f, 0.1f, 100.0f);


            shader->SetUniform4f("u_Colour", 1.0f, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", model);
            shader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
            shader->SetUnifromMat4("projection", projection);
            shader->Bind();

            renderer->DrawObject(*va, *ib);

            glm::mat4 platformModel = glm::mat4(1.0f);
            glm::vec3 platformPos = glm::vec3(0.0f);
            platformPos.y = playerTransform.y - 6.0f;
            platformModel = glm::translate(platformModel, platformPos);
            platformModel = glm::scale(platformModel, glm::vec3(10.0f));

            shader->SetUniform4f("u_Colour", 0.0f, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", platformModel);
            shader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
            shader->SetUnifromMat4("projection", projection);
            shader->Bind();

            
            renderer->DrawObject(*va, *ib);
            

            glDepthFunc(GL_LEQUAL);

            //skyboxShader->SetUniform4f("u_Texture", 1.0f, 1.0f, 1.0f, 1.0f);
            //skyboxShader->Bind();

            //renderer->DrawObject(*va, *ib);

            glDepthFunc(GL_LESS);


            //run UI Window
            ImGuiRun(io);
           
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

        }
        shader->Unbind();
        skyboxShader->Unbind();
        skyboxTexture->Unbind();

    }

   
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    return 0;
}