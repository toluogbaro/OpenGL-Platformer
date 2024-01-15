//Credit to https://www.youtube.com/@TheCherno Youtube channel for all of the rendering classes and shader files
//Followed Tutorials from the OpenGL Playlist: https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&feature=shared
// Shader File Tutorial: https://youtu.be/2pv0Fbo-7ms?feature=shared
// 
//Credit to https://learnopengl.com/ and Joey De Vries for the Matrix Transformation maths using GLM
// Followed Tutorial: https://learnopengl.com/Getting-started/Transformations
// Followed Tutorial: https://learnopengl.com/Getting-started/Coordinate-Systems
// 
//Credit to https://learnopengl.com/ and Joey De Vries for the camera class
//Followed Tutorial: https://learnopengl.com/Getting-started/Camera
//
//Collision System Built Upon AABB Tutorial From https://learnopengl.com/ 
//Followed Tutorial: https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
//
//Credit to https://youtube.com/@danielblagy for the Entity Component System Tutorial
//Followed Tutorial: https://youtu.be/8LbVpkEqKuY?feature=shared

//All classes appropriately credited within them


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


float deltaTime; //time between the current frame and last frame

float cameraOffsetY = 4.6f; //Y offset behind player in player camera mode
float cameraOffsetZ = -8.5f; //Z offset behind player in player camera mode
float cameraAngle;

int windowHeight = 1920;
int windowWidth = 1080;


std::unique_ptr<Camera> cameraObj(new Camera(50.0f, 45.0f)); //initialise camera with FOV
static bool isGameStarted;
static bool isGrounded;



World gameWorld;

float playerRotX;
float playerRotY;
float walkSpeed = 10.0f; //player walkspeed stored as global to change when player is jumping
float maxJumpHeight;

enum PlayerState { NEUTRAL, JUMPING, RUNNING };
PlayerState currentPlayerState;

//convert enum to string for use in imgui debugging
const char* to_string(PlayerState p)
{
    switch (p)
    {
        case  PlayerState::NEUTRAL:
            return "Neutral";
            break;

        case  PlayerState::JUMPING:
            return "Jumping";
            break;

        case  PlayerState::RUNNING:
            return "Running";
            break;

        default:
            return "None";
            break;

    }
}

void mouse_callback(GLFWwindow* currentWindow, double xPos, double yPos) 
{
    cameraObj->ProcessMouseInput(currentWindow, xPos, yPos, windowHeight, windowWidth);

    float lastMouseX = 0;
    float lastMouseY = 0;

   
    float xOffset = xPos - lastMouseX;
    float yOffset = lastMouseY - yPos;


    //current mouse input vs the last mouse input

    lastMouseX = xPos;
    lastMouseY = yPos;

    //setting the last mouse positions to the current ones

    xOffset *= 0.1f;
    yOffset *= 0.1f;

    //multiplying offset by a sensitivity value, for ex. 0.5f, dampens it

    playerRotX += xOffset;
    playerRotY += yOffset;

    // adding the current mouse position - the last mouse position gives you where the mouse is currently meant
    // to be in terms of these euler angles

    yOffset = glm::clamp(yOffset, -89.0f, 89.0f);

}

//scroll callback calls this function when the user scrolls their mousewheel
void scroll_callback(GLFWwindow* currentWindow, double xOffset, double yOffset)
{
    cameraObj->ProcessScroll(yOffset);
    //FOV control
}

//key callback calls this function when the user presses a key
void key_callback(GLFWwindow* currentWindow, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
       

    }

  
}

//Moving the mouse around with WASD in free roam mode
void ProcessInput(GLFWwindow* currentWindow)
{
    if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE))
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //bring back cursor with escape button
    }
    else
    {
        glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //cursor is usually hidden

    }

    cameraObj->ProcessKeyInput(currentWindow, deltaTime);

    glfwSetCursorPosCallback(currentWindow, mouse_callback);
    glfwSetScrollCallback(currentWindow, scroll_callback);
    

}

//Initialise UI
void ImGuiInitialize(GLFWwindow* window)
{

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, GL_TRUE);
    ImGui::StyleColorsDark();

}

