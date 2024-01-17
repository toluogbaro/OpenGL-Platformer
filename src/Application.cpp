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
//s

//All classes appropriately credited within them

#pragma region Includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <random>


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
#pragma endregion

#pragma region Global Variables
float deltaTime; //time between the current frame and last frame

float cameraOffsetY = 4.6f; //Y offset behind player in player camera mode
float cameraOffsetZ = -8.5f; //Z offset behind player in player camera mode
float cameraAngle;

int windowHeight = 1920;
int windowWidth = 1080;

std::unique_ptr<Camera> cameraObj(new Camera(50.0f, 45.0f)); //initialise camera with FOV

static bool isPlayerDead;
static bool isGrounded;
static bool isShooting;
static bool isAIActive;
static bool isWandering;
static bool isLineOfSight;


float walkSpeed = 10.0f; //player walkspeed stored as global to change when player is jumping
float maxJumpHeight;

enum PlayerState { NEUTRAL, JUMPING, RUNNING};
enum AIStates { VERIFICATION, WANDER, ATTACK, DEATH};
PlayerState currentPlayerState;
AIStates currentAIState;

#pragma endregion

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

    //credit to: https://mariusbancila.ro/blog/2023/08/17/how-to-convert-an-enum-to-string-in-cpp/
}

//convert enum to string for use in imgui debugging
const char* to_string(AIStates ai)
{
    switch (ai)
    {
    case  AIStates::VERIFICATION:
        return "Verification";
        break;

    case  AIStates::WANDER:
        return "Wander";
        break;

    case  AIStates::ATTACK:
        return "Attack";
        break;

    case  AIStates::DEATH:
        return "Death";
        break;

    

    default:
        return "None";
        break;

    }

    //credit to: https://mariusbancila.ro/blog/2023/08/17/how-to-convert-an-enum-to-string-in-cpp/
}

