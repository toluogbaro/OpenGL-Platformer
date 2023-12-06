#include "Camera.h"




Camera::~Camera()
{
}

void Camera::ProcessInput(GLFWwindow* currentWindow, float deltaTime)
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


void Camera::ProcessMovement(double xPos, double yPos)
{
    if (m_FirstMouseInput)
    {
        m_LastMouseX = xPos;
        m_LastMouseY = yPos;
        m_FirstMouseInput = false;
        //added so that the mouse doesn't fly off when it first detects input
    }


    float xOffset = xPos - m_LastMouseX;
    float yOffset = m_LastMouseY - yPos;

    //current mouse input vs the last mouse input

    m_LastMouseX = xPos;
    m_LastMouseY = yPos;

    //setting the last mouse positions to the current ones

    xOffset *= m_MouseSensitivity;
    yOffset *= m_MouseSensitivity;

    //multiplying offset by a sensitivity value, for ex. 0.5f, dampens it

    m_xAxisEuler += xOffset; 
    m_yAxisEuler += yOffset; 

    // adding the current mouse position - the last mouse position gives you where the mouse is currently meant
    // to be in terms of these euler angles

    if (m_yAxisEuler > 89.0f)
        m_yAxisEuler = 89.0f;

    if (m_yAxisEuler < -89.0f)
        m_yAxisEuler = -89.0f;

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
