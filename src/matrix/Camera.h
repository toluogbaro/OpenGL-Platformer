#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


enum Camera_Mode
{
	FREE_ROAM,
	PLAYER
};

class Camera
{

	private:

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraRight;

		float m_xAxisEuler;
		float m_yAxisEuler;
		float m_MoveSpeed;
		float m_MouseSensitivity;
		float m_FOV;
		float m_LastMouseX;
		float m_LastMouseY;

		bool m_FirstMouseInput;

	public:
		Camera(float moveSpeed, float fov)
		{
			m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			m_CameraPosition = glm::vec3(1.0f);
			m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
			m_xAxisEuler = 45.0f;
			m_yAxisEuler = 90.0f;
			m_MouseSensitivity = 0.1f;
			m_MoveSpeed = moveSpeed;
			m_FOV = fov;
			m_LastMouseX = 0.0f;
			m_LastMouseY = 0.0f;
			m_FirstMouseInput = true;
		}

		~Camera();

		//Camera Movement
		void ProcessInput(GLFWwindow* window, float deltaTime);

		//Mouse Movement
		void ProcessMovement(double xMousePosition, double yMousePosition);
		void ProcessScroll(double scrollDepth);

		//calculate lookat matrix after getting vector positions
		glm::mat4 ProcessViewMatrix();

		inline float GetFOV() { return m_FOV;  }

		inline float SetMouseSensitivity(float newSensitivity) { m_MouseSensitivity = newSensitivity; }
		inline float SetCameraYaw(float newYaw) { m_xAxisEuler = newYaw;  }
		inline float SetCameraPitch(float newPitch) { m_yAxisEuler = newPitch; }

		inline void SetCameraDirection()
		{

			glm::vec3 newFront = glm::vec3(1.0f);
			newFront.x = cos(glm::radians(m_xAxisEuler)) * cos(glm::radians(m_yAxisEuler));
			newFront.y = sin(glm::radians(m_yAxisEuler));
			newFront.z = sin(glm::radians(m_xAxisEuler)) * cos(glm::radians(m_yAxisEuler));

			m_CameraFront = glm::normalize(newFront);

			m_CameraRight = glm::normalize(glm::cross(m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
			m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
		}




};