#pragma region GLFW Functions
void mouse_callback(GLFWwindow* currentWindow, double xPos, double yPos) 
{
    cameraObj->ProcessMouseInput(currentWindow, xPos, yPos, windowHeight, windowWidth);

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
#pragma endregion

#pragma region ImGui
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

        ImGui::Text("AI State = %s",  to_string(currentAIState));


        if (ImGui::Button("Activate AI", standardButtonSize))
        {
            isAIActive = true;
            currentAIState = VERIFICATION;
        }

        if (ImGui::Button("Activate LineOfSight", standardButtonSize) && isAIActive)
        {
            isLineOfSight = true;
        }

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
#pragma endregion

#pragma region Extra Functions

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

bool DistanceCheck(glm::vec3 positionOne, glm::vec3 positionTwo, float distanceThreshold)
{
    glm::vec3 distBetweenObj = positionTwo - positionOne;

    float distBeforeSqrX = (distBetweenObj.x * distBetweenObj.x);
    float distBeforeSqrY = (distBetweenObj.y * distBetweenObj.y);
    float distBeforeSqrZ = (distBetweenObj.z * distBetweenObj.z);

    float distBeforeSqr = distBeforeSqrX + distBeforeSqrY + distBeforeSqrZ;

    float distanceSquared = glm::sqrt(distBeforeSqr);

    bool detection = distanceSquared <= distanceThreshold ? true : false;

    return detection;
}

bool LineOfSight(glm::vec3 playerPos,  glm::vec3 enemyPos, float viewThreshold)
{
    
    glm::vec3 enemyToPlayerDir = glm::normalize(playerPos - enemyPos);
    //get direction to player from enemy

    glm::vec3 enemyLookDir = glm::normalize(glm::vec3(0, 0, enemyPos.z));

    float dotX = enemyToPlayerDir.x * enemyLookDir.x;
    float dotY = enemyToPlayerDir.y * enemyLookDir.y;
    float dotZ = enemyToPlayerDir.z * enemyLookDir.z;

    float dotEnemyandPlayer = dotX + dotY + dotZ;

    bool isLooking = dotEnemyandPlayer >= viewThreshold;

    return isLooking;
}
#pragma endregion


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

#pragma region Variables

        std::unique_ptr<VertexArray> va(new VertexArray);

        std::unique_ptr<VertexBufferLayout> layout(new VertexBufferLayout);
        std::unique_ptr<MeshTemplate> meshTemplate(new MeshTemplate);

        layout->Push<float>(3);
        layout->Push<float>(2);

        std::unique_ptr<VertexBuffer> vb(new VertexBuffer(sizeof(meshTemplate->m_Cube_Vertices), meshTemplate->m_Cube_Vertices));

        va->AddBuffer(*vb, *layout);

        std::unique_ptr<IndexBuffer> ib(new IndexBuffer(meshTemplate->m_Cube_Indices, 36));

        std::unique_ptr<Shader> shader(new Shader("res/shaders/Basic.shader"));

        std::unique_ptr<Shader> collisionShader(new Shader("res/shaders/light.shader"));

        std::unique_ptr<Renderer> renderer(new Renderer);

        renderer->EnableAll();
        //enable depth buffer

        ImGuiInitialize(window);

        ImGuiIO& io = ImGui::GetIO();

        glm::vec3 originalPos = glm::vec3(0.0f, 0.0f, -10.0f);
        glm::vec3 playerTransform = originalPos;
        
        glm::vec3 platformPos = glm::vec3(0.0f);
        platformPos.y = playerTransform.y - 15.0f;

        glm::vec3 playerToPlatform = platformPos - playerTransform;
        glm::vec3 dirPlayerToPlatform = glm::normalize(playerToPlatform);
        glm::vec3 normalVec = playerTransform + dirPlayerToPlatform;
        //normalised vector between player and platform

        float oldTimeSinceStart = 0.0f; //last frame
        float timeSinceStart = 0.0f; //current frame
        float airTime = 0.1f; //how long the player stays in the air before gravity activates
        
        float gravityVelo = 0.0f; //gravity velocity
        float gravityAccel = 0.0f; //gravity acceleration
        float jumpPower = 3.5f; //forms the maximum distance the player can jump

        glm::vec3 originalBulletPos = glm::vec3(0.0f);
        glm::vec3 bulletPosition = glm::vec3(0.0f);
        float currentBulletPoz = originalBulletPos.z;
        glm::vec3 bulletSize = glm::vec3(0.2f);

        glm::vec3 originalAIPosition = glm::vec3(0, -8.6f, 10.0f); //storing ai position
        glm::vec3 aiPosition = glm::vec3(0, -8.6f, 10.0f);
        glm::vec3 aiSize = glm::vec3(1.0f);
        float wanderTimer = 0.0f; //the enemy will wander everytime this timer reaches a certain number
        int randDistX = 0;
        int randDistZ = 0;
        bool PositiveOrNegative = false;

        glm::vec3 aiDetectionGizmoTransform = glm::vec3(0.0f);

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
                    playerTransform.y += jumpPower * deltaTime;
                    //addition each frame
                }
                break;
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                //if (isGrounded) playerTransform.z += (normalVec.z * 3.0f) * deltaTime;

                playerTransform.z += walkSpeed * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                //if (isGrounded) playerTransform.z += (normalVec.z * 3.0f) * deltaTime;
                playerTransform.z -= walkSpeed * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {

                //if (isGrounded) playerTransform.x += (normalVec.x * 3.0f) * deltaTime;
                playerTransform.x -= walkSpeed * deltaTime;

            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                //if (isGrounded) playerTransform.x += (normalVec.x * 3.0f) * deltaTime;

                playerTransform.x += walkSpeed * deltaTime;

            }



            if (!isGrounded)
            {
                airTime -= deltaTime;
                //airtime goes down while in the air

                walkSpeed = 3.0f;
                //walk speed slowed down to simulate lack of control while jumping
                
                 gravityVelo += -7.00f * deltaTime;
                 gravityAccel += gravityVelo * deltaTime;
                //gravity velocity is equal to gravitational constant over time
                 //gravity acceleration is equal to gravity velocity over time

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

                gravityVelo = 0.0f;
                gravityAccel = 0.0f;
                //reset gravity values for acceleration after object has been in the air for a set amount of time

                maxJumpHeight = playerTransform.y + 6.0f;
                //calculate max jump height only when player is grounded to avoid infinite jump

                walkSpeed = 10.0f;
               
            }

