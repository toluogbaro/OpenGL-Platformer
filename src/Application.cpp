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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

float deltaTime;
float cameraSpeed = 10.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastMouseX = 0.0f;
float lastMouseY = 0.0f;
float yaw = -90.0f;
float pitch = -89.0f;
float fov = 45.0f;
float mouseSensitivity = 0.1f;

bool firstMouse = true;


void ProcessInput(GLFWwindow* currentWindow)
{
    

    if (glfwGetKey(currentWindow, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraFront * cameraSpeed * deltaTime;

    if (glfwGetKey(currentWindow, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraFront * cameraSpeed * deltaTime;

    if (glfwGetKey(currentWindow, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::cross(cameraFront, cameraUp) * cameraSpeed * deltaTime;

    if (glfwGetKey(currentWindow, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::cross(cameraFront, cameraUp) * cameraSpeed * deltaTime;
}

void mouse_callback(GLFWwindow* currentWindow, double xPos, double yPos) 
{

    if (firstMouse)
    {
        lastMouseX = xPos;
        lastMouseY = yPos;
        firstMouse = false;
        //added so that the mouse doesn't fly off when it first detects input
    }


    float xOffset = xPos - lastMouseX;
    float yOffset = lastMouseY - yPos;

    //current mouse input vs the last mouse input

    lastMouseX = xPos; 
    lastMouseY = yPos;

    //setting the last mouse positions to the current ones

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    //multiplying offset by a sensitivity value, for ex. 0.5f, dampens it

    yaw += xOffset; // yaw is the euler angle for the x axis
    pitch += yOffset; // pitch is the euler angle for the y axis

    // adding the current mouse position - the last mouse position gives you where the mouse is currently meant
    // to be in terms of these euler angles

    if (pitch > 89.0f)
        pitch = 89.0f;

    if (pitch < -89.0f)
        pitch = -89.0f;

    // clamping the pitch which is the y axis.
    // We dont want it to be able to go beyond looking at the sky or looking below us

    glm::vec3 direction; //vector 3 for camera direction

    direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    //need more understanding on why we're multiplying the sin and cos for each axis

    cameraFront = glm::normalize(direction);


}

void scroll_callback(GLFWwindow* currentWindow, double xOffset, double yOffset)
{
    fov -= (float)yOffset;

    if (fov < 1.0f)
        fov = 1.0f;

    if (fov > 45.0f)
        fov = 45.0f;
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
            
            float timeSinceStart = static_cast<float>(glfwGetTime());
            deltaTime = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;
            
            renderer->Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            ProcessInput(window);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

#pragma region ViewCalculationsw

            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetScrollCallback(window, scroll_callback);

            if (!glfwGetKey(window, GLFW_KEY_ESCAPE))
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                
            }

          
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


#pragma endregion ViewCalculations

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.1f, 100.0f);

           
            shader.SetUniform4f("u_Colour", r, 0.6f, 0.8f, 1.0f);
            shader.SetUnifromMat4("model", model);
            shader.SetUnifromMat4("view", view);
            shader.SetUnifromMat4("projection", projection);
            shader.Bind();


            renderer->DrawObject(va, ib, shader);

          

            static float blinkSpeed = 20.0f;

            if (shouldBlink)

            {
               

                if (r >= 1.0f)
                    increment = -0.05f;
                else if (r <= 0.0f)
                    increment = 0.05f;

                r += increment * blinkSpeed * deltaTime;
            }


            {
                
                static int counter = 0;

                ImVec2 standardButtonSize{ 200.0f, .0f };

                ImGui::Begin("Hello, world!");                         

                ImGui::SliderFloat("Blink Speed", &blinkSpeed, 20.0f, 100.0f); 

                ImGui::SliderFloat("Camera Speed", &cameraSpeed, 5.0f, 50.0f);

                ImGui::SliderFloat("Mouse Speed", &mouseSensitivity, 0.1f, 1.0f);

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