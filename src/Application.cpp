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

#include "World.h"
#include "Entity.h"



float deltaTime;
float cameraOffsetX;
float cameraOffsetY;
float cameraOffsetZ;
float cameraAngle;

int windowHeight = 1920;
int windowWidth = 1080;

std::unique_ptr<Camera> cameraObj(new Camera(50.0f, 45.0f));
static bool isGameStarted;

World gameWorld;


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

        if (ImGui::Button("Start Game", standardButtonSize))
        {
            isGameStarted = true;
        }


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }


    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}

bool CollisionDetection(glm::vec3 positionOne, glm::vec3 sizeOne, glm::vec3 positionTwo, glm::vec3 sizeTwo, float collisionOffset)
{
    //bool collisionX = positionOne.x

    bool collisionX = positionOne.x + (sizeOne.x / 2.0f) + collisionOffset >= positionTwo.x &&
        positionTwo.x + (sizeTwo.x / 2.0f) + collisionOffset >= positionOne.x;

    bool collisionY = positionOne.y + (sizeOne.y / 2.0f) + collisionOffset >= positionTwo.y &&
        positionTwo.y + (sizeTwo.y / 2.0f) + collisionOffset >= positionOne.y;

    bool collisionZ = positionOne.z + (sizeOne.z / 2.0f) + collisionOffset >= positionTwo.z &&
        positionTwo.z + (sizeTwo.z / 2.0f) + collisionOffset >= positionOne.z;

    return collisionX && collisionY && collisionZ;
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
        //std::unique_ptr<Shader> skyboxShader(new Shader("res/shaders/Skybox.shader"));
        std::unique_ptr<Shader> lightShader(new Shader("res/shaders/light.shader"));

        std::unique_ptr<Texture> skyboxTexture(new Texture("res/textures/Sunset.png", true));

        skyboxTexture->Bind();

        std::unique_ptr<Renderer> renderer(new Renderer);

        renderer->EnableAll();

        ImGuiInitialize(window);

        ImGuiIO& io = ImGui::GetIO();

        float oldTimeSinceStart = 0.0f;

        
        Entity player = gameWorld.create_entity("player");
        player.add_component<Transform>();
        player.add_component<Gravity>();

        Entity projectile = gameWorld.create_entity("projectile");
        projectile.add_component<Transform>();
        projectile.add_component<Kinematic>();
 
        glm::vec3 originalPos = glm::vec3(0.0f);
        glm::vec3 playerTransform = originalPos;
        

        glm::vec3 platformPos = glm::vec3(0.0f);
        platformPos.y = playerTransform.y - 7.5f;


        while (!glfwWindowShouldClose(window))
        {
            
            float timeSinceStart = static_cast<float>(glfwGetTime());
            deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;
            
            renderer->Clear();

            ProcessInput(window);
            //glfwSetKeyCallback(window, key_callback);

           /* float x = player.get_component<Transform>().posX;
            float y = player.get_component<Transform>().posY;
            float z = player.get_component<Transform>().posZ;

            playerTransform = glm::vec3(x, y, z);*/

     /*       projectile.get_component<Kinematic>().accelerationX = 0.0f;
            projectile.get_component<Kinematic>().accelerationY = -9.81f;
            projectile.get_component<Kinematic>().angle = 45.0f;
            projectile.get_component<Kinematic>().power = 200.0f;
            projectile.get_component<Kinematic>().velocityX = playerTransform.x;
            projectile.get_component<Kinematic>().velocityY = playerTransform.y;

            x = projectile.get_component<Transform>().posX;
            y = projectile.get_component<Transform>().posY;

            playerTransform = glm::vec3(x, y, z);*/

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                playerTransform.z += 20.0f * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                playerTransform.z -= 20.0f * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {
                playerTransform.x -= 20.0f * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                playerTransform.x += 20.0f * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                playerTransform.y -= 2.0f * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
            {
                playerTransform.y += 2.0f * deltaTime;

            }

            

            
                   
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, playerTransform);

            if (cameraObj->GetCameraMode() == Camera_Mode::PLAYER)
            {
                cameraObj->PlayerCamera(playerTransform);
                cameraObj->SetCameraOffset(cameraOffsetX, cameraOffsetY, cameraOffsetZ);
            }

            cameraObj->ProcessViewMatrix();

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(cameraObj->GetFOV()), 1920.0f / 1080.0f, 0.1f, 1000.0f);

            shader->SetUniform4f("u_Colour", 1.0f, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", model);
            shader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
            shader->SetUnifromMat4("projection", projection);
            shader->Bind();

            renderer->DrawObject(*va, *ib);

            glm::mat4 platformModel = glm::mat4(1.0f);
            platformModel = glm::translate(platformModel, platformPos);
            glm::vec3 platformSize = glm::vec3(10.0f);
            platformModel = glm::scale(platformModel, platformSize);

            shader->SetUniform4f("u_Colour", 0.0f, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", platformModel);
            shader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
            shader->SetUnifromMat4("projection", projection);
            shader->Bind();

            renderer->DrawObject(*va, *ib);

            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, originalPos + 1.5f);
            lightModel = glm::scale(lightModel, glm::vec3(2.0f));

           /* lightShader->SetUniform4f("u_LightColour", 1.0f, 1.0f, 1.0f, 1.0f);
            lightShader->SetUnifromMat4("model", lightModel);
            lightShader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
            lightShader->SetUnifromMat4("projection", projection);
            lightShader->Bind();*/
            
            //renderer->DrawObject(*va, *ib);

            if (isGameStarted)
            {
                gameWorld.Update(deltaTime);
            } 

            if (CollisionDetection(playerTransform, glm::vec3(1.0f), platformPos, platformSize, 0.5f))
            {
                std::cout << "collision" << std::endl;
            }

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
       //skyboxShader->Unbind();
        skyboxTexture->Unbind();

    }

   
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    return 0;
}