#pragma endregion

            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            {
                isShooting = true;
            }
            else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            {
                isShooting = false;
            }

            //shooting based on hold


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

            if (!isPlayerDead)
            {
                renderer->DrawObject(*va, *ib);
            }
                //draw call using Vertex Array and Index Buffer

            glm::mat4 platformModel = glm::mat4(1.0f);
            platformModel = glm::translate(platformModel, platformPos);
            glm::vec3 platformSize = glm::vec3(26.0f, 12.0f, 38.0f);
            platformModel = glm::scale(platformModel, platformSize);

            shader->SetUniform4f("u_Colour", 0.0f, 0.6f, 0.8f, 1.0f);
            shader->SetUnifromMat4("model", platformModel);

            renderer->DrawObject(*va, *ib);

            if (isShooting)
            {
                currentBulletPoz += 10.0f * deltaTime;

                glm::mat4 bulletModel = glm::mat4(1.0f);
                
                bulletPosition = glm::vec3(originalBulletPos.x, originalBulletPos.y, currentBulletPoz);

                bulletModel = glm::translate(bulletModel, bulletPosition);
                bulletModel = glm::scale(bulletModel, bulletSize);

                shader->SetUniform4f("u_Colour", 0.5f, 1.0f, 0.8f, 1.0f);
                shader->SetUnifromMat4("model", bulletModel);

                renderer->DrawObject(*va, *ib);

                //move the bullet on the z but keep the x and y axis locked

            }
            else
            {
                originalBulletPos = glm::vec3(playerTransform.x, playerTransform.y, playerTransform.z + 0.6f);
                currentBulletPoz = originalBulletPos.z;

                //reset the bullet position back to the player
            }

#pragma endregion