//Run UI
void ImGuiRun(ImGuiIO& io)
{
    ImGui_ImplGlfwGL3_NewFrame();

    {

        ImVec2 standardButtonSize{ 200.0f, .0f };

        ImGui::Begin("Maximum Processing Power");

        ImGui::Text("Player State = %s",  to_string(currentPlayerState));

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

//Function takes in two objects position and size and uses AABB method to detect collisiion
bool CollisionDetection(glm::vec3 positionOne, glm::vec3 sizeOne, glm::vec3 positionTwo, glm::vec3 sizeTwo)
{

    bool collisionX = positionOne.x - (sizeOne.x / 2.0f) <= positionTwo.x + (sizeTwo.x / 2.0f) &&
        positionOne.x + (sizeOne.x / 2.0f) >= positionTwo.x - (sizeTwo.x / 2.0f);
   
    bool collisionY = positionOne.y - (sizeOne.y / 2.0f) <= positionTwo.y + (sizeTwo.y / 2.0f) && 
        positionOne.y + (sizeOne.y / 2.0f) >= positionTwo.y - (sizeTwo.y / 2.0f);

    bool collisionZ = positionOne.z - (sizeOne.z / 2.0f) <= positionTwo.z + (sizeTwo.z / 2.0f) &&
        positionOne.z + (sizeOne.z / 2.0f) >= positionTwo.z - (sizeTwo.z / 2.0f);

    //each collision is taking the position of both objects + or - half their size to get the boundary areas
    //comparison of the axis alignment of both objects boundary areas gives a collision on that axis
    

    return collisionX && collisionY && collisionZ;
    //just like in traditional AABB, collision is detected when all three axis are colliding
}


int main(void)
{

    #pragma region Initialisation

    GLFWwindow* window;

    currentPlayerState = NEUTRAL;

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
        //if there is no glfw window, terminate glfw and close visual studio window
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "not glew ok" << std::endl;

    std::cout << "Current OpenGL Version is: " << GLFW_VERSION_MAJOR << "\n";

#pragma endregion

    {
        #pragma region Construction

        std::unique_ptr<VertexArray> va(new VertexArray);

        std::unique_ptr<VertexBufferLayout> layout(new VertexBufferLayout);
        std::unique_ptr<MeshTemplate> meshTemplate(new MeshTemplate);

        layout->Push<float>(3);
        layout->Push<float>(2);

        std::unique_ptr<VertexBuffer> vb(new VertexBuffer(sizeof(meshTemplate->m_Cube_Vertices), meshTemplate->m_Cube_Vertices));

        va->AddBuffer(*vb, *layout);

        std::unique_ptr<IndexBuffer> ib(new IndexBuffer(meshTemplate->m_Cube_Indices, 36));

        std::unique_ptr<Shader> shader(new Shader("res/shaders/Basic.shader"));

        std::unique_ptr<Shader> lightShader(new Shader("res/shaders/light.shader"));

        std::unique_ptr<Renderer> renderer(new Renderer);

        renderer->EnableAll();
        //enable depth buffer

        ImGuiInitialize(window);

        ImGuiIO& io = ImGui::GetIO();

        glm::vec3 originalPos = glm::vec3(0.0f);
        glm::vec3 playerTransform = originalPos;
        
        glm::vec3 platformPos = glm::vec3(0.0f);
        platformPos.y = playerTransform.y - 15.0f;

        glm::vec3 playerToPlatform = platformPos - playerTransform;
        glm::vec3 dirPlayerToPlatform = glm::normalize(playerToPlatform);
        glm::vec3 normalVec = playerTransform + dirPlayerToPlatform;
        //normalised vector between player and platform

        float oldTimeSinceStart = 0.0f;
        float timeSinceStart = 0.0f;
        float airTime = 0.1f;
        
        float gravityVelo = 0.0f;
        float gravityAccel = 0.0f;
        float jumpDistance = 0.0f;

#pragma endregion

        while (!glfwWindowShouldClose(window))
        {
            
            timeSinceStart = static_cast<float>(glfwGetTime());
            deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;

            renderer->Clear();

            ProcessInput(window);
            glfwSetKeyCallback(window, key_callback);

            #pragma region Movement and Gravity

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded)
            {
                currentPlayerState = JUMPING;
                //keep player state as jump for each frame cycle

            }

            switch (currentPlayerState)
            {
            case NEUTRAL:

                break;

            case JUMPING:
                if (playerTransform.y < maxJumpHeight)
                {
                    playerTransform.y += 6.0f * deltaTime;
                    //addition each frame
                }
                break;

            default:
                break;
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                //if (isColliding) playerTransform.z += (normalVec.z * 5.0f) * deltaTime;

                playerTransform.z += walkSpeed * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                //if (isColliding) playerTransform.z -= (normalVec.z * 5.0f) * deltaTime;
                playerTransform.z -= walkSpeed * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {

                //if (isColliding) playerTransform.x -= (normalVec.x * 5.0f) * deltaTime;
                playerTransform.x -= walkSpeed * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                //if (isColliding) playerTransform.x += (normalVec.x * 5.0f) * deltaTime;

                playerTransform.x += walkSpeed * deltaTime;

            }


            if (!isGrounded)
            {
                airTime -= deltaTime;
                //airtime goes down while in the air

                walkSpeed = 2.0f;
                //walk speed slowed down to simulate lack of control while jumping
                
                gravityVelo += -9.81f * deltaTime;
                gravityAccel += gravityVelo * deltaTime;
                //gravity increases over time

                if (airTime <= 0)
                {
                    playerTransform.y += gravityAccel * deltaTime;
                    //when airtime reaches zero, gravity acts upon the player
                    //this is to create a parabola effect when the player jumps
                }
            }
            else
            {
                airTime = 0.1f;
                //reset amount of time player is allowed to be in the air before gravity kicks in
                walkSpeed = 10.0f;
                //reset walk speed
                maxJumpHeight = playerTransform.y + 5.0f;
                //calculate max jump height only when player is grounded to avoid infinite jump
                currentPlayerState = NEUTRAL;
                //reset player state
                gravityVelo = 0.0f;
                gravityAccel = 0.0f;
                //reset gravity values for acceleration after object has been in the air for a set amount of time
               
            }

#pragma endregion

            #pragma region Matrices

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, playerTransform);
            //model matrix updates based on current player transform

            if (cameraObj->GetCameraMode() == Camera_Mode::PLAYER)
            {
                cameraObj->PlayerCamera(playerTransform);
                cameraObj->SetCameraOffset(0.0f, cameraOffsetY, cameraOffsetZ);
            }

            cameraObj->ProcessViewMatrix();

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(cameraObj->GetFOV()), 1920.0f / 1080.0f, 0.1f, 1000.0f);
            //projection based off of camera FOV

            shader->SetUniform4f("u_Colour", 1.0f, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", model);
            shader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
            shader->SetUnifromMat4("projection", projection);
            shader->Bind();

            //updating the shader with the defined MVP values

            renderer->DrawObject(*va, *ib); //draw call using Vertex Array and Index Buffer

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

#pragma endregion

            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, glm::vec3(0.0f));

            playerToPlatform = platformPos - playerTransform;
            dirPlayerToPlatform = glm::normalize(playerToPlatform); 
            normalVec = playerTransform + dirPlayerToPlatform; 

            //calculating the direction between player and platform for collision on the xz plane
            //to add an equal force that sends the player back
            //simulating inelastic collision that keeps the player static

            if (CollisionDetection(playerTransform, glm::vec3(1.0f), platformPos, platformSize))
            {
                
                lightModel = glm::translate(lightModel, normalVec);
                lightModel = glm::scale(lightModel, glm::vec3(1.0f));
                shader->SetUniform4f("u_Colour", 1.0f, 1.0f, 1.0f, 1.0f);
                shader->SetUnifromMat4("model", lightModel);
                shader->SetUnifromMat4("view", cameraObj->ProcessViewMatrix());
                shader->SetUnifromMat4("projection", projection);
                shader->Bind();

                renderer->DrawObject(*va, *ib);

                //rendering a bright object for debugging purposes

                isGrounded = true;

               // std::cout << "Collided" << std::endl;
            }
            else
                isGrounded = false;


            if (isGameStarted)
            {
                gameWorld.Update(deltaTime);
            }

            /* run UI Window */
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