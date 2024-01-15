//Credit to https://learnopengl.com/ and Joey De Vries for the camera class
//Followed Tutorial: https://learnopengl.com/Getting-started/Camera

#include "Camera.h"

Camera::~Camera()
{
}

void Camera::ProcessKeyInput(GLFWwindow* currentWindow, float deltaTime)
{

    if (m_CameraMode == Camera_Mode::FREE_ROAM)
    {
        if (glfwGetKey(currentWindow, GLFW_KEY_W) == GLFW_PRESS)
            m_CameraPosition += m_CameraFront * m_MoveSpeed * deltaTime;

        if (glfwGetKey(currentWindow, GLFW_KEY_S) == GLFW_PRESS)
            m_CameraPosition -= m_CameraFront * m_MoveSpeed * deltaTime;

        if (glfwGetKey(currentWindow, GLFW_KEY_D) == GLFW_PRESS)
            m_CameraPosition += glm::cross(m_CameraFront, m_CameraUp) * m_MoveSpeed * deltaTime;

        if (glfwGetKey(currentWindow, GLFW_KEY_A) == GLFW_PRESS)
            m_CameraPosition -= glm::cross(m_CameraFront, m_CameraUp) * m_MoveSpeed * deltaTime;

    }


}


void Camera::ProcessMouseInput(GLFWwindow* currentWindow , double xPos, double yPos, int windowHeight, int windowWidth)
{

 /*   if (m_FirstMouseInput)
    {

        xPos = windowWidth / 2;
        yPos = windowHeight / 2;

        std::cout << "X postion: " << xPos << std::endl;
        std::cout << "Y postion: " << yPos << std::endl;

        m_FirstMouseInput = false;
        return;
    }*/

   
    m_XOffset = xPos - m_LastMouseX;
    m_YOffset = m_LastMouseY - yPos;


    //current mouse input vs the last mouse input

    m_LastMouseX = xPos;
    m_LastMouseY = yPos;

    //setting the last mouse positions to the current ones

    m_XOffset *= m_MouseSensitivity;
    m_YOffset *= m_MouseSensitivity;

    //multiplying offset by a sensitivity value, for ex. 0.5f, dampens it

    m_xAxisEuler += m_XOffset;
    m_yAxisEuler += m_YOffset;

    // adding the current mouse position - the last mouse position gives you where the mouse is currently meant
    // to be in terms of these euler angles

    m_yAxisEuler = glm::clamp(m_yAxisEuler, -89.0f, 89.0f);

    // clamping the pitch which is the y axis.
    // We dont want it to be able to go beyond looking at the sky or looking below us

    SetCameraDirection();


 
}


void Camera::ProcessScroll(double scrollDepth)
{
    m_FOV -= (float)scrollDepth;

    if (m_FOV < 1.0f)
        m_FOV = 1.0f;

    if (m_FOV > 45.0f)
        m_FOV = 45.0f;
}


glm::mat4 Camera::ProcessViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);

    return view;
}

void Camera::PlayerCamera(glm::vec3 playerPos)
{
    m_CameraPosition = playerPos + m_CameraOffset;
}