#pragma region Dynamic AI

            if (isAIActive)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distr(-10, 10);
                
                glm::vec3 oldAIPos = glm::vec3(0.0f);

                switch (currentAIState)
                {
                case VERIFICATION:
                    wanderTimer += deltaTime; //goes up before wandering to give a buffer of time before it stops
                    oldAIPos = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z); //update ai position
                    randDistX = distr(gen); //random number between -10 and 10
                    randDistZ = distr(gen); //random number between -10 and 10
                    PositiveOrNegative = (rand() % 100) < 50; //is true if the random number is above 50 percent of 100
                    break;

                case WANDER:
                    wanderTimer -= deltaTime;//goes down while wandering to give a buffer of time before it stops
                    glm::vec3 calculatedDist = glm::vec3(0.0f);

                    if(PositiveOrNegative)
                        calculatedDist =  glm::vec3(oldAIPos.x + randDistX, 0, oldAIPos.z - randDistZ);
                    else
                        calculatedDist = glm::vec3(oldAIPos.x - randDistX, 0, oldAIPos.z + randDistZ);

                    //PositiveOrNegative switches up the axis on a coinflip to make the enenmy movement feel less robotic

                    glm::vec3 dirToCalculatedDist = glm::normalize(calculatedDist);

                    aiPosition.x = glm::clamp(aiPosition.x, -10.0f, 10.0f);
                    aiPosition.z = glm::clamp(aiPosition.z, -30.0f, 16.0f);

                    aiPosition += dirToCalculatedDist * 5.0f * deltaTime;

                    //move the enemy to a point along the direction of the calculated distance within a clamped zone

                    break;

                case ATTACK:

                    glm::vec3 aiToPlayer = playerTransform - aiPosition;
                    glm::vec3 dirAItoPlayer = glm::normalize(aiToPlayer);
                                      

                    if (aiPosition != playerTransform )
                    {
                        aiPosition += dirAItoPlayer * 6.0f * deltaTime;

                        //ai will go towards player in attack mode while their positions are not the same
                       
                    }

                    if (DistanceCheck(aiPosition, playerTransform, 0.5f))
                    {
                        isPlayerDead = true;

                        //stop player rendering if the enemy reaches within a 0.5 radius of the player
                    }
                    break;

                }

                if (wanderTimer <= 0.0f)
                {
                    currentAIState = VERIFICATION;

                   

                }
                else if(wanderTimer > 1.0f)
                {
                    currentAIState = WANDER;
                }

                //switching between verification and wander based on time


                glm::mat4 aiModel = glm::mat4(1.0f);
                aiModel = glm::translate(aiModel, aiPosition);

                shader->SetUniform4f("u_Colour", 1.0f, 1.0f, 0.0f, 1.0f);
                shader->SetUnifromMat4("model", aiModel);

                renderer->DrawObject(*va, *ib);

                glm::mat4 aiDetectionGizmo = glm::mat4(1.0f);
               

                if (DistanceCheck(aiPosition, bulletPosition, 0.5f))
                {
                    currentAIState = DEATH;
                    isAIActive = false;

                    //if enemy is shot, stop their rendering
                }

                if (!isLineOfSight)

                {
                    
                    aiDetectionGizmoTransform = glm::vec3(aiPosition.x, aiPosition.y + 2.5f, aiPosition.z);
                    aiDetectionGizmo = glm::translate(aiDetectionGizmo,aiDetectionGizmoTransform);
                    aiDetectionGizmo = glm::scale(aiDetectionGizmo, glm::vec3(12.0f, 5.0f, 12.0f));
                    shader->SetUniform4f("u_Colour", 1.0f, 0.0f, 0.0f, 1.0f);
                    shader->SetUnifromMat4("model", aiDetectionGizmo);

                    if (DistanceCheck(playerTransform, aiPosition, 7.5f))
                    {
                        shader->SetUniform4f("u_Colour", 0.0f, 1.0f, 0.0f, 1.0f);
                        currentAIState = ATTACK;

                        //gizmos turns green if the player is within the enemys radius +  attack mode
                    }
                    else if (!DistanceCheck(playerTransform, aiPosition, 7.5f) && currentAIState != WANDER)
                    {
                        currentAIState = VERIFICATION;

                        //reset if the player leaves the radius
                    }

                    //distance check mode

                   
 
                }
                else
                {

                    glm::mat4 lineOfSightGizmo = glm::mat4(1.0f);
                    lineOfSightGizmo = glm::translate(lineOfSightGizmo, glm::vec3(aiPosition.x, aiPosition.y + 1.5f, aiPosition.z + 2.0f));
                    lineOfSightGizmo = glm::scale(lineOfSightGizmo, glm::vec3(3.0f));
                    shader->SetUniform4f("u_Colour", 1.0f, 1.0f, 1.0f, 1.0f);
                    shader->SetUnifromMat4("model", lineOfSightGizmo);

                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                    renderer->DrawObject(*va, *ib);

                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    //cube in front of the enemy to represent their view angle 

                   
                    glm::vec3 enemyToPlayer = glm::normalize(playerTransform - aiPosition);

                    aiDetectionGizmoTransform = aiPosition + enemyToPlayer;
                    aiDetectionGizmo = glm::translate(aiDetectionGizmo, aiDetectionGizmoTransform);
                    aiDetectionGizmo = glm::scale(aiDetectionGizmo, glm::vec3(1.0f, 1.0f, 1.0f));
                    shader->SetUniform4f("u_Colour", 1.0f, 0.0f, 0.0f, 1.0f);
                    shader->SetUnifromMat4("model", aiDetectionGizmo);

                    if (LineOfSight(playerTransform, aiPosition, 0.0f))
                    {
                        shader->SetUniform4f("u_Colour", 0.0f, 1.0f, 0.1f, 1.0f);
                        currentAIState = ATTACK;
                    }
                    else if (!LineOfSight(playerTransform, aiPosition, 0.1f) && currentAIState != WANDER)
                    {
                        currentAIState = VERIFICATION;
                    }

                    //gizmos direction towards the player to represent how dot product is working

                  
                }

                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                renderer->DrawObject(*va, *ib);

                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                //render gizmos in either mode
               

#pragma endregion
                
            }

            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, glm::vec3(0.0f));

            playerToPlatform = platformPos - playerTransform;
            dirPlayerToPlatform = glm::normalize(playerToPlatform); 
            normalVec = playerTransform + dirPlayerToPlatform; 

            //calculating the direction between player and platform for collision on the xz plane
            //to add an equal force that sends the player back

            if (CollisionDetection(playerTransform, glm::vec3(1.0f), platformPos, platformSize))
            {
                
                //lightModel = glm::translate(lightModel, normalVec);
                //lightModel = glm::scale(lightModel, glm::vec3(1.0f));
                //shader->SetUniform4f("u_Colour", 1.0f, 1.0f, 1.0f, 1.0f);
                //shader->SetUnifromMat4("model", lightModel);

                //renderer->DrawObject(*va, *ib);

                //rendering a bright object for debugging purposes

                currentPlayerState = NEUTRAL;
                //reset player state

                isGrounded = true;

                //std::cout << "Collided" << std::endl;
            }
            else
                isGrounded = false;

